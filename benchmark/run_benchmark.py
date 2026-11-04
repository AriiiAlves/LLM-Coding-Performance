"""
Benchmark of the AI-generated LeetCode solutions in ../generated_code.

    python benchmark/run_benchmark.py            # everything (tests, build, run, report)
    python benchmark/run_benchmark.py --help     # options

Steps
  1. Generates the tests (gen_tests.py), if they're missing or outdated.
  2. Writes a treated copy of every generated .cpp to ../treated_generated_code:
     the AI code is kept byte for byte; only a LeetCode-like preamble
     (#include <bits/stdc++.h> + using namespace std) and a main() for
     input/output + timing are added around it.
  3. Compiles: optimized build (-O2), sanitizer build (ASan + UBSan), and a
     "strict" syntax check of the untouched original (does it compile without
     the LeetCode preamble?).
  4. Runs the reference solutions to get the expected answers, and checks them
     against the brute-force oracles (oracle.py) on the small tests.
  5. Runs every solution on every test of its problem:
       edge + random  -> correctness (in parallel)
       heavy + stress -> correctness + timing (one at a time, median of N runs)
       sanitizer build on edge + random -> memory errors / undefined behavior
  6. Saves results/results.json and builds the report (make_report.py).
"""
import argparse
import datetime as dt
import json
import os
import platform
import re
import shutil
import signal
import statistics
import subprocess
import sys
import threading
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

BENCH = Path(__file__).resolve().parent
ROOT = BENCH.parent
GENERATED = ROOT / "generated_code"
TREATED = ROOT / "treated_generated_code"
BUILD = BENCH / "build"
TESTS = BENCH / "tests"
RESULTS = BENCH / "results"
HARNESS = BENCH / "harness"
REFERENCE = BENCH / "reference"

sys.path.insert(0, str(BENCH))
import gen_tests  # noqa: E402
import oracle  # noqa: E402

PROBLEMS = {
    4: ("Median of Two Sorted Arrays", "findMedianSortedArrays"),
    10: ("Regular Expression Matching", "isMatch"),
    23: ("Merge k Sorted Lists", "mergeKLists"),
    25: ("Reverse Nodes in k-Group", "reverseKGroup"),
    30: ("Substring with Concatenation of All Words", "findSubstring"),
    32: ("Longest Valid Parentheses", "longestValidParentheses"),
    37: ("Sudoku Solver", "solveSudoku"),
    41: ("First Missing Positive", "firstMissingPositive"),
    42: ("Trapping Rain Water", "trap"),
    44: ("Wildcard Matching", "isMatch"),
}

CXX = os.environ.get("CXX", "g++")
STD = "-std=c++20"
OPT_FLAGS = [STD, "-O2", "-Wall", "-Wextra"]
SAN_FLAGS = [STD, "-O1", "-g", "-fno-omit-frame-pointer", "-fsanitize=address,undefined"]
STACK_KB = 8 * 1024          # 8 MiB stack (the usual Linux default)
MEMORY_LIMIT_MB = 4096       # address-space limit for normal runs (MLE above it)
TIER_ORDER = ["edge", "random", "heavy", "stress"]
COUNTED_TIERS = {"edge", "random", "heavy"}   # tiers that decide "Accepted"

LISTNODE = """struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
"""


def log(msg):
    print(msg, flush=True)


# --------------------------------------------------------------------------
# Discovery + treatment
# --------------------------------------------------------------------------
def discover(models_filter=None, problems_filter=None):
    """Every (model, problem, prompt) slot. A slot without .cpp is 'no code'."""
    slots = []
    for model_dir in sorted(p for p in GENERATED.iterdir() if p.is_dir()):
        model = model_dir.name
        if models_filter and model not in models_filter:
            continue
        prompts = sorted({int(m.group(1)) for f in model_dir.iterdir()
                          if (m := re.match(r"\d+_prompt(\d+)\.(cpp|txt)$", f.name))}) or [0]
        for pid in PROBLEMS:
            if problems_filter and pid not in problems_filter:
                continue
            for prompt in prompts:
                cpp = model_dir / f"{pid}_prompt{prompt}.cpp"
                txt = model_dir / f"{pid}_prompt{prompt}.txt"
                slots.append({
                    "model": model, "pid": pid, "prompt": prompt,
                    "original": str(cpp.relative_to(ROOT)) if cpp.exists() else None,
                    "raw_response": str(txt.relative_to(ROOT)) if txt.exists() else None,
                })
    return slots


