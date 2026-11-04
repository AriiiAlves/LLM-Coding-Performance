"""
Independent brute-force answers (simple Python), used to double-check the C++
reference solutions on the small tiers (edge + random). If the reference ever
disagrees with the oracle, the benchmark stops: the expected answers would be wrong.
"""
import sys
from functools import lru_cache

sys.setrecursionlimit(100_000)


class Tokens:
    def __init__(self, text):
        self.t = text.split()
        self.i = 0

    def num(self):
        self.i += 1
        return int(self.t[self.i - 1])

    def tok(self):
        self.i += 1
        return self.t[self.i - 1]

    def str(self):
        n = self.num()
        return self.tok() if n else ""

    def ints(self):
        n = self.num()
        return [self.num() for _ in range(n)]


def regex_match(s, p):
    @lru_cache(None)
    def m(i, j):
        if j == len(p):
            return i == len(s)
        first = i < len(s) and p[j] in (s[i], ".")
        if j + 1 < len(p) and p[j + 1] == "*":
            return m(i, j + 2) or (first and m(i + 1, j))
        return first and m(i + 1, j + 1)
    return m(0, 0)


def wildcard_match(s, p):
    @lru_cache(None)
    def m(i, j):
        if j == len(p):
            return i == len(s)
        if p[j] == "*":
            return m(i, j + 1) or (i < len(s) and m(i + 1, j))
        return i < len(s) and p[j] in (s[i], "?") and m(i + 1, j + 1)
    return m(0, 0)


def lvp(s):
    # Every substring s[i..j]: valid when the balance never dips below 0 and ends at 0
    best = 0
    for i in range(len(s)):
        bal = 0
        for j in range(i, len(s)):
            bal += 1 if s[j] == "(" else -1
            if bal < 0:
                break
            if bal == 0:
                best = max(best, j - i + 1)
    return best


def substring_concat(s, words):
    from collections import Counter
    L, K = len(words[0]), len(words)
    need = Counter(words)
    return [i for i in range(len(s) - L * K + 1)
            if Counter(s[i + j * L:i + (j + 1) * L] for j in range(K)) == need]


def trap(h):
    return sum(max(0, min(max(h[:i + 1]), max(h[i:])) - h[i]) for i in range(len(h)))


def sudoku_ok(puzzle, out):
    if len(out) != 81 or any(c not in "123456789" for c in out):
        return False
    if any(a != "." and a != b for a, b in zip(puzzle, out)):
        return False
    g = [out[r * 9:(r + 1) * 9] for r in range(9)]
    units = [set(row) for row in g] + [set(g[r][c] for r in range(9)) for c in range(9)] + \
            [set(g[r][c] for r in range(br, br + 3) for c in range(bc, bc + 3))
             for br in (0, 3, 6) for bc in (0, 3, 6)]
    return all(len(u) == 9 for u in units)


def answers(pid, text):
    """Expected output lines for a test input (None for problem 37: checked by sudoku_ok)."""
    tk = Tokens(text)
    T = tk.num()
    out = []
    for _ in range(T):
        if pid == 4:
            v = sorted(tk.ints() + tk.ints())
            n = len(v)
            out.append(f"{(v[n // 2] if n % 2 else (v[n // 2 - 1] + v[n // 2]) / 2):.5f}")
        elif pid == 10:
            s, p = tk.str(), tk.str()
            out.append("true" if regex_match(s, p) else "false")
        elif pid == 44:
            s, p = tk.str(), tk.str()
            out.append("true" if wildcard_match(s, p) else "false")
        elif pid == 23:
            k = tk.num()
            vals = []
            for _ in range(k):
                vals += tk.ints()
            out.append(" ".join(map(str, sorted(vals))))
        elif pid == 25:
            v, k = tk.ints(), tk.num()
            res = []
            for i in range(0, len(v), k):
                chunk = v[i:i + k]
                res += chunk[::-1] if len(chunk) == k else chunk
            out.append(" ".join(map(str, res)))
        elif pid == 30:
            s = tk.str()
            words = [tk.str() for _ in range(tk.num())]
            out.append(" ".join(map(str, substring_concat(s, words))))
        elif pid == 32:
            out.append(str(lvp(tk.str())))
        elif pid == 37:
            out.append(tk.tok())  # the puzzle, validated with sudoku_ok
        elif pid == 41:
            v = set(tk.ints())
            x = 1
            while x in v:
                x += 1
            out.append(str(x))
        elif pid == 42:
            out.append(str(trap(tk.ints())))
    return out
