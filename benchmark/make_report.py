"""
Builds the statistics from results/results.json:

    results/report.pdf    full report: charts, tables, and how every metric is computed
    results/summary.md    markdown tables (e.g. for the README)
    results/tests.csv     one row per (model, problem, test)
    console               model ranking + verdict matrix

    .venv/bin/python benchmark/make_report.py      # rebuild without re-running the benchmark

The PDF is rendered from a print-ready HTML page with headless Chrome/Chromium
(set CHROME=/path/to/chrome if it isn't found). Without a browser, the HTML is
saved to results/report.html instead.
"""
import csv
import html
import json
import math
import os
import re
import shutil
import subprocess
import tempfile
from collections import Counter
from pathlib import Path

BENCH = Path(__file__).resolve().parent
RESULTS = BENCH / "results"
COUNTED = ("edge", "random", "heavy")

VERDICT_TEXT = {"AC": "Accepted", "WA": "Wrong Answer", "RE": "Runtime Error", "TLE": "Time Limit Exceeded",
                "MLE": "Memory Limit Exceeded", "CE": "Compile Error", "NO_CODE": "No code"}
# Fixed categorical order (one color per model, always the same model -> same color)
SERIES = ["#2a78d6", "#eb6834", "#1baf7a", "#eda100", "#e87ba4", "#008300", "#6250d6", "#e34948"]

# Problem statements (short), LeetCode constraints and the reference algorithm
PROBLEM_INFO = {
    4: ("Return the median of two sorted arrays (LeetCode asks for O(log(m+n)) time).",
        "0 ≤ m, n ≤ 1000 · 1 ≤ m + n ≤ 2000 · −10⁶ ≤ value ≤ 10⁶",
        "Binary search on the partition of the smaller array, O(log min(m, n))."),
    10: ("Regular expression matching with '.' (any char) and '*' (zero or more of the previous element), "
         "covering the entire string.",
         "1 ≤ |s|, |p| ≤ 20 · lowercase letters, '.', '*' · every '*' follows a valid character",
         "Bottom-up dynamic programming, O(|s|·|p|)."),
    23: ("Merge k sorted linked lists into one sorted linked list.",
         "0 ≤ k ≤ 10⁴ · each list ≤ 500 nodes · total ≤ 10⁴ nodes · −10⁴ ≤ value ≤ 10⁴",
         "Min-heap of the list heads, O(N log k)."),
    25: ("Reverse the nodes of a linked list k at a time; a final group shorter than k stays as it is.",
         "1 ≤ k ≤ n ≤ 5000 · 0 ≤ value ≤ 1000",
         "Iterative in-place reversal, O(n) time, O(1) extra memory."),
    30: ("Find every start index of a substring that is a concatenation of all the words, in any order.",
         "1 ≤ |s| ≤ 10⁴ · 1 ≤ #words ≤ 5000 · 1 ≤ |word| ≤ 30 · all words have the same length",
         "Sliding window for each offset over precomputed word ids, O(|s|·|word|)."),
    32: ("Length of the longest well-formed (valid) parentheses substring.",
         "0 ≤ |s| ≤ 3·10⁴ · s contains only '(' and ')'",
         "Stack of indices, O(n)."),
    37: ("Fill the empty cells of a 9×9 sudoku (the input has exactly one solution).",
         "9×9 board · digits 1–9 or '.' · unique solution",
         "Backtracking with bitmasks, always filling the cell with the fewest candidates first."),
    41: ("Smallest missing positive integer (LeetCode asks for O(n) time and O(1) extra memory).",
         "1 ≤ n ≤ 10⁵ · −2³¹ ≤ value ≤ 2³¹ − 1",
         "Cyclic placement (value v goes to index v − 1), O(n) time, O(1) memory."),
    42: ("How much rain water is trapped between bars of the given heights.",
         "1 ≤ n ≤ 2·10⁴ · 0 ≤ height ≤ 10⁵",
         "Two pointers, O(n) time, O(1) memory."),
    44: ("Wildcard matching with '?' (any single char) and '*' (any sequence), covering the entire string.",
         "0 ≤ |s|, |p| ≤ 2000 · s lowercase · p lowercase, '?', '*'",
         "Greedy scan that backtracks to the last '*', O(1) memory (O(|s|·|p|) worst case)."),
}


# --------------------------------------------------------------------------
# Statistics
# --------------------------------------------------------------------------
def display_name(model):
    name = re.sub(r"^[^_]+_", "", model)   # drop the "org_" prefix
    return name.replace("_no_think", " (no think)")


def geomean(xs):
    xs = [x for x in xs if x and x > 0]
    return math.exp(sum(map(math.log, xs)) / len(xs)) if xs else None


def verdict(sol):
    if not sol.get("original"):
        return "NO_CODE", None
    if not sol.get("compiled"):
        return "CE", None
    for t in sol["tests"]:
        if t["tier"] in COUNTED and t["status"] != "AC":
            return t["status"], t
    return "AC", None