def treat(code, pid, origin):
    """Treated source = preamble + untouched AI code + harness. Returns (text, first, last line of AI code)."""
    uses_class = re.search(r"\bclass\s+Solution\b", code) is not None
    needs_listnode = pid in (23, 25) and re.search(r"\bstruct\s+ListNode\b", code) is None
    head = [
        "// " + "=" * 76,
        f"// [harness] Treated copy of {origin}",
        "// Changes (input/output + timing only):",
        "//   1. LeetCode judge preamble below: all std headers + 'using namespace std'",
        "//      (LeetCode adds these implicitly).",
        "//   2. A main() after the END marker that reads the test, calls the function,",
        "//      times the call and writes the output.",
        "// The code between the BEGIN/END markers is byte-for-byte the AI-generated code.",
        "// " + "=" * 76,
        "#include <bits/stdc++.h>",
        "using namespace std;",
        "#define HH_CALL(fn) " + ("Solution().fn" if uses_class else "fn"),
    ]
    if needs_listnode:
        head += ["// [harness] ListNode is provided by the judge on LeetCode", LISTNODE.rstrip()]
    head.append("// ===== BEGIN AI-GENERATED CODE (unchanged) =====")
    first = len(head) + 1
    body = code if code.endswith("\n") else code + "\n"
    last = first + body.count("\n") - 1
    tail = ["// ===== END AI-GENERATED CODE =====", "",
            (HARNESS / "common.hpp").read_text().rstrip(), "",
            (HARNESS / f"p{pid}.cpp").read_text().rstrip(), ""]
    return "\n".join(head) + "\n" + body + "\n".join(tail), first, last


def prepare(slots):
    TREATED.mkdir(exist_ok=True)
    (TREATED / "README.md").write_text(
        "# Treated generated code\n\n"
        "Copies of `../generated_code/*/*.cpp`, generated by `../benchmark/run_benchmark.py`.\n\n"
        "The AI code between the `BEGIN/END AI-GENERATED CODE` markers is unchanged (byte for byte).\n"
        "Only added around it:\n\n"
        "- a LeetCode judge preamble: `#include <bits/stdc++.h>` and `using namespace std;`\n"
        "  (LeetCode adds these implicitly; ListNode is added only if the code doesn't define it),\n"
        "- a `main()` that reads the test input, calls the function, times only the call and writes the output.\n\n"
        "Don't edit these files by hand: they are rewritten on every benchmark run.\n")
    for s in slots:
        if not s["original"]:
            continue
        code = (ROOT / s["original"]).read_text(encoding="utf-8")
        text, first, last = treat(code, s["pid"], f"generated_code/{s['model']}/{Path(s['original']).name}")
        out = TREATED / s["model"] / Path(s["original"]).name
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(text, encoding="utf-8")
        s.update(treated=str(out.relative_to(ROOT)), code_lines=(first, last), loc=count_loc(code))
    # References (not AI code), same treatment, kept inside the benchmark folder
    refs = {}
    for pid in PROBLEMS:
        code = (REFERENCE / f"{pid}.cpp").read_text()
        text, first, last = treat(code, pid, f"benchmark/reference/{pid}.cpp")
        out = BUILD / "reference" / f"p{pid}.cpp"
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(text)
        refs[pid] = {"model": "reference", "pid": pid, "prompt": 0, "treated": str(out),
                     "code_lines": (first, last), "loc": count_loc(code)}
    return refs


