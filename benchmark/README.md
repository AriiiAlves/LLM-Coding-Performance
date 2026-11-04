# Benchmark

Tests the AI-generated C++ solutions in [`../generated_code`](../generated_code) for correctness, speed
and code quality, and produces statistics.

```bash
# from the repository root (needs g++, prlimit and Chrome/Chromium for the PDF; only Python's standard library)
.venv/bin/python benchmark/run_benchmark.py
```

The report is **[`results/report.pdf`](results/report.pdf)** (rendered with headless Chrome/Chromium; set
`CHROME=/path/to/chrome` if it isn't found, otherwise an HTML file is saved instead). Also generated:
`results/summary.md` (markdown tables), `results/tests.csv` (every test run) and `results/results.json` (raw data).
To rebuild the report without re-running: `.venv/bin/python benchmark/make_report.py`.

Useful options: `--problems 4 10`, `--models microsoft_phi-4`, `--repeats 5`, `--timeout 10`,
`--no-sanitizers`, `--regen-tests`, `--jobs 8`.

## Files

| File | Role |
|---|---|
| `run_benchmark.py` | Entry point: tests → treated copies → build → run → `results/` |
| `gen_tests.py` | Deterministic test generator (all tiers, all problems) |
| `oracle.py` | Brute-force Python answers that double-check the references |
| `reference/` | Hand-written, efficient reference solutions (expected answers + speed baseline) |
| `harness/` | `common.hpp` (input reader, timer, memory meter) + one `main()` per problem |
| `make_report.py` | Statistics → `report.pdf`, `summary.md`, `tests.csv` |
| `results/` | Outputs (committed) |
| `tests/`, `build/` | Generated inputs and binaries (git-ignored, recreated automatically) |

## What it measures

The full explanation of every metric, verdict and test tier is in the report (pages 2–7) and in the
[main README](../README.md#metrics).

| Metric | How |
|---|---|
| **Accepted** | Every *edge*, *random* and *heavy* test passes (like LeetCode, the verdict is the first failing test) |
| **Tests passed** | Test files passed out of the counted tiers (edge + random + heavy) |
| **Compiles** | Compiles in a LeetCode-like environment (`bits/stdc++.h` + `using namespace std` pre-included) |
| **Standalone** | The untouched code compiles on its own, with only its own `#include`s |
| **Speed vs ref** | Geometric mean of (solution time / reference time) on heavy tests of accepted problems |
| **Memory vs ref** | Same, for the peak memory used by the calls |
| **Stress passed** | Tests *beyond* LeetCode's limits (10x–1000x): scalability only, don't count for Accepted |
| **Sanitizer** | Problems where AddressSanitizer / UBSan found memory errors or undefined behavior |
| **Warnings** | `-Wall -Wextra` warnings inside the AI code |
| **Avg LOC** | Lines of code (no blanks/comments) |

## Tests

`gen_tests.py` writes ~220 test files (deterministic, fixed seed) into `tests/` (git-ignored, ~250 MB, regenerated
automatically):

- `edge_*` – one hand-written corner case per file (LeetCode examples, empty inputs, extremes, traps)
- `random_*` – hundreds/thousands of small random cases per file
- `heavy_*` – LeetCode's maximum input sizes, many cases per file, plus adversarial inputs
  (e.g. exponential-backtracking traps for the matching problems, the anti-brute-force sudoku)
- `stress_*` – far beyond the constraints (e.g. 10^6-node lists, 10^7-char strings)

Expected answers come from the reference solutions in `reference/` (efficient, written by hand) and are
cross-checked against the brute-force Python oracles in `oracle.py` on every edge/random case.

## How the code is run

The original files are never touched. `run_benchmark.py` writes treated copies to
[`../treated_generated_code`](../treated_generated_code) with:

1. LeetCode's implicit preamble: `#include <bits/stdc++.h>` and `using namespace std;`
   (plus the `ListNode` struct, only if the code didn't define it),
2. the AI code, byte for byte, between `BEGIN/END AI-GENERATED CODE` markers,
3. a `main()` from `harness/` that reads the input, calls the function and writes the output.

Only the function calls are timed (input parsing excluded). Heavy/stress tests run one at a time, and the
median of 3 runs is kept. Limits: 10 s per test file (20 s for stress), 8 MiB stack, 4 GiB address space.
Build: `g++ -std=c++20 -O2`.