def analyze(data):
    ref = {int(k): v for k, v in data["reference"].items()}
    problems = {int(k): v for k, v in data["problems"].items()}
    prompts = sorted({s["prompt"] for s in data["solutions"]})
    multi_prompt = len(prompts) > 1

    rows, cells = {}, {}
    for s in data["solutions"]:
        pid = s["pid"]
        if pid not in problems:
            continue
        key = s["model"] + (f" · prompt{s['prompt']}" if multi_prompt else "")
        v, failing = verdict(s)
        tests = s.get("tests", [])
        counted = [t for t in problems[pid]["tests"] if t["tier"] in COUNTED]
        stress = [t for t in problems[pid]["tests"] if t["tier"] == "stress"]
        by_name = {t["test"]: t for t in tests}
        ref_by = {t["test"]: t for t in ref[pid]["tests"]}
        passed = sum(1 for t in counted if by_name.get(t["name"], {}).get("status") == "AC")
        stress_ok = sum(1 for t in stress if by_name.get(t["name"], {}).get("status") == "AC")
        ratios, mem_ratios = [], []
        for t in tests:
            r = ref_by.get(t["test"])
            if t["tier"] == "heavy" and t["status"] == "AC" and r and r.get("time_ms") is not None:
                ratios.append(max(t["time_ms"], 1e-3) / max(r["time_ms"], 1e-3))
                if t.get("mem_kb") is not None and r.get("mem_kb") is not None:
                    mem_ratios.append((t["mem_kb"] + 64) / (r["mem_kb"] + 64))
        heavy = [t for t in problems[pid]["tests"] if t["tier"] == "heavy"]
        all_heavy_ok = bool(tests) and all(by_name.get(t["name"], {}).get("status") == "AC" for t in heavy)
        cells[(key, pid)] = {
            "verdict": v, "failing": failing, "passed": passed, "total": len(counted),
            "stress_ok": stress_ok, "stress_total": len(stress),
            "speed": geomean(ratios), "mem": geomean(mem_ratios),
            "heavy_ms": sum(by_name[t["name"]]["time_ms"] for t in heavy) if all_heavy_ok else None,
            "sanitizer": s.get("sanitizer") or [], "warnings": s.get("warnings") or [],
            "compile_errors": s.get("compile_errors") or [], "strict": s.get("strict_compiles"),
            "strict_errors": s.get("strict_errors") or [], "loc": s.get("loc"),
            "printed": any(t.get("printed_to_stdout") for t in tests), "sol": s,
        }
        row = rows.setdefault(key, {"key": key, "model": s["model"], "name": display_name(s["model"]) +
                                    (f" · prompt{s['prompt']}" if multi_prompt else ""), "cells": []})
        row["cells"].append(cells[(key, pid)])

    for row in rows.values():
        cs = row["cells"]
        row["accepted"] = sum(c["verdict"] == "AC" for c in cs)
        row["problems"] = len(cs)
        row["passed"] = sum(c["passed"] for c in cs)
        row["total"] = sum(c["total"] for c in cs)
        row["compiles"] = sum(c["verdict"] not in ("NO_CODE", "CE") for c in cs)
        row["no_code"] = sum(c["verdict"] == "NO_CODE" for c in cs)
        row["strict"] = sum(bool(c["strict"]) for c in cs)
        row["stress_ok"] = sum(c["stress_ok"] for c in cs)
        row["stress_total"] = sum(c["stress_total"] for c in cs)
        row["speed"] = geomean([c["speed"] for c in cs if c["verdict"] == "AC"])
        row["mem"] = geomean([c["mem"] for c in cs if c["verdict"] == "AC"])
        row["sanitizer"] = sum(bool(c["sanitizer"]) for c in cs)
        row["warnings"] = sum(len(c["warnings"]) for c in cs)
        locs = [c["loc"] for c in cs if c["loc"]]
        row["loc"] = sum(locs) / len(locs) if locs else None
        row["pass_rate"] = row["passed"] / row["total"] if row["total"] else 0

    order = sorted(rows.values(), key=lambda r: (-r["accepted"], -r["pass_rate"], r["speed"] or 1e9))
    models = sorted({r["model"] for r in rows.values()})
    for r in order:
        r["color"] = models.index(r["model"]) % len(SERIES)
    return {"rows": order, "cells": cells, "problems": problems, "ref": ref, "meta": data["meta"]}


# --------------------------------------------------------------------------
# Text outputs (console, markdown, csv)
# --------------------------------------------------------------------------
def fmt_x(x):
    if x is None:
        return "-"
    return f"{x:.2f}x" if x < 10 else f"{x:.0f}x"


def fmt_ms(v):
    if v is None:
        return "-"
    if v >= 100:
        return f"{v:,.0f} ms"
    if v >= 1:
        return f"{v:.1f} ms"
    return f"{v:.3f} ms"


def cell_label(c):
    v = c["verdict"]
    if v == "AC":
        return "AC " + fmt_x(c["speed"])
    if v == "NO_CODE":
        return "No code"
    if v == "CE":
        return "CE"
    return f"{v} {c['passed']}/{c['total']}"


def model_table(A):
    head = ["#", "Model", "Accepted", "Tests passed", "Compiles", "Standalone", "Speed vs ref",
            "Memory vs ref", "Stress passed", "Sanitizer", "Warnings", "Avg LOC"]
    rows = []
    for i, r in enumerate(A["rows"], 1):
        rows.append([str(i), r["name"], f"{r['accepted']}/{r['problems']}",
                     f"{r['passed']}/{r['total']} ({r['pass_rate'] * 100:.1f}%)",
                     f"{r['compiles']}/{r['problems']}", f"{r['strict']}/{r['problems']}",
                     fmt_x(r["speed"]), fmt_x(r["mem"]), f"{r['stress_ok']}/{r['stress_total']}",
                     str(r["sanitizer"]), str(r["warnings"]), f"{r['loc']:.0f}" if r["loc"] else "-"])
    return head, rows


def matrix_table(A):
    pids = sorted(A["problems"])
    head = ["Model"] + [str(p) for p in pids]
    rows = [[r["name"]] + [cell_label(A["cells"][(r["key"], p)]) for p in pids] for r in A["rows"]]
    return head, rows


def md_table(head, rows):
    out = ["| " + " | ".join(head) + " |", "|" + "|".join("---" for _ in head) + "|"]
    out += ["| " + " | ".join(r) + " |" for r in rows]
    return "\n".join(out)


def console_table(head, rows):
    w = [max(len(h), *(len(r[i]) for r in rows)) for i, h in enumerate(head)]
    line = lambda r: "  ".join(c.ljust(w[i]) for i, c in enumerate(r))
    return "\n".join([line(head), "  ".join("-" * x for x in w)] + [line(r) for r in rows])