def count_loc(code):
    """Non-blank lines that are not only a comment."""
    n, in_block = 0, False
    for line in code.splitlines():
        t = line.strip()
        if in_block:
            if "*/" in t:
                in_block = False
                t = t.split("*/", 1)[1].strip()
            else:
                continue
        if t.startswith("/*"):
            if "*/" not in t:
                in_block = True
            continue
        if t and not t.startswith("//"):
            n += 1
    return n


# --------------------------------------------------------------------------
# Compilation
# --------------------------------------------------------------------------
def first_errors(stderr, limit=6):
    lines = [l for l in stderr.splitlines() if ": error:" in l or ": fatal error:" in l]
    return [re.sub(r"^.*?/([^/]+\.cpp):", r"\1:", l) for l in lines[:limit]]


def compile_slot(s):
    src = ROOT / s["treated"] if not Path(s["treated"]).is_absolute() else Path(s["treated"])
    out_dir = BUILD / s["model"]
    out_dir.mkdir(parents=True, exist_ok=True)
    stem = f"p{s['pid']}_prompt{s['prompt']}"
    exe, san = out_dir / stem, out_dir / f"{stem}.asan"

    r = subprocess.run([CXX, *OPT_FLAGS, "-o", str(exe), str(src)], capture_output=True, text=True)
    s["compiled"] = r.returncode == 0
    s["compile_errors"] = first_errors(r.stderr) if r.returncode else []
    first, last = s["code_lines"]
    warn = set()
    for m in re.finditer(r"^(.*?):(\d+):\d+: warning: (.*)$", r.stderr, re.M):
        if Path(m.group(1)).name == src.name and first <= int(m.group(2)) <= last:
            warn.add((int(m.group(2)) - first + 1, m.group(3)))
    s["warnings"] = [f"line {ln}: {msg}" for ln, msg in sorted(warn)]
    s["exe"] = str(exe) if s["compiled"] else None

    s["asan_exe"] = None
    if s["compiled"]:
        r = subprocess.run([CXX, *SAN_FLAGS, "-o", str(san), str(src)], capture_output=True, text=True)
        s["asan_exe"] = str(san) if r.returncode == 0 else None

    # Does the untouched original compile on its own (own includes, no preamble)?
    if s.get("original"):
        r = subprocess.run([CXX, STD, "-fsyntax-only", str(ROOT / s["original"])], capture_output=True, text=True)
        s["strict_compiles"] = r.returncode == 0
        s["strict_errors"] = first_errors(r.stderr, 3)
    return s


# --------------------------------------------------------------------------
# Running
# --------------------------------------------------------------------------
_run_id = 0
_run_lock = threading.Lock()


def run_binary(exe, inp, timeout, sanitizer=False):
    """Runs one test. Returns the process outcome and the raw output text."""
    global _run_id
    with _run_lock:
        _run_id += 1
        rid = _run_id
    tmp = BUILD / "runs"
    tmp.mkdir(parents=True, exist_ok=True)
    out_f, so_f, se_f = (tmp / f"{rid}.{x}" for x in ("out", "stdout", "stderr"))
    cmd = ["prlimit", f"--stack={STACK_KB * 1024}"]
    if not sanitizer:   # ASan reserves terabytes of virtual memory
        cmd.append(f"--as={MEMORY_LIMIT_MB * 1024 * 1024}")
    cmd += [exe, str(inp), str(out_f)]
    env = dict(os.environ, ASAN_OPTIONS="detect_leaks=0:abort_on_error=0",
               UBSAN_OPTIONS="print_stacktrace=0")
    killed = threading.Event()
    with open(so_f, "wb") as so, open(se_f, "wb") as se:
        start = time.perf_counter()
        p = subprocess.Popen(cmd, stdout=so, stderr=se, stdin=subprocess.DEVNULL, env=env)

        def kill():
            killed.set()
            try:
                os.kill(p.pid, signal.SIGKILL)
            except ProcessLookupError:
                pass
        timer = threading.Timer(timeout, kill)
        timer.start()
        _, status, ru = os.wait4(p.pid, 0)
        timer.cancel()
        wall = time.perf_counter() - start
        p.returncode = 0  # already reaped with wait4
    res = {
        "wall_ms": round(wall * 1000, 2),
        "rss_kb": ru.ru_maxrss,
        "timeout": killed.is_set(),
        "signal": signal.Signals(os.WTERMSIG(status)).name if os.WIFSIGNALED(status) else None,
        "exit": os.WEXITSTATUS(status) if os.WIFEXITED(status) else None,
        "stdout_bytes": so_f.stat().st_size,
        "stderr": se_f.read_text(errors="replace")[-4000:],
    }
    out = out_f.read_text(errors="replace") if out_f.exists() else ""
    for f in (out_f, so_f, se_f):
        f.unlink(missing_ok=True)
    return res, out


