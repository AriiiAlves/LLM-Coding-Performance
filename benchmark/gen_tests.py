"""
Generates the test inputs for the 10 LeetCode problems.

Every test file has the same layout: the first token is T (number of cases),
then T cases in a problem-specific format (see harness/p<id>.cpp). Strings are
written as "<length> <chars>" (length 0 has no chars), so empty strings are safe.

Tiers (a file's name starts with its tier):
  edge    one hand-written corner case per file
  random  many small random cases per file (correctness)
  heavy   inputs at the LeetCode max constraints (correctness + timing)
  stress  inputs BEYOND the LeetCode constraints (scalability only,
          they don't count for "Accepted")

Everything is deterministic (fixed seed), so the same tests are generated on
every machine. The expected answers are produced later by the reference
solutions (run_benchmark.py), and cross-checked with the oracles in oracle.py.
"""
import json
import random
import sys
from pathlib import Path

BENCH = Path(__file__).resolve().parent
TESTS = BENCH / "tests"
SEED = 20261104
# Bump when the generated tests change, so old tests/ folders are regenerated
VERSION = 4

INT_MIN, INT_MAX = -(2**31), 2**31 - 1


# --------------------------------------------------------------------------
# Writers
# --------------------------------------------------------------------------
class Case:
    """Tokens of one test case."""

    def __init__(self):
        self.parts = []

    def int(self, v):
        self.parts.append(str(v))
        return self

    def ints(self, vs):
        # Length-prefixed int list
        self.parts.append(str(len(vs)))
        if vs:
            self.parts.append(" ".join(map(str, vs)))
        return self

    def str(self, s):
        self.parts.append(f"{len(s)} {s}" if s else "0")
        return self

    def text(self):
        return " ".join(self.parts)


class Writer:
    def __init__(self):
        self.manifest = {}

    def write(self, pid, name, cases, desc):
        folder = TESTS / f"p{pid}"
        folder.mkdir(parents=True, exist_ok=True)
        with open(folder / f"{name}.in", "w") as f:
            f.write(f"{len(cases)}\n")
            for c in cases:
                f.write(c.text())
                f.write("\n")
        tier = name.split("_")[0]
        self.manifest.setdefault(str(pid), []).append(
            {"name": name, "tier": tier, "cases": len(cases), "desc": desc}
        )

    def edges(self, pid, cases_with_desc):
        for i, (case, desc) in enumerate(cases_with_desc, 1):
            self.write(pid, f"edge_{i:02d}", [case], desc)


def C():
    return Case()


# --------------------------------------------------------------------------
# 4 - Median of Two Sorted Arrays   (0 <= m, n <= 1000, 1 <= m + n, |v| <= 1e6)
# --------------------------------------------------------------------------
def gen_4(w, rng):
    def case(a, b):
        return C().ints(sorted(a)).ints(sorted(b))

    E = 10**6
    w.edges(4, [
        (case([1, 3], [2]), "example 1"),
        (case([1, 2], [3, 4]), "example 2"),
        (case([1], []), "second array empty"),
        (case([], [1]), "first array empty"),
        (case([], [2, 3]), "first empty, even length"),
        (case([5], [5]), "equal single elements"),
        (case([1, 1, 1], [1, 1]), "all duplicates"),
        (case([1, 2, 3, 4, 5], []), "only first array, odd"),
        (case([1, 2, 3, 4, 5, 6], [7]), "first array longer"),
        (case([3], [1, 2, 4, 5]), "single element in the middle"),
        (case([2, 3, 4, 5, 6], [1]), "single element smaller than all"),
        (case([-E, -E], [-E]), "min values"),
        (case([E - 1, E], [E]), "max values"),
        (case([-E], [E]), "extreme values, even"),
        (case([1, 2, 3], [10, 11, 12]), "disjoint ranges"),
        (case([10, 11, 12], [1, 2, 3]), "disjoint ranges, reversed"),
        (case([-5, -3, -1], [-4, -2]), "negatives interleaved"),
    ])

    for f in range(1, 6):
        cases = []
        for _ in range(300):
            m, n = rng.randint(0, 40), rng.randint(0, 40)
            if m + n == 0:
                m = 1
            span = rng.choice([3, 20, 2 * E])
            lo = -min(span, E)
            hi = min(span, E)
            cases.append(case([rng.randint(lo, hi) for _ in range(m)],
                              [rng.randint(lo, hi) for _ in range(n)]))
        w.write(4, f"random_{f:02d}", cases, "300 random cases, sizes 0..40")

    def rnd(n):
        return [rng.randint(-E, E) for _ in range(n)]

    w.write(4, "heavy_01", [case(rnd(1000), rnd(1000)) for _ in range(2000)],
            "2000 cases, m = n = 1000, random")
    w.write(4, "heavy_02",
            [case([rng.randint(-E, 0) for _ in range(1000)], [rng.randint(1, E) for _ in range(1000)])
             for _ in range(2000)],
            "2000 cases, m = n = 1000, disjoint ranges")
    w.write(4, "heavy_03",
            [case(rnd(rng.choice([0, 1, 999, 1000])), rnd(rng.randint(1, 1000))) for _ in range(2000)],
            "2000 cases, unbalanced sizes")
    w.write(4, "stress_01", [case(rnd(10**6), rnd(10**6)) for _ in range(2)],
            "m = n = 1e6 (1000x the constraint)")