def write_csv(A, path):
    with open(path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["model", "problem", "verdict", "test", "tier", "status", "cases_ok", "cases",
                    "time_ms", "ref_time_ms", "mem_kb", "ref_mem_kb", "rss_kb", "detail"])
        for r in A["rows"]:
            for pid in sorted(A["problems"]):
                c = A["cells"][(r["key"], pid)]
                ref_by = {t["test"]: t for t in A["ref"][pid]["tests"]}
                tests = c["sol"].get("tests") or []
                if not tests:
                    w.writerow([r["key"], pid, c["verdict"]] + [""] * 11)
                for t in tests:
                    rt = ref_by.get(t["test"], {})
                    w.writerow([r["key"], pid, c["verdict"], t["test"], t["tier"], t["status"], t["cases_ok"],
                                t["cases"], t.get("time_ms"), rt.get("time_ms"), t.get("mem_kb"),
                                rt.get("mem_kb"), t.get("rss_kb"), t.get("detail", "")])


# --------------------------------------------------------------------------
# PDF (print-ready HTML -> headless Chrome)
# --------------------------------------------------------------------------
E = html.escape
STATUS_CLASS = {"AC": "good", "WA": "critical", "RE": "critical", "MLE": "critical",
                "TLE": "warning", "CE": "serious", "NO_CODE": "none"}
STATUS_ICON = {"AC": "✓", "WA": "✗", "RE": "✗", "MLE": "✗", "TLE": "⏱", "CE": "⚠", "NO_CODE": "∅"}

CSS = """
@page { size: A4 landscape; margin: 10mm 12mm 10mm; }
:root {
  --surface: #ffffff; --panel: #f6f6f3; --ink: #0b0b0b; --ink-2: #52514e; --muted: #898781;
  --grid: #e1e0d9; --axis: #c3c2b7; --ring: rgba(11,11,11,0.12);
  --good: #0ca30c; --warning: #fab219; --serious: #ec835a; --critical: #d03b3b; --none: #e1e0d9;
  %SERIES%
}
* { box-sizing: border-box; -webkit-print-color-adjust: exact; print-color-adjust: exact; }
html, body { margin: 0; background: var(--surface); color: var(--ink);
  font: 10.5px/1.45 system-ui, -apple-system, "Segoe UI", "Noto Sans", sans-serif; }
.page { break-before: page; }
h1 { font-size: 30px; margin: 0 0 6px; letter-spacing: -0.01em; }
h2 { font-size: 17px; margin: 0 0 4px; }
h3 { font-size: 12.5px; margin: 14px 0 5px; }
p { margin: 0 0 8px; }
.sub { color: var(--ink-2); margin-bottom: 12px; }
.small { font-size: 9.5px; color: var(--ink-2); }
.card { background: var(--panel); border-radius: 8px; padding: 12px 14px; }
.cols { display: grid; grid-template-columns: 1fr 1fr; gap: 18px; }
.cols3 { display: grid; grid-template-columns: 1.2fr 1fr; gap: 18px; }
.tiles { display: grid; grid-template-columns: repeat(4, 1fr); gap: 10px; margin: 14px 0 18px; }
.tile .v { font-size: 24px; font-weight: 650; }
.tile .l { color: var(--ink-2); }
table { border-collapse: collapse; width: 100%; font-variant-numeric: tabular-nums; }
th, td { padding: 3px 7px; text-align: left; border-bottom: 1px solid var(--grid); vertical-align: top; }
th { color: var(--ink-2); font-weight: 600; font-size: 9.5px; border-bottom: 1px solid var(--axis); }
td.num, th.num { text-align: right; white-space: nowrap; }
td.c, th.c { text-align: center; white-space: nowrap; }
tr { break-inside: avoid; }
thead { display: table-header-group; }
.swatch { display: inline-block; width: 8px; height: 8px; border-radius: 50%; margin-right: 6px; }
.badge { display: inline-block; padding: 1px 6px; border-radius: 5px; font-size: 9.5px; font-weight: 650;
  white-space: nowrap; }
.good { background: var(--good); color: #fff; } .warning { background: var(--warning); color: #0b0b0b; }
.serious { background: var(--serious); color: #0b0b0b; } .critical { background: var(--critical); color: #fff; }
.none { background: var(--none); color: #52514e; }
.bars .row { display: grid; grid-template-columns: 200px 1fr 50px; align-items: center; gap: 10px; height: 24px; }
.bars .track { position: relative; height: 12px; }
.bars .bar { position: absolute; left: 0; top: 0; height: 12px; border-radius: 0 4px 4px 0; }
.bars .val { text-align: right; font-variant-numeric: tabular-nums; }
.legend { display: flex; flex-wrap: wrap; gap: 14px; margin: 2px 0 6px; color: var(--ink-2); }
svg text { fill: var(--muted); font-size: 10px; font-family: inherit; }
svg .lbl { fill: var(--ink); font-size: 10.5px; }
ul { margin: 4px 0 8px; padding-left: 16px; } li { margin: 2px 0; }
dl.gloss { display: grid; grid-template-columns: 110px 1fr; gap: 5px 12px; margin: 0; }
dl.gloss dt { font-weight: 650; } dl.gloss dd { margin: 0; color: var(--ink-2); }
code { font-family: ui-monospace, "DejaVu Sans Mono", Menlo, monospace; font-size: 9.5px; }
.issues li { color: var(--ink-2); }
.pill { display: inline-block; border: 1px solid var(--axis); border-radius: 10px; padding: 0 7px; margin-right: 4px;
  font-size: 9.5px; color: var(--ink-2); }
.foot { color: var(--muted); font-size: 9px; margin-top: 10px; }
"""


def clip(s, n=105):
    s = re.sub(r"\s+", " ", s)
    return s if len(s) <= n else s[:n] + "…"


def badge(v, text=None):
    return f'<span class="badge {STATUS_CLASS.get(v, "none")}">{STATUS_ICON.get(v, "")} {E(text or VERDICT_TEXT[v])}</span>'


def sw(r):
    return f"<span class='swatch' style='background:var(--s{r['color']})'></span>"


def matrix_badge(c):
    v = c["verdict"]
    if v == "AC":
        return badge(v, "AC " + fmt_x(c["speed"]))
    if v == "NO_CODE":
        return badge(v, "No code")
    if v == "CE":
        return badge(v, "CE")
    return badge(v, f"{v} {c['passed']}/{c['total']}")