def parse_output(out):
    lines, stats = [], {}
    for line in out.split("\n"):
        if line.startswith("#STAT "):
            _, k, v = line.split()
            stats[k] = int(v)
        else:
            lines.append(line)
    if lines and lines[-1] == "":
        lines.pop()
    return lines, stats


def same(pid, got, exp):
    if pid == 4:
        try:
            return abs(float(got) - float(exp)) <= 1.5e-5
        except ValueError:
            return False
    if pid == 30:
        return sorted(got.split(), key=lambda x: (len(x), x)) == sorted(exp.split(), key=lambda x: (len(x), x))
    return got.strip() == exp.strip()


def judge(pid, test, res, out, expected):
    lines, stats = parse_output(out)
    ok = sum(1 for g, e in zip(lines, expected) if same(pid, g, e))
    r = {"test": test["name"], "tier": test["tier"], "cases": test["cases"], "cases_ok": ok,
         "time_ms": round(stats["time_ns"] / 1e6, 4) if "time_ns" in stats else None,
         "mem_kb": stats.get("mem_kb"), "rss_kb": res["rss_kb"], "wall_ms": res["wall_ms"],
         "stdout_bytes": res["stdout_bytes"]}
    err = res["stderr"]
    if res["timeout"]:
        r["status"] = "TLE"
    elif "bad_alloc" in err:
        r["status"] = "MLE"
    elif res["signal"] or res["exit"] or "time_ns" not in stats:
        r["status"] = "RE"
        r["detail"] = res["signal"] or f"exit code {res['exit']}"
        m = re.search(r"terminate called after throwing an instance of '([^']+)'(?:\s+what\(\):\s*(.*))?", err)
        if m:
            r["detail"] = f"exception {m.group(1)}" + (f": {m.group(2).strip()}" if m.group(2) else "")
        elif res["signal"] == "SIGSEGV":
            r["detail"] = "SIGSEGV (segmentation fault / stack overflow)"
    elif ok == test["cases"] and len(lines) == test["cases"]:
        r["status"] = "AC"
    else:
        r["status"] = "WA"
        bad = next((i for i, (g, e) in enumerate(zip(lines, expected)) if not same(pid, g, e)), None)
        if bad is not None:
            r["detail"] = f"case {bad + 1}: expected {short(expected[bad])}, got {short(lines[bad])}"
    if res["stdout_bytes"]:
        r["printed_to_stdout"] = True
    return r


def short(s, n=60):
    return s if len(s) <= n else s[:n] + f"... ({len(s)} chars)"


def sanitizer_issue(stderr):
    for pat in (r"SUMMARY: AddressSanitizer: .*", r"SUMMARY: UndefinedBehaviorSanitizer: .*",
                r"runtime error: .*", r"ERROR: AddressSanitizer: .*"):
        m = re.search(pat, stderr)
        if m:
            return re.sub(r"/\S*/", "", m.group(0))[:200]
    return None