# --------------------------------------------------------------------------
# 10 - Regular Expression Matching   (1 <= |s|, |p| <= 20)
# --------------------------------------------------------------------------
def random_regex(rng, alphabet, max_len):
    """Valid pattern: every '*' follows a letter or '.'."""
    p = ""
    while True:
        tok = rng.choice(alphabet + ".") if rng.random() < 0.85 else "."
        if rng.random() < 0.4:
            tok += "*"
        if len(p) + len(tok) > max_len:
            break
        p += tok
        if rng.random() < 1 / max_len * 2:
            break
    return p or rng.choice(alphabet)


def string_from_regex(rng, p, alphabet, max_len):
    """Builds a string that matches p (when it fits in max_len)."""
    out = ""
    i = 0
    while i < len(p):
        ch = p[i]
        star = i + 1 < len(p) and p[i + 1] == "*"
        reps = rng.randint(0, 3) if star else 1
        for _ in range(reps):
            out += rng.choice(alphabet) if ch == "." else ch
        i += 2 if star else 1
    return out[:max_len] or rng.choice(alphabet)


def gen_10(w, rng):
    def case(s, p):
        return C().str(s).str(p)

    w.edges(10, [
        (case("aa", "a"), "example 1"),
        (case("aa", "a*"), "example 2"),
        (case("ab", ".*"), "example 3"),
        (case("aab", "c*a*b"), "zero repetitions"),
        (case("mississippi", "mis*is*p*."), "classic false"),
        (case("mississippi", "mis*is*ip*."), "classic true"),
        (case("a", "ab*"), "trailing star zero"),
        (case("ab", ".*c"), "dot-star then missing char"),
        (case("aaa", "a*a"), "star then literal"),
        (case("aaa", "ab*a*c*a"), "many optional groups"),
        (case("a", ".*..a*"), "pattern needs more chars"),
        (case("bbbba", ".*a*a"), "backtrack into dot-star"),
        (case("ab", ".*.."), "dot-star then dots"),
        (case("a", "."), "single dot"),
        (case("abcdefghijklmnopqrst", "...................."), "20 dots"),
        (case("aaaaaaaaaaaaaaaaaaab", "a*a*a*a*a*a*a*a*a*c"), "exponential backtracking trap (false)"),
        (case("aaaaaaaaaaaaaaaaaaaa", "a*a*a*a*a*a*a*a*a*a"), "many stars (true)"),
        (case("abcabcabcabcabcabcab", ".*.*.*.*.*.*.*.*.*.*"), "all dot-stars"),
    ])

    def rand_case(max_len, alphabet="ab"):
        p = random_regex(rng, alphabet, max_len)
        if rng.random() < 0.5:
            s = string_from_regex(rng, p, alphabet, max_len)
        else:
            s = "".join(rng.choice(alphabet) for _ in range(rng.randint(1, max_len)))
        return case(s, p)

    for f in range(1, 6):
        alphabet = "ab" if f <= 3 else "abc"
        w.write(10, f"random_{f:02d}", [rand_case(rng.randint(2, 20), alphabet) for _ in range(2000)],
                f"2000 random cases, alphabet '{alphabet}', lengths <= 20")

    w.write(10, "heavy_01", [rand_case(20, "ab") for _ in range(200_000)],
            "200k random cases at max length 20")

    def trap():
        k = rng.randint(5, 9)
        n = rng.randint(14, 19)
        s = "a" * n + rng.choice(["b", "a"])
        p = "a*" * k + rng.choice(["c", "b", "a"])
        return case(s, p[:20])

    w.write(10, "heavy_02", [trap() for _ in range(20_000)],
            "20k backtracking traps like s=a..ab, p=a*a*..a*c")

    # Beyond constraints: long strings
    def long_case(n):
        p = random_regex(rng, "ab", n)
        s = string_from_regex(rng, p, "ab", n) if rng.random() < 0.5 else \
            "".join(rng.choice("ab") for _ in range(n))
        return case(s, p)

    w.write(10, "stress_01", [long_case(1000) for _ in range(30)],
            "30 cases, |s|, |p| ~ 1000 (50x the constraint)")
    w.write(10, "stress_02", [case("a" * 1000 + "b", ("a*" * 300) + "c")],
            "trap with |s| = 1001, 300 stars")