def svg_dotplot(A):
    pids = sorted(A["problems"])
    rows = A["rows"]
    W, left, right, top, row_h = 1000, 250, 20, 24, 25
    H = top + row_h * len(pids) + 26
    vals = [A["cells"][(r["key"], p)]["speed"] for r in rows for p in pids
            if A["cells"][(r["key"], p)]["verdict"] == "AC" and A["cells"][(r["key"], p)]["speed"]]
    lo = 10 ** math.floor(math.log10(min(vals + [0.5])))
    hi = 10 ** math.ceil(math.log10(max(vals + [2])))
    span = math.log10(hi) - math.log10(lo)
    x = lambda v: left + (math.log10(v) - math.log10(lo)) / span * (W - left - right)
    out = [f'<svg viewBox="0 0 {W} {H}" width="100%">']
    k = lo
    while k <= hi * 1.0001:
        out.append(f'<line x1="{x(k):.1f}" x2="{x(k):.1f}" y1="{top - 6}" y2="{H - 22}" stroke="var(--grid)"/>')
        out.append(f'<text x="{x(k):.1f}" y="{H - 8}" text-anchor="middle">{k:g}×</text>')
        k *= 10
    out.append(f'<line x1="{x(1):.1f}" x2="{x(1):.1f}" y1="{top - 6}" y2="{H - 22}" stroke="var(--axis)" '
               f'stroke-width="2" stroke-dasharray="4 3"/>')
    out.append(f'<text x="{x(1):.1f}" y="{top - 11}" text-anchor="middle">reference = 1×</text>')
    n = len(rows)
    for i, pid in enumerate(pids):
        y0 = top + i * row_h
        cy = y0 + row_h / 2
        out.append(f'<line x1="{left}" x2="{W - right}" y1="{y0 + row_h:.1f}" y2="{y0 + row_h:.1f}" '
                   f'stroke="var(--grid)" stroke-width="0.5"/>')
        out.append(f'<text class="lbl" x="{left - 10}" y="{cy + 4:.1f}" text-anchor="end">'
                   f'{pid}. {E(clip(A["problems"][pid]["title"], 32))}</text>')
        for j, r in enumerate(rows):
            c = A["cells"][(r["key"], pid)]
            if c["verdict"] == "AC" and c["speed"]:
                dy = (j - (n - 1) / 2) * min(5, (row_h - 12) / max(n - 1, 1))
                out.append(f'<circle cx="{x(c["speed"]):.1f}" cy="{cy + dy:.1f}" r="4.5" '
                           f'fill="var(--s{r["color"]})" stroke="#fff" stroke-width="1.5"/>')
    out.append("</svg>")
    return "\n".join(out)


def highlights(A):
    rows, pids, cells = A["rows"], sorted(A["problems"]), A["cells"]
    out = []
    top = max(r["accepted"] for r in rows)
    best = [r for r in rows if r["accepted"] == top]
    out.append(f"<b>{E(' , '.join(r['name'] for r in best).replace(' , ', ', '))}</b> "
               f"{'accepted' if len(best) > 1 else 'accepted'} the most problems: {top} of {len(pids)}. "
               f"Ties are ranked by the share of tests passed (<b>{E(best[0]['name'])}</b>: "
               f"{best[0]['pass_rate'] * 100:.1f}%).")
    everyone = [p for p in pids if all(cells[(r["key"], p)]["verdict"] == "AC" for r in rows)]
    nobody = [p for p in pids if not any(cells[(r["key"], p)]["verdict"] == "AC" for r in rows)]
    if everyone:
        out.append("Solved by every model: " + ", ".join(f"{p} ({E(A['problems'][p]['title'])})" for p in everyone) + ".")
    if nobody:
        out.append("Solved by no model: " + ", ".join(f"{p} ({E(A['problems'][p]['title'])})" for p in nobody) + ".")
    fails = Counter(cells[(r["key"], p)]["verdict"] for r in rows for p in pids)
    total = sum(fails.values())
    out.append(f"{fails['AC']} of {total} solutions were accepted. Failures: " + ", ".join(
        f"{fails[v]} {VERDICT_TEXT[v].lower()}" for v in ("WA", "RE", "TLE", "MLE", "CE", "NO_CODE") if fails[v]) + ".")
    for r in rows:
        if r["no_code"]:
            miss = [str(p) for p in pids if cells[(r["key"], p)]["verdict"] == "NO_CODE"]
            out.append(f"<b>{E(r['name'])}</b> returned no code for {len(miss)} problems ({', '.join(miss)}): "
                       f"the answer had no C++ code block (a reasoning model that runs out of tokens while "
                       f"thinking never writes the answer).")
    slow = []
    for p in pids:
        sp = [cells[(r["key"], p)]["speed"] for r in rows if cells[(r["key"], p)]["verdict"] == "AC"]
        g = geomean(sp)
        if g and g >= 20:
            slow.append((g, p))
    for g, p in sorted(slow, reverse=True):
        out.append(f"Problem {p} ({E(A['problems'][p]['title'])}): accepted solutions were ~{fmt_x(g)} slower than "
                   f"the reference on heavy tests. Reference: {E(PROBLEM_INFO[p][2])}")
    san = [(r, p) for r in rows for p in pids if cells[(r["key"], p)]["sanitizer"]]
    if san:
        out.append("Memory errors / undefined behavior found by the sanitizers: " + ", ".join(
            f"{E(r['name'])} on {p}" for r, p in san) + ".")
    return out