# --------------------------------------------------------------------------
# Main
# --------------------------------------------------------------------------
def machine_info():
    cpu = platform.processor()
    try:
        for line in open("/proc/cpuinfo"):
            if line.startswith("model name"):
                cpu = line.split(":", 1)[1].strip()
                break
    except OSError:
        pass
    gcc = subprocess.run([CXX, "--version"], capture_output=True, text=True).stdout.splitlines()[0]
    return {"cpu": cpu, "cores": os.cpu_count(), "compiler": gcc, "os": platform.platform()}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 2) // 2),
                    help="parallel runs for the untimed phases (edge/random/sanitizer)")
    ap.add_argument("--repeats", type=int, default=3, help="timed runs per heavy/stress test (median is kept)")
    ap.add_argument("--timeout", type=float, default=10.0, help="seconds per run (edge/random/heavy)")
    ap.add_argument("--stress-timeout", type=float, default=20.0, help="seconds per stress run")
    ap.add_argument("--models", nargs="*", help="only these model folders")
    ap.add_argument("--problems", nargs="*", type=int, help="only these problem ids")
    ap.add_argument("--no-sanitizers", action="store_true", help="skip the ASan/UBSan runs")
    ap.add_argument("--regen-tests", action="store_true", help="regenerate the tests")
    args = ap.parse_args()

    if not shutil.which("prlimit"):
        sys.exit("prlimit (util-linux) is needed to apply the stack/memory limits")

    started = time.time()
    gen_tests.generate(force=args.regen_tests)
    manifest = json.loads((TESTS / "manifest.json").read_text())["tests"]

    slots = discover(args.models, args.problems)
    log(f"{len(slots)} solution slots from {len({s['model'] for s in slots})} models")
    refs = prepare(slots)
    refs = {pid: r for pid, r in refs.items() if not args.problems or pid in args.problems}

    log("Compiling...")
    to_compile = [s for s in slots if s.get("treated")] + list(refs.values())
    with ThreadPoolExecutor(args.jobs) as ex:
        list(ex.map(compile_slot, to_compile))
    for r in refs.values():
        if not r["compiled"]:
            sys.exit(f"Reference {r['pid']} does not compile: {r['compile_errors']}")

    # Expected answers from the references, double-checked by the oracles
    log("Computing expected answers with the reference solutions...")
    expected = {}
    for pid in refs:
        for test in manifest[str(pid)]:
            inp = TESTS / f"p{pid}" / f"{test['name']}.in"
            res, out = run_binary(refs[pid]["exe"], inp, timeout=120)
            lines, stats = parse_output(out)
            if res["timeout"] or res["signal"] or res["exit"] or len(lines) != test["cases"]:
                sys.exit(f"Reference failed on p{pid}/{test['name']}: {res}")
            expected[(pid, test["name"])] = lines
            (TESTS / f"p{pid}" / f"{test['name']}.ans").write_text("\n".join(lines) + "\n")
            if test["tier"] in ("edge", "random"):
                want = oracle.answers(pid, inp.read_text())
                for i, (g, e) in enumerate(zip(lines, want)):
                    good = oracle.sudoku_ok(e, g) if pid == 37 else same(pid, g, e)
                    if not good:
                        sys.exit(f"Reference disagrees with the oracle on p{pid}/{test['name']} case {i + 1}: "
                                 f"reference {short(g)} vs oracle {short(e)}")
    log("Reference answers verified by the oracles")

    runnable = [s for s in slots if s.get("exe")] + list(refs.values())
    for s in runnable:
        s["tests"] = {}
        s["sanitizer"] = []

    # Phase A: correctness tiers in parallel
    jobs = [(s, t) for s in runnable for t in manifest[str(s["pid"])] if t["tier"] in ("edge", "random")]
    log(f"Phase A: {len(jobs)} correctness runs (edge/random), {args.jobs} in parallel")

    def do_a(job):
        s, t = job
        inp = TESTS / f"p{s['pid']}" / f"{t['name']}.in"
        res, out = run_binary(s["exe"], inp, args.timeout)
        s["tests"][t["name"]] = judge(s["pid"], t, res, out, expected[(s["pid"], t["name"])])

    with ThreadPoolExecutor(args.jobs) as ex:
        list(ex.map(do_a, jobs))

    # Phase B: timed tiers, one process at a time
    jobs = [(s, t) for s in runnable for t in manifest[str(s["pid"])] if t["tier"] in ("heavy", "stress")]
    log(f"Phase B: {len(jobs)} timed tests (heavy/stress), sequential, median of {args.repeats}")
    for i, (s, t) in enumerate(jobs, 1):
        inp = TESTS / f"p{s['pid']}" / f"{t['name']}.in"
        timeout = args.stress_timeout if t["tier"] == "stress" else args.timeout
        runs = []
        for rep in range(args.repeats):
            res, out = run_binary(s["exe"], inp, timeout)
            r = judge(s["pid"], t, res, out, expected[(s["pid"], t["name"])])
            runs.append(r)
            if r["status"] != "AC":   # failed runs are not repeated
                break
        best = runs[0]
        if best["status"] == "AC" and len(runs) > 1:
            best["time_ms"] = round(statistics.median(r["time_ms"] for r in runs), 4)
            best["wall_ms"] = round(statistics.median(r["wall_ms"] for r in runs), 2)
            best["time_runs_ms"] = [r["time_ms"] for r in runs]
        s["tests"][t["name"]] = best
        if i % 25 == 0 or best["status"] != "AC":
            log(f"  [{i}/{len(jobs)}] {s['model']} p{s['pid']} {t['name']}: {best['status']}"
                + (f" ({best['time_ms']} ms)" if best.get("time_ms") is not None else ""))

    # Phase C: sanitizers
    if not args.no_sanitizers:
        jobs = [(s, t) for s in runnable if s.get("asan_exe")
                for t in manifest[str(s["pid"])] if t["tier"] in ("edge", "random")]
        log(f"Phase C: {len(jobs)} sanitizer runs (ASan + UBSan)")

        def do_c(job):
            s, t = job
            inp = TESTS / f"p{s['pid']}" / f"{t['name']}.in"
            res, _ = run_binary(s["asan_exe"], inp, args.timeout * 3, sanitizer=True)
            issue = sanitizer_issue(res["stderr"])
            if issue:
                s["sanitizer"].append({"test": t["name"], "issue": issue})

        with ThreadPoolExecutor(args.jobs) as ex:
            list(ex.map(do_c, jobs))

    # Save
    def clean(s):
        keys = ["model", "pid", "prompt", "original", "raw_response", "treated", "loc", "compiled",
                "compile_errors", "warnings", "strict_compiles", "strict_errors", "sanitizer"]
        d = {k: s.get(k) for k in keys if k in s}
        if "tests" in s:
            order = {t["name"]: i for i, t in enumerate(manifest[str(s["pid"])])}
            d["tests"] = sorted(s["tests"].values(), key=lambda r: order[r["test"]])
        if d.get("sanitizer"):
            d["sanitizer"] = sorted(d["sanitizer"], key=lambda x: x["test"])
        return d

    RESULTS.mkdir(exist_ok=True)
    data = {
        "meta": {"date": dt.datetime.now().isoformat(timespec="seconds"), **machine_info(),
                 "flags": " ".join(OPT_FLAGS), "sanitizer_flags": " ".join(SAN_FLAGS),
                 "timeout_s": args.timeout, "stress_timeout_s": args.stress_timeout,
                 "repeats": args.repeats, "stack_kb": STACK_KB, "memory_limit_mb": MEMORY_LIMIT_MB,
                 "seed": gen_tests.SEED, "duration_s": round(time.time() - started, 1)},
        "problems": {str(pid): {"title": PROBLEMS[pid][0], "function": PROBLEMS[pid][1],
                                "tests": manifest[str(pid)]} for pid in PROBLEMS
                     if not args.problems or pid in args.problems},
        "solutions": [clean(s) for s in slots],
        "reference": {str(pid): clean(r) for pid, r in refs.items()},
    }
    (RESULTS / "results.json").write_text(json.dumps(data, indent=1))
    log(f"Saved {RESULTS / 'results.json'} ({data['meta']['duration_s']} s)")

    import make_report
    make_report.build(data)


if __name__ == "__main__":
    main()