# --------------------------------------------------------------------------
# 23 - Merge k Sorted Lists   (k <= 1e4, list len <= 500, total <= 1e4, |v| <= 1e4)
# --------------------------------------------------------------------------
def gen_23(w, rng):
    def case(lists):
        c = C().int(len(lists))
        for lst in lists:
            c.ints(sorted(lst))
        return c

    w.edges(23, [
        (case([[1, 4, 5], [1, 3, 4], [2, 6]]), "example 1"),
        (case([]), "k = 0"),
        (case([[]]), "one empty list"),
        (case([[], []]), "two empty lists"),
        (case([[1]]), "one single-node list"),
        (case([[], [1]]), "empty then non-empty"),
        (case([[2], [1]]), "two single nodes, reversed"),
        (case([[5, 5, 5], [5, 5], [5]]), "all equal values"),
        (case([[-10000, 0, 10000], [-10000, 10000]]), "extreme values"),
        (case([list(range(500))] + [[]] * 50), "one long list among empties"),
        (case([[i] for i in range(100, 0, -1)]), "100 single-node lists, descending heads"),
    ])

    def rnd_lists(k, max_len, vr=10**4):
        return [[rng.randint(-vr, vr) for _ in range(rng.randint(0, max_len))] for _ in range(k)]

    for f in range(1, 6):
        w.write(23, f"random_{f:02d}",
                [case(rnd_lists(rng.randint(0, 20), 20, rng.choice([5, 10**4]))) for _ in range(300)],
                "300 random cases, k <= 20, lengths <= 20")

    w.write(23, "heavy_01", [case([[rng.randint(-10**4, 10**4)] for _ in range(10**4)]) for _ in range(30)],
            "30 cases, k = 10000 lists of 1 node")
    w.write(23, "heavy_02", [case(rnd_lists(20, 500)) for _ in range(30)],
            "30 cases, k = 20 lists of up to 500 nodes")
    w.write(23, "heavy_03", [case([[rng.randint(-10**4, 10**4) for _ in range(100)] for _ in range(100)])
                             for _ in range(30)],
            "30 cases, k = 100 lists of 100 nodes")
    w.write(23, "heavy_04", [case([[7] * 500 for _ in range(20)]) for _ in range(30)],
            "30 cases, 20 lists of 500 equal values")
    w.write(23, "stress_01", [case([[rng.randint(-10**4, 10**4) for _ in range(100)] for _ in range(10**4)])],
            "k = 10000 lists of 100 nodes (1e6 nodes, 100x the constraint)")
    w.write(23, "stress_02", [case([[rng.randint(-10**4, 10**4) for _ in range(5)] for _ in range(10**5)])],
            "k = 100000 lists of 5 nodes")