def build_html(A):
    meta, rows, pids, cells = A["meta"], A["rows"], sorted(A["problems"]), A["cells"]
    tests_total = sum(len(p["tests"]) for p in A["problems"].values())
    cases_total = sum(t["cases"] for p in A["problems"].values() for t in p["tests"])
    series = " ".join(f"--s{i}: {c};" for i, c in enumerate(SERIES))
    h = [f"<!doctype html><html lang='en'><head><meta charset='utf-8'><title>LLM LeetCode Benchmark</title>"
         f"<style>{CSS.replace('%SERIES%', series)}</style></head><body>"]

    # ---------------- Page 1: overview ----------------
    h.append("<h1>LLM LeetCode Benchmark</h1>")
    h.append(f"<p class='sub'>How well do open-source LLMs solve 10 hard LeetCode problems in C++? "
             f"Correctness, speed, memory and code quality of every generated solution. "
             f"Run {E(meta['date'][:16].replace('T', ' '))} · {E(meta['cpu'])} · {E(meta['compiler'])}</p>")
    total = sum(r["problems"] for r in rows)
    h.append("<div class='tiles'>")
    for v, l in [(f"{sum(r['accepted'] for r in rows)}/{total}", "solutions accepted (all models)"),
                 (str(len(rows)), "models · 10 problems each"),
                 (str(tests_total), "test files"), (f"{cases_total:,}", "test cases")]:
        h.append(f"<div class='card tile'><div class='v'>{v}</div><div class='l'>{l}</div></div>")
    h.append("</div><div class='cols3'><div>")
    h.append("<h2>Problems accepted (out of 10)</h2><p class='sub'>Accepted = passes every edge, random and heavy "
             "test of the problem.</p><div class='card bars'>")
    for r in rows:
        w = r["accepted"] / max(r["problems"], 1) * 100
        h.append(f"<div class='row'><div>{sw(r)}{E(r['name'])}</div><div class='track'><div class='bar' "
                 f"style='width:{w:.1f}%;background:var(--s{r['color']})'></div></div>"
                 f"<div class='val'>{r['accepted']}/{r['problems']}</div></div>")
    h.append("</div></div><div><h2>Highlights</h2><ul>")
    h += [f"<li>{x}</li>" for x in highlights(A)]
    h.append("</ul></div></div>")
    h.append("<h3>What was tested</h3><p>Each model got the problem statement and a C++ starter (function signature "
             "only) and had to write the function. Its code is compiled and run on hundreds of generated tests per "
             "problem, from tiny corner cases up to LeetCode's maximum input sizes and beyond. The answers are "
             "compared with a trusted reference solution, the run time is measured, and the code is checked for "
             "memory errors and warnings. Pages 2–6 explain every metric and the test suite; then one page per "
             "problem.</p>")

    # ---------------- Page 2: model summary + glossary ----------------
    h.append("<section class='page'><h2>Model summary</h2><p class='sub'>Ranked by problems accepted, then by "
             "share of tests passed, then by speed.</p>")
    head, trs = model_table(A)
    h.append("<table><thead><tr>" + "".join(
        f"<th class='{'num' if i > 1 else ''}'>{E(x)}</th>" for i, x in enumerate(head)) + "</tr></thead>")
    for r, tr in zip(rows, trs):
        h.append("<tr>" + "".join(f"<td class='{'num' if i > 1 else ''}'>{sw(r) if i == 1 else ''}{E(c)}</td>"
                                  for i, c in enumerate(tr)) + "</tr>")
    h.append("</table><h3>What each column means</h3><dl class='gloss'>")
    gloss = [
        ("Accepted", "Problems where the solution passed <b>every</b> edge, random and heavy test file — the same "
                     "bar as LeetCode's “Accepted”. A single wrong case, crash or timeout fails the problem."),
        ("Tests passed", "Test files passed out of all edge + random + heavy files of the 10 problems (a missing "
                         "or non-compiling solution counts as 0 passed). Finer-grained than Accepted: it shows how "
                         "close a failing solution got."),
        ("Compiles", "Solutions that compile in a LeetCode-like environment: <code>#include &lt;bits/stdc++.h&gt;"
                     "</code> and <code>using namespace std;</code> are added before the code, as LeetCode's judge "
                     "does. Missing code counts as not compiling."),
        ("Standalone", "Solutions whose untouched file compiles on its own, with only the #includes the model "
                       "wrote. Measures whether the model followed the prompt (“include every #include the code "
                       "needs”). Failing this alone does not fail a problem."),
        ("Speed vs ref", "How much slower than the reference solution: for each heavy test, solution time ÷ "
                         "reference time; then the geometric mean over the heavy tests of a problem, and over the "
                         "accepted problems of a model. 1× = as fast, 3× = three times slower, 0.9× = 10% faster. "
                         "Only accepted problems count, so a model with few (easy) accepted problems can look fast."),
        ("Memory vs ref", "Same idea for memory: peak memory the function calls added (input excluded), ÷ the "
                          "reference's. 64 KB is added to both sides so tiny values (a few KB) don't create huge "
                          "ratios."),
        ("Stress passed", "Stress tests passed. They go 10× to 1000× beyond LeetCode's limits (e.g. a million list "
                          "nodes) to show how solutions scale; they <b>don't</b> count for Accepted."),
        ("Sanitizer", "Problems where AddressSanitizer or UndefinedBehaviorSanitizer reported an error (out-of-bounds "
                      "access, use of invalid values, crash...) on the edge and random tests. Such code can pass by "
                      "luck and break on another compiler or machine."),
        ("Warnings", "Compiler warnings (<code>-Wall -Wextra</code>) inside the AI code, e.g. comparing signed and "
                     "unsigned integers."),
        ("Avg LOC", "Average lines of code per solution, without blank lines and comments."),
    ]
    h += [f"<dt>{a}</dt><dd>{b}</dd>" for a, b in gloss]
    h.append("</dl></section>")

    # ---------------- Page 3: verdict matrix ----------------
    h.append("<section class='page'><h2>Verdict per problem</h2><p class='sub'>One cell per (model, problem). "
             "Accepted cells show the speed vs the reference; failing cells show the verdict and how many of the "
             "problem's counted test files passed.</p>")
    h.append("<table><thead><tr><th>Model</th>" + "".join(f"<th class='c'>{p}</th>" for p in pids) + "</tr></thead>")
    for r in rows:
        h.append(f"<tr><td>{sw(r)}{E(r['name'])}</td>" +
                 "".join(f"<td class='c'>{matrix_badge(cells[(r['key'], p)])}</td>" for p in pids) + "</tr>")
    h.append("</table><p class='small' style='margin-top:6px'>" + " · ".join(
        f"{p} = {E(A['problems'][p]['title'])}" for p in pids) + "</p>")
    h.append("<div class='cols'><div><h3>Verdicts</h3><dl class='gloss'>")
    vg = [("AC", "Accepted: every counted test passed."),
          ("WA", "Wrong Answer: the program ran, but at least one case gave a different answer than the reference."),
          ("RE", "Runtime Error: crash (segmentation fault, stack overflow), uncaught exception or abort."),
          ("TLE", f"Time Limit Exceeded: a test file took more than {meta['timeout_s']:g} s (killed). "
                  "Includes infinite loops."),
          ("MLE", f"Memory Limit Exceeded: allocation failed above {meta['memory_limit_mb']} MiB."),
          ("CE", "Compile Error: the code does not compile, even with LeetCode's implicit headers."),
          ("NO_CODE", "No code: the model's answer had no C++ code block to extract.")]
    h += [f"<dt>{badge(v, v if v != 'NO_CODE' else 'No code')}</dt><dd>{d}</dd>" for v, d in vg]
    h.append("</dl></div><div><h3>How the verdict is chosen</h3><p>Like LeetCode, the verdict is the result of the "
             "<b>first failing test</b>, in the order edge → random → heavy (inside a tier, by file name). "
             "So a solution that is wrong on a corner case shows WA even if it would also be too slow on big "
             "inputs. Every test still runs, so “Tests passed” counts all of them.</p>"
             "<p>Stress tests are reported separately (column “Stress passed”) and never change the verdict.</p>"
             "</div></div></section>")

    # ---------------- Page 4: speed ----------------
    h.append("<section class='page'><h2>Runtime vs reference (heavy tests)</h2><p class='sub'>Accepted solutions "
             "only. Each dot: geometric mean over the problem's heavy tests of (solution time ÷ reference time). "
             "Log scale: every grid line is 10× slower; left of the dashed line = faster than the reference.</p>")
    h.append("<div class='legend'>" + "".join(f"<span>{sw(r)}{E(r['name'])}</span>" for r in rows) + "</div>")
    h.append(svg_dotplot(A))
    h.append("<h3>Total time of the solution function on all heavy tests of the problem</h3>"
             "<table><thead><tr><th>Problem</th><th class='num'>Reference</th>" +
             "".join(f"<th class='num'>{sw(r)}{E(r['name'])}</th>" for r in rows) + "</tr></thead>")
    for p in pids:
        rt = sum(t["time_ms"] for t in A["ref"][p]["tests"] if t["tier"] == "heavy")
        h.append(f"<tr><td>{p}. {E(A['problems'][p]['title'])}</td><td class='num'>{fmt_ms(rt)}</td>")
        for r in rows:
            c = cells[(r["key"], p)]
            h.append(f"<td class='num'>{fmt_ms(c['heavy_ms']) if c['heavy_ms'] is not None else '—'}</td>")
        h.append("</tr>")
    h.append("</table><p class='small' style='margin-top:4px'>— = not all heavy tests passed. Times are the median "
             f"of {meta['repeats']} runs, measured inside the program around the function calls only.</p></section>")

    # ---------------- Page 5: how it works ----------------
    tier_counts = Counter(t["tier"] for p in A["problems"].values() for t in p["tests"])
    tier_cases = Counter()
    for p in A["problems"].values():
        for t in p["tests"]:
            tier_cases[t["tier"]] += t["cases"]
    h.append("<section class='page'><h2>How the benchmark works</h2><div class='cols'><div>")
    h.append("<h3>Pipeline</h3><ol>"
             "<li><b>Generation</b> (<code>generation/generate.py</code>): each model receives the LeetCode "
             "statement and a C++ starter with the function signature, and must return only the function in a "
             "<code>```cpp</code> block. Greedy decoding (no sampling), so the same prompt gives the same code.</li>"
             "<li><b>Treatment</b>: the code is copied to <code>treated_generated_code/</code> byte for byte; only "
             "LeetCode's implicit preamble and a <code>main()</code> that reads the input, calls the function, "
             "times it and prints the output are added around it. Wrong code is never fixed.</li>"
             f"<li><b>Build</b>: <code>{E(meta['flags'])}</code>, plus a sanitizer build "
             f"(<code>-fsanitize=address,undefined</code>) and a standalone syntax check of the original file.</li>"
             "<li><b>Expected answers</b>: hand-written reference solutions (efficient, well-known algorithms) "
             "produce the answers; on every edge and random case they are double-checked against simple "
             "brute-force Python oracles. The run stops if they ever disagree.</li>"
             "<li><b>Runs</b>: edge/random tests in parallel; heavy/stress tests one at a time, "
             f"{meta['repeats']} runs each, median time kept; sanitizer build on edge/random tests.</li>"
             "<li><b>Judging</b>: outputs compared case by case (any order for problem 30, ±10⁻⁵ for problem 4; "
             "sudoku boards must equal the unique solution).</li></ol>")
    h.append("<h3>Measurements</h3><ul>"
             "<li><b>Time</b>: <code>steady_clock</code> around each call to the solution function, summed over the "
             "cases of the file. Reading and parsing the input is excluded.</li>"
             "<li><b>Memory</b>: the kernel's peak-RSS counter is reset after the input is loaded and read after the "
             "calls: the memory the solution itself allocated.</li>"
             f"<li><b>Limits</b>: {meta['timeout_s']:g} s per test file ({meta['stress_timeout_s']:g} s for stress), "
             f"{meta['stack_kb'] // 1024} MiB stack (Linux default), {meta['memory_limit_mb']} MiB address space.</li>"
             f"<li><b>Machine</b>: {E(meta['cpu'])}, {meta['cores']} threads, {E(meta['os'])}.</li></ul>")
    h.append("</div><div><h3>Test tiers</h3><table><thead><tr><th>Tier</th><th>What it is</th><th class='num'>Files"
             "</th><th class='num'>Cases</th><th>Counts for Accepted</th></tr></thead>")
    tier_desc = {
        "edge": "One hand-written corner case per file: LeetCode's examples, empty inputs, single elements, "
                "extreme values, known traps.",
        "random": "Hundreds or thousands of small random cases per file, compared with the oracles. Finds logic "
                  "errors the edge cases miss.",
        "heavy": "LeetCode's maximum input sizes, many cases per file, and adversarial inputs (exponential "
                 "backtracking traps, the hardest known sudokus). Correctness + timing.",
        "stress": "10× to 1000× beyond LeetCode's limits. Shows how a solution scales (e.g. recursion that "
                  "overflows the stack, O(n²) that times out). Timing only.",
    }
    for t in ("edge", "random", "heavy", "stress"):
        h.append(f"<tr><td><b>{t}</b></td><td>{tier_desc[t]}</td><td class='num'>{tier_counts[t]}</td>"
                 f"<td class='num'>{tier_cases[t]:,}</td><td>{'yes' if t in COUNTED else 'no'}</td></tr>")
    h.append("</table><p class='small' style='margin-top:4px'>All tests are generated deterministically "
             f"(seed {meta['seed']}), so every run uses exactly the same inputs.</p>")
    h.append("<h3>Code-quality checks</h3><ul>"
             "<li><b>Sanitizers</b>: AddressSanitizer (out-of-bounds, use-after-free, stack overflow) and "
             "UndefinedBehaviorSanitizer (invalid bool values, signed overflow, ...).</li>"
             "<li><b>Standalone compile</b>: the original file, with only its own #includes.</li>"
             "<li><b>Warnings</b>: <code>-Wall -Wextra</code>, counted only inside the AI code.</li>"
             "<li><b>Prints</b>: the prompt forbids printing; output to stdout is flagged.</li>"
             "<li><b>Size</b>: lines of code without blanks and comments.</li></ul>")
    h.append("</div></div></section>")

    # ---------------- Page 6: test suite overview ----------------
    h.append("<section class='page'><h2>The 10 problems and their tests</h2><p class='sub'>All are LeetCode "
             "<b>Hard</b> problems. Constraints are LeetCode's; heavy tests reach them, stress tests exceed them.</p>")
    h.append("<table><thead><tr><th>#</th><th>Problem</th><th>LeetCode constraints</th><th>Reference algorithm</th>"
             "<th class='num'>Edge</th><th class='num'>Random</th><th class='num'>Heavy</th><th class='num'>Stress</th>"
             "<th class='num'>Cases</th></tr></thead>")
    for p in pids:
        tests = A["problems"][p]["tests"]
        cnt = Counter(t["tier"] for t in tests)
        h.append(f"<tr><td>{p}</td><td><b>{E(A['problems'][p]['title'])}</b><br><span class='small'>"
                 f"{E(PROBLEM_INFO[p][0])}</span></td><td class='small'>{E(PROBLEM_INFO[p][1])}</td>"
                 f"<td class='small'>{E(PROBLEM_INFO[p][2])}</td>" +
                 "".join(f"<td class='num'>{cnt[t]}</td>" for t in ("edge", "random", "heavy", "stress")) +
                 f"<td class='num'>{sum(t['cases'] for t in tests):,}</td></tr>")
    h.append("</table></section>")

    # ---------------- Per problem ----------------
    for p in pids:
        prob = A["problems"][p]
        ref_by = {t["test"]: t for t in A["ref"][p]["tests"]}
        n_ac = sum(cells[(r["key"], p)]["verdict"] == "AC" for r in rows)
        h.append(f"<section class='page'><h2>{p}. {E(prob['title'])} "
                 f"<span class='small'>· {n_ac}/{len(rows)} models accepted</span></h2>")
        h.append(f"<p class='sub'>{E(PROBLEM_INFO[p][0])}<br><span class='pill'>constraints</span>"
                 f"{E(PROBLEM_INFO[p][1])}<br><span class='pill'>reference</span>{E(PROBLEM_INFO[p][2])}</p>")
        h.append("<div class='cols'><div><h3>Results</h3><table><thead><tr><th>Model</th><th>Verdict</th>"
                 "<th class='num'>Tests</th><th class='num'>Stress</th><th class='num'>Speed</th>"
                 "<th class='num'>LOC</th></tr></thead>")
        for r in rows:
            c = cells[(r["key"], p)]
            h.append(f"<tr><td>{sw(r)}{E(r['name'])}</td><td>{badge(c['verdict'])}</td>"
                     f"<td class='num'>{c['passed']}/{c['total']}</td><td class='num'>{c['stress_ok']}/"
                     f"{c['stress_total']}</td><td class='num'>{fmt_x(c['speed']) if c['verdict'] == 'AC' else '—'}"
                     f"</td><td class='num'>{c['loc'] or '—'}</td></tr>")
        h.append("</table></div><div><h3>Issues found</h3>")
        notes = []
        for r in rows:
            c = cells[(r["key"], p)]
            items = []
            if c["verdict"] == "NO_CODE":
                items.append("no code generated")
            items += [f"compile error: <code>{E(clip(e))}</code>" for e in c["compile_errors"][:1]]
            if c["verdict"] not in ("NO_CODE", "CE") and c["strict"] is False and c["strict_errors"]:
                items.append(f"not standalone: <code>{E(clip(c['strict_errors'][0]))}</code>")
            if c["failing"]:
                f = c["failing"]
                d = next((t["desc"] for t in prob["tests"] if t["name"] == f["test"]), "")
                items.append(f"first failure <b>{E(f['test'])}</b> ({E(d)}): {E(VERDICT_TEXT[f['status']])}"
                             + (f" — {E(f['detail'])}" if f.get("detail") else ""))
            items += [f"sanitizer ({E(s['test'])}): <code>{E(clip(s['issue']))}</code>" for s in c["sanitizer"][:1]]
            items += [f"warning: <code>{E(clip(w))}</code>" for w in c["warnings"][:1]]
            if c["printed"]:
                items.append("prints to stdout (the prompt asked not to)")
            if items:
                notes.append(f"<li><b>{E(r['name'])}</b><ul>" + "".join(f"<li>{i}</li>" for i in items) + "</ul></li>")
        h.append("<ul class='issues'>" + "".join(notes) + "</ul>" if notes else "<p class='small'>None.</p>")
        h.append("</div></div>")
        h.append("<h3>Tests</h3><table><thead><tr><th>Test</th><th style='width:34%'>Description</th><th class='num'>Cases</th>"
                 "<th class='num'>Reference</th>" +
                 "".join(f"<th class='num'>{sw(r)}{E(r['name'])}</th>" for r in rows) + "</tr></thead>")
        # Edge and random tiers: one summary row each (they are not timed)
        for tier in ("edge", "random"):
            tts = [t for t in prob["tests"] if t["tier"] == tier]
            desc = ("one corner case per file: " + "; ".join(t["desc"] for t in tts)) if tier == "edge" \
                else f"{len(tts)} files of small random cases ({tts[0]['desc']} each)"
            h.append(f"<tr><td><code>{tier}_*</code></td><td class='small'>{E(desc)}</td>"
                     f"<td class='num'>{sum(t['cases'] for t in tts):,}</td><td class='num'>{len(tts)}/{len(tts)}</td>")
            for r in rows:
                res = {x["test"]: x for x in cells[(r["key"], p)]["sol"].get("tests") or []}
                if not res:
                    h.append("<td class='num'>—</td>")
                    continue
                st = Counter(res[t["name"]]["status"] for t in tts if t["name"] in res)
                ok = st.pop("AC", 0)
                if not st:
                    h.append(f"<td class='num'>{ok}/{len(tts)} ✓</td>")
                else:
                    worst = max(st, key=st.get)
                    h.append(f"<td class='num'>{ok}/{len(tts)} " + badge(worst, " ".join(
                        f"{k}×{v}" for k, v in st.items())) + "</td>")
            h.append("</tr>")
        for t in (t for t in prob["tests"] if t["tier"] in ("heavy", "stress")):
            rt = ref_by.get(t["name"], {})
            h.append(f"<tr><td><code>{E(t['name'])}</code></td><td>{E(t['desc'])}</td>"
                     f"<td class='num'>{t['cases']:,}</td><td class='num'>{fmt_ms(rt.get('time_ms'))}</td>")
            for r in rows:
                c = cells[(r["key"], p)]
                res = next((x for x in c["sol"].get("tests") or [] if x["test"] == t["name"]), None)
                if not res:
                    h.append("<td class='num'>—</td>")
                elif res["status"] == "AC":
                    h.append(f"<td class='num'>{fmt_ms(res.get('time_ms'))}</td>")
                else:
                    h.append(f"<td class='num'>{badge(res['status'], res['status'])}</td>")
            h.append("</tr>")
        h.append("</table><p class='small' style='margin-top:4px'>Edge/random rows: files passed (and the failure "
                 "kinds). Heavy/stress rows: time of the solution function (median of the runs) or the verdict of that "
                 "file. — = no compiled code. Full per-file data: <code>benchmark/results/tests.csv</code>.</p></section>")

    # ---------------- Reproduce ----------------
    h.append("<section class='page'><h2>Reproducing</h2><p>From the repository root:</p><ul>"
             "<li><code>.venv/bin/python generation/generate.py</code> — generates the code (needs a GPU and the "
             "Hugging Face models; already done, results in <code>generated_code/</code>).</li>"
             "<li><code>.venv/bin/python benchmark/run_benchmark.py</code> — tests, builds, runs and writes this "
             f"report (≈{meta['duration_s'] / 60:.0f} min on the machine above).</li>"
             "<li><code>.venv/bin/python benchmark/make_report.py</code> — rebuilds the report from "
             "<code>benchmark/results/results.json</code> without re-running.</li></ul>"
             "<p>Raw data: <code>benchmark/results/results.json</code> (everything), <code>tests.csv</code> (one row "
             "per test run), <code>summary.md</code> (the tables above in Markdown).</p>"
             "<p class='foot'>Timings depend on the machine; ratios vs the reference are more portable than "
             "absolute times.</p></section>")
    h.append("</body></html>")
    return "\n".join(h)