# --------------------------------------------------------------------------
# 25 - Reverse Nodes in k-Group   (1 <= k <= n <= 5000, 0 <= v <= 1000)
# --------------------------------------------------------------------------
def gen_25(w, rng):
    def case(vals, k):
        return C().ints(vals).int(k)

    w.edges(25, [
        (case([1, 2, 3, 4, 5], 2), "example 1"),
        (case([1, 2, 3, 4, 5], 3), "example 2"),
        (case([1, 2, 3, 4, 5], 1), "k = 1"),
        (case([1, 2, 3, 4, 5], 5), "k = n"),
        (case([1, 2, 3, 4, 5], 4), "k = n - 1"),
        (case([1], 1), "single node"),
        (case([1, 2], 2), "two nodes, k = 2"),
        (case([1, 2, 3], 2), "odd length, k = 2"),
        (case([1, 2, 3, 4, 5, 6], 3), "exact multiple of k"),
        (case([0, 0, 1000, 1000], 2), "boundary values"),
    ])

    for f in range(1, 6):
        cases = []
        for _ in range(300):
            n = rng.randint(1, 50)
            cases.append(case([rng.randint(0, 1000) for _ in range(n)], rng.randint(1, n)))
        w.write(25, f"random_{f:02d}", cases, "300 random cases, n <= 50")

    ks = [1, 2, 3, 7, 50, 2500, 4999, 5000]
    w.write(25, "heavy_01", [case([rng.randint(0, 1000) for _ in range(5000)], ks[i % len(ks)])
                             for i in range(200)],
            "200 cases, n = 5000, k in {1,2,3,7,50,2500,4999,5000}")
    w.write(25, "heavy_02", [case([rng.randint(0, 1000) for _ in range(5000)], 2) for _ in range(200)],
            "200 cases, n = 5000, k = 2")
    w.write(25, "stress_01", [case([rng.randint(0, 1000) for _ in range(10**6)], 2)],
            "n = 1e6, k = 2 (200x the constraint)")
    w.write(25, "stress_02", [case([rng.randint(0, 1000) for _ in range(10**6)], 1000)],
            "n = 1e6, k = 1000")


# --------------------------------------------------------------------------
# 30 - Substring with Concatenation of All Words
#      (|s| <= 1e4, 1 <= #words <= 5000, 1 <= |word| <= 30, lowercase)
# --------------------------------------------------------------------------
def gen_30(w, rng):
    def case(s, words):
        c = C().str(s).int(len(words))
        for x in words:
            c.str(x)
        return c

    w.edges(30, [
        (case("barfoothefoobarman", ["foo", "bar"]), "example 1"),
        (case("wordgoodgoodgoodbestword", ["word", "good", "best", "word"]), "example 2"),
        (case("barfoofoobarthefoobarman", ["bar", "foo", "the"]), "example 3"),
        (case("wordgoodgoodgoodbestword", ["word", "good", "best", "good"]), "duplicate words"),
        (case("aaaaaa", ["aa", "aa"]), "overlapping matches"),
        (case("a", ["a"]), "single char match"),
        (case("a", ["b"]), "single char no match"),
        (case("ab", ["abc"]), "word longer than s"),
        (case("aaa", ["a", "a", "a", "a"]), "words longer than s in total"),
        (case("abababab", ["ab", "ba"]), "non-aligned offsets"),
        (case("lingmindraboofooowingdingbarrwingmonkeypoundcake", ["fooo", "barr", "wing", "ding", "wing"]),
         "classic tricky case"),
        (case("abc", ["abc"]), "word equals s"),
    ])

    def rnd_case(max_s):
        wl = rng.randint(1, 3)
        k = rng.randint(1, 5)
        alphabet = rng.choice(["ab", "abc"])
        s = "".join(rng.choice(alphabet) for _ in range(rng.randint(1, max_s)))
        words = []
        for _ in range(k):
            if len(s) >= wl and rng.random() < 0.7:
                i = rng.randint(0, len(s) - wl)
                words.append(s[i:i + wl])
            else:
                words.append("".join(rng.choice(alphabet) for _ in range(wl)))
        return case(s, words)

    for f in range(1, 6):
        w.write(30, f"random_{f:02d}", [rnd_case(40) for _ in range(300)], "300 random cases, |s| <= 40")

    w.write(30, "heavy_01", [case("a" * 10**4, ["a"] * 5000)],
            "s = a^10000, 5000 words 'a' (5001 matches)")
    w.write(30, "heavy_02", [case("a" * 10**4, ["a" * 30] * 333) for _ in range(3)],
            "3 cases, s = a^10000, 333 words of length 30")

    def rnd_heavy(alphabet, wl, k):
        s = "".join(rng.choice(alphabet) for _ in range(10**4))
        words = []
        for _ in range(k):
            i = rng.randint(0, len(s) - wl)
            words.append(s[i:i + wl])
        return case(s, words)

    w.write(30, "heavy_03", [rnd_heavy("ab", 2, 500) for _ in range(5)],
            "5 cases, random s over 'ab', 500 words of length 2")
    w.write(30, "heavy_04", [case("ab" * 5000, ["ab", "ba"] * 1250) for _ in range(5)],
            "5 cases, s = (ab)^5000, 2500 words")
    w.write(30, "heavy_05", [rnd_heavy("abcdefghijklmnopqrstuvwxyz", 5, 2000) for _ in range(5)],
            "5 cases, random lowercase s, 2000 words of length 5")
    w.write(30, "stress_01", [case("a" * 10**5, ["a"] * 50_000)],
            "s = a^100000, 50000 words 'a' (10x the constraints)")


# --------------------------------------------------------------------------
# 32 - Longest Valid Parentheses   (0 <= |s| <= 3e4)
# --------------------------------------------------------------------------
def gen_32(w, rng):
    def case(s):
        return C().str(s)

    w.edges(32, [
        (case("(()"), "example 1"),
        (case(")()())"), "example 2"),
        (case(""), "empty string"),
        (case("("), "single open"),
        (case(")"), "single close"),
        (case("()"), "single pair"),
        (case("()(()"), "valid then unclosed"),
        (case("(()())"), "nested"),
        (case("))))(((("), "no valid pair"),
        (case("()(())"), "concatenated"),
        (case("(()(((()"), "mostly open"),
        (case(")(((((()())()()))()(()))("), "long valid in the middle"),
    ])

    def rnd(n, p_open=0.5):
        return "".join("(" if rng.random() < p_open else ")" for _ in range(n))

    for f in range(1, 6):
        w.write(32, f"random_{f:02d}", [case(rnd(rng.randint(0, 60), rng.choice([0.4, 0.5, 0.6])))
                                        for _ in range(500)], "500 random cases, |s| <= 60")

    N = 3 * 10**4
    w.write(32, "heavy_01", [case(rnd(N)) for _ in range(100)], "100 cases, |s| = 30000, random")
    w.write(32, "heavy_02", [case("()" * (N // 2)), case("(" * (N // 2) + ")" * (N // 2)),
                             case(")" * N), case("(" * N), case(")" + "()" * (N // 2 - 1) + "(")],
            "5 structured cases at |s| = 30000")
    w.write(32, "stress_01", [case(rnd(10**7, 0.5)), case("(" * (5 * 10**6) + ")" * (5 * 10**6))],
            "|s| = 1e7 (333x the constraint)")


# --------------------------------------------------------------------------
# 37 - Sudoku Solver (unique solution guaranteed)
# --------------------------------------------------------------------------
ALL = (1 << 9) - 1


def sudoku_parse(s):
    return [0 if ch == "." else int(ch) for ch in s]


def sudoku_count(cells, limit=2, rng=None):
    """Counts solutions (up to limit). Bitmask + minimum-remaining-values.
    Returns (count, one_solution)."""
    cells = list(cells)
    rows, cols, boxes = [0] * 9, [0] * 9, [0] * 9
    for i, v in enumerate(cells):
        if v:
            b = 1 << (v - 1)
            r, c = divmod(i, 9)
            k = (r // 3) * 3 + c // 3
            if rows[r] & b or cols[c] & b or boxes[k] & b:
                return 0, None
            rows[r] |= b
            cols[c] |= b
            boxes[k] |= b
    empties = [i for i, v in enumerate(cells) if not v]
    found = [0, None]

    def rec():
        best, best_mask, best_n = -1, 0, 10
        for i in empties:
            if cells[i]:
                continue
            r, c = divmod(i, 9)
            mask = ALL & ~(rows[r] | cols[c] | boxes[(r // 3) * 3 + c // 3])
            n = bin(mask).count("1")
            if n < best_n:
                best, best_mask, best_n = i, mask, n
                if n <= 1:
                    break
        if best < 0:
            found[0] += 1
            if found[1] is None:
                found[1] = list(cells)
            return found[0] >= limit
        if best_n == 0:
            return False
        r, c = divmod(best, 9)
        k = (r // 3) * 3 + c // 3
        digits = [d for d in range(9) if best_mask >> d & 1]
        if rng:
            rng.shuffle(digits)
        for d in digits:
            b = 1 << d
            cells[best] = d + 1
            rows[r] |= b
            cols[c] |= b
            boxes[k] |= b
            if rec():
                return True
            rows[r] ^= b
            cols[c] ^= b
            boxes[k] ^= b
            cells[best] = 0
        return False

    rec()
    return found[0], found[1]


def sudoku_make(rng, min_clues):
    """Random unique-solution puzzle: random full grid, then remove cells
    while the solution stays unique, until min_clues is reached."""
    _, full = sudoku_count([0] * 81, limit=1, rng=rng)
    cells = list(full)
    order = list(range(81))
    rng.shuffle(order)
    clues = 81
    for i in order:
        if clues <= min_clues:
            break
        keep = cells[i]
        cells[i] = 0
        if sudoku_count(cells)[0] != 1:
            cells[i] = keep
        else:
            clues -= 1
    return "".join(str(v) if v else "." for v in cells)


# Well-known hard puzzles; kept only if valid with a unique solution
FAMOUS_SUDOKUS = [
    ("8..........36......7..9.2...5...7.......457.....1...3...1....68..85...1..9....4..", "Arto Inkala 2012"),
    ("..............3.85..1.2.......5.7.....4...1...9.......5......73..2.1........4...9",
     "anti brute force (Norvig)"),
    (".......39.....1..5..3.5.8....8.9...6.7...2..1..4.......9.8..5..2....6..4..7.....", "Golden Nugget"),
    ("1.......2.9.4...5...6...7...5.9.3.......7.......85..4.7.....6...3...9.8...2.....1", "Easter Monster"),
    ("4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......", "Norvig hard 1"),
    ("52...6.........7.13...........4..8..6......5...........418.........3..2...87.....", "Norvig hard 2"),
    ("6.....8.3.4.7.................5.4.7.3..2.....1.6.......2.....5.....8.6......1....", "Norvig hard 3"),
    ("48.3............71.2.......7.5....6....2..8.............1.76...3.....4......5....", "Norvig hard 4"),
]


def gen_37(w, rng):
    def _raw(s):
        c = C()
        c.parts.append(s)
        return c

    def unique(s):
        n, _ = sudoku_count(sudoku_parse(s))
        return n == 1

    example = "53..7....6..195....98....5.8...6...34..8.3..17...2...6.6....28....419..5....8..79"
    solved = sudoku_count(sudoku_parse(example), limit=1)[1]
    solved_str = "".join(map(str, solved))
    one_empty = "." + solved_str[1:]
    last_empty = solved_str[:80] + "."
    row_empty = "." * 9 + solved_str[9:]
    edges = [(example, "LeetCode example"), (one_empty, "only the first cell empty"),
             (last_empty, "only the last cell empty"), (row_empty, "first row empty"),
             (solved_str[:40] + "." * 9 + solved_str[49:], "middle row empty")]
    for s, _ in edges:
        assert unique(s), s
    w.edges(37, [(_raw(s), d) for s, d in edges])

    for f in range(1, 6):
        w.write(37, f"random_{f:02d}", [_raw(sudoku_make(rng, rng.randint(28, 40))) for _ in range(40)],
                "40 random unique puzzles with 28..40 clues")

    famous = [(s, d) for s, d in FAMOUS_SUDOKUS if unique(s)]
    dropped = [d for s, d in FAMOUS_SUDOKUS if not unique(s)]
    if dropped:
        print(f"  (dropped non-unique famous sudokus: {', '.join(dropped)})")
    w.write(37, "heavy_01", [_raw(sudoku_make(rng, 17)) for _ in range(15)],
            "15 minimal random puzzles (no clue can be removed)")
    anti = [s for s, d in famous if d.startswith("anti brute force")]
    w.write(37, "heavy_02", [_raw(s) for s, d in famous if not d.startswith("anti brute force")],
            "known hardest puzzles: " + ", ".join(d for s, d in famous if not d.startswith("anti")))
    if anti:
        w.write(37, "heavy_03", [_raw(anti[0])],
                "puzzle built to defeat row-major brute force (first row is 987654321)")


# --------------------------------------------------------------------------
# 41 - First Missing Positive   (1 <= n <= 1e5, int32 values)
# --------------------------------------------------------------------------
def gen_41(w, rng):
    def case(v):
        return C().ints(v)

    perm = list(range(1, 101))
    rng.shuffle(perm)
    w.edges(41, [
        (case([1, 2, 0]), "example 1"),
        (case([3, 4, -1, 1]), "example 2"),
        (case([7, 8, 9, 11, 12]), "example 3"),
        (case([1]), "single 1"),
        (case([2]), "single 2"),
        (case([-1]), "single negative"),
        (case([0]), "single zero"),
        (case([1, 1]), "duplicate ones"),
        (case([2, 2, 2, 2]), "duplicates, answer 1"),
        (case([INT_MIN, INT_MAX]), "int32 extremes"),
        (case([INT_MAX]), "only INT_MAX"),
        (case(perm), "shuffled permutation 1..100 (answer n+1)"),
        (case(list(range(100, 0, -1))), "reversed permutation"),
        (case([1, 1000, 2, -5, 3]), "gap after 3"),
    ])

    for f in range(1, 6):
        cases = []
        for _ in range(300):
            n = rng.randint(1, 50)
            if rng.random() < 0.3:
                v = list(range(1, n + 1))
                rng.shuffle(v)
                if rng.random() < 0.5:
                    v[rng.randrange(n)] = rng.randint(-5, 2 * n)
            else:
                v = [rng.randint(-5, n + 5) for _ in range(n)]
            cases.append(case(v))
        w.write(41, f"random_{f:02d}", cases, "300 random cases, n <= 50")

    N = 10**5

    def shuffled_perm(n):
        v = list(range(1, n + 1))
        rng.shuffle(v)
        return v

    def missing_one(n):
        v = shuffled_perm(n)
        v[rng.randrange(n)] = rng.choice([-7, 0, INT_MAX])
        return v

    w.write(41, "heavy_01", [case(shuffled_perm(N)) for _ in range(20)], "20 cases, shuffled permutation of 1..1e5")
    w.write(41, "heavy_02", [case(missing_one(N)) for _ in range(20)], "20 cases, permutation with one value removed")
    w.write(41, "heavy_03", [case([rng.randint(INT_MIN, INT_MAX) for _ in range(N)]) for _ in range(20)],
            "20 cases, n = 1e5, full int32 range")
    w.write(41, "heavy_04", [case([rng.randint(1, 10) for _ in range(N)]) for _ in range(20)],
            "20 cases, n = 1e5, heavy duplicates")
    w.write(41, "stress_01", [case(shuffled_perm(3 * 10**6))], "shuffled permutation, n = 3e6 (30x)")


# --------------------------------------------------------------------------
# 42 - Trapping Rain Water   (1 <= n <= 2e4, 0 <= h <= 1e5)
# --------------------------------------------------------------------------
def gen_42(w, rng):
    def case(v):
        return C().ints(v)

    H = 10**5
    w.edges(42, [
        (case([0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1]), "example 1"),
        (case([4, 2, 0, 3, 2, 5]), "example 2"),
        (case([1]), "single bar"),
        (case([1, 2]), "two bars"),
        (case([2, 0, 2]), "simple valley"),
        (case([0, 0, 0, 0]), "all zeros"),
        (case([1, 2, 3, 4, 5]), "increasing"),
        (case([5, 4, 3, 2, 1]), "decreasing"),
        (case([5, 4, 1, 2]), "small pocket"),
        (case([H, 0, H]), "max height valley"),
        (case([3, 3, 3]), "flat"),
        (case([2, 1, 0, 1, 3, 0, 4]), "nested pockets"),
    ])

    for f in range(1, 6):
        w.write(42, f"random_{f:02d}", [case([rng.randint(0, 10) for _ in range(rng.randint(1, 60))])
                                        for _ in range(300)], "300 random cases, n <= 60")

    N = 2 * 10**4
    w.write(42, "heavy_01", [case([rng.randint(0, H) for _ in range(N)]) for _ in range(100)],
            "100 cases, n = 20000, random heights")
    mountain = [min(i, N - 1 - i) * 10 for i in range(N)]
    w.write(42, "heavy_02", [case([H] + [0] * (N - 2) + [H]), case(mountain), case(list(range(N))),
                             case([H - (i % 1000) * 100 for i in range(N)])],
            "structured: huge valley (answer ~2e9), mountain, increasing, sawtooth")
    w.write(42, "stress_01", [case([rng.randint(0, 500) for _ in range(2 * 10**6)])],
            "n = 2e6 (100x the constraint), heights <= 500")


# --------------------------------------------------------------------------
# 44 - Wildcard Matching   (0 <= |s|, |p| <= 2000)
# --------------------------------------------------------------------------
def gen_44(w, rng):
    def case(s, p):
        return C().str(s).str(p)

    w.edges(44, [
        (case("aa", "a"), "example 1"),
        (case("aa", "*"), "example 2"),
        (case("cb", "?a"), "example 3"),
        (case("adceb", "*a*b"), "stars around letters"),
        (case("acdcb", "a*c?b"), "classic false"),
        (case("", ""), "both empty"),
        (case("", "*"), "empty s, star"),
        (case("", "***"), "empty s, many stars"),
        (case("", "?"), "empty s, question mark"),
        (case("a", ""), "empty pattern"),
        (case("abc", "a**c"), "consecutive stars"),
        (case("mississippi", "m??*ss*?i*pi"), "classic mixed false"),
        (case("aaaa", "***a"), "stars then letter"),
        (case("abcde", "*?*?*?*?*?*"), "five ? between stars"),
        (case("abcd", "*?*?*?*?*?*"), "one ? too many"),
        (case("aaaaaaaaaaaaaaaaaaab", "*a*a*a*a*a*a*a*a*a*c"), "backtracking trap"),
    ])

    def pattern_from(s):
        out = []
        i = 0
        while i < len(s):
            r = rng.random()
            if r < 0.15:
                out.append("*")
                i += rng.randint(0, 3)
            elif r < 0.3:
                out.append("?")
                i += 1
            else:
                out.append(s[i])
                i += 1
        return "".join(out)

    def rnd_case(max_len, alphabet="ab"):
        s = "".join(rng.choice(alphabet) for _ in range(rng.randint(0, max_len)))
        if rng.random() < 0.5:
            p = pattern_from(s)
        else:
            p = "".join(rng.choice(alphabet + "?*") for _ in range(rng.randint(0, max_len)))
        return case(s, p)

    for f in range(1, 6):
        w.write(44, f"random_{f:02d}", [rnd_case(25, rng.choice(["ab", "abc"])) for _ in range(1000)],
                "1000 random cases, lengths <= 25")

    N = 2000
    w.write(44, "heavy_01", [case("a" * N, ("*a" * 999) + "*b")] * 5,
            "5 cases, s = a^2000, p = *a*a...*b (exponential for naive recursion)")
    w.write(44, "heavy_02", [rnd_case(N, "ab") for _ in range(30)], "30 random cases, lengths up to 2000")
    big = []
    for _ in range(20):
        s = "".join(rng.choice("abc") for _ in range(N))
        big.append(case(s, pattern_from(s)[:N]))
    w.write(44, "heavy_03", big, "20 cases, |s| = 2000 with matching-ish patterns")
    w.write(44, "heavy_04", [case("ab" * 1000, "*" + "?b*" * 600 + "c")] * 5,
            "5 cases, s = (ab)^1000, long ?b* chain that fails at the end")
    w.write(44, "heavy_05", [case("a" * N, "*" + "a" * 999 + "b")] * 5,
            "5 cases, s = a^2000, p = *a^999b (worst case for greedy star backtracking)")
    w.write(44, "stress_01", [case("a" * 20_000, ("*a" * 9_999) + "*b")],
            "|s| = |p| = 20000 (10x the constraint), trap")


GENERATORS = {4: gen_4, 10: gen_10, 23: gen_23, 25: gen_25, 30: gen_30,
              32: gen_32, 37: gen_37, 41: gen_41, 42: gen_42, 44: gen_44}


def manifest_path():
    return TESTS / "manifest.json"


def up_to_date():
    try:
        return json.loads(manifest_path().read_text())["version"] == VERSION
    except Exception:
        return False


def generate(force=False):
    if not force and up_to_date():
        print("Tests are up to date")
        return
    import shutil
    if TESTS.exists():
        shutil.rmtree(TESTS)
    w = Writer()
    for pid, gen in GENERATORS.items():
        print(f"Generating tests for problem {pid}")
        # Own seed per problem: changing one generator doesn't change the others
        gen(w, random.Random(SEED * 100 + pid))
    manifest_path().write_text(json.dumps({"version": VERSION, "seed": SEED, "tests": w.manifest}, indent=1))
    total = sum(len(v) for v in w.manifest.values())
    print(f"Generated {total} test files in {TESTS}")


if __name__ == "__main__":
    generate(force="--force" in sys.argv)