def find_chrome():
    for c in [os.environ.get("CHROME"), "google-chrome", "google-chrome-stable", "chromium", "chromium-browser",
              "chrome"]:
        if c and shutil.which(c):
            return shutil.which(c)
    return None


def write_pdf(html_text, pdf_path):
    chrome = find_chrome()
    if not chrome:
        out = pdf_path.with_suffix(".html")
        out.write_text(html_text, encoding="utf-8")
        print(f"Chrome/Chromium not found (set CHROME=...): saved {out} instead of the PDF")
        return None
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / "report.html"
        src.write_text(html_text, encoding="utf-8")
        subprocess.run([chrome, "--headless=new", "--disable-gpu", "--no-sandbox", "--no-pdf-header-footer",
                        f"--user-data-dir={tmp}/profile", f"--print-to-pdf={pdf_path}", src.as_uri()],
                       check=True, capture_output=True, timeout=180)
    return pdf_path


def build(data):
    A = analyze(data)
    RESULTS.mkdir(exist_ok=True)
    pdf = write_pdf(build_html(A), RESULTS / "report.pdf")
    write_csv(A, RESULTS / "tests.csv")
    mh, mr = model_table(A)
    xh, xr = matrix_table(A)
    (RESULTS / "summary.md").write_text(
        f"# Benchmark summary\n\nRun {data['meta']['date']} on {data['meta']['cpu']} ({data['meta']['compiler']}). "
        "Metrics are explained in [report.pdf](report.pdf) and [../README.md](../README.md).\n\n"
        "## Models\n\n" + md_table(mh, mr) + "\n\n## Verdict per problem\n\n"
        "AC = accepted (speed vs the reference), WA = wrong answer, RE = runtime error, TLE = time limit, "
        "CE = compile error, No code = no code generated; x/y = test files passed.\n\n" + md_table(xh, xr) + "\n")
    print("\n" + console_table(mh, mr) + "\n\n" + console_table(xh, xr) + "\n")
    print(f"Report: {pdf or RESULTS / 'report.html'}\nTables: {RESULTS / 'summary.md'}, {RESULTS / 'tests.csv'}")


if __name__ == "__main__":
    build(json.loads((RESULTS / "results.json").read_text()))
