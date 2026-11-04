# LLM-Coding-Performance

How well do **open-source LLMs that run on a consumer GPU** solve **hard LeetCode problems in C++**?

Five models wrote solutions to 10 LeetCode *Hard* problems. Each solution was compiled and run on
**253,785 test cases**, ranging from tiny corner cases to LeetCode's maximum input sizes and beyond. It was
checked against a trusted reference for **correctness**, **speed**, **memory** and **code quality**.
This is a quick study developed for a university extension fair.

📄 **Full report: [benchmark/results/report.pdf](benchmark/results/report.pdf)** (charts, every metric explained,
one page per problem).

---

## Results

| # | Model | Problems accepted | Test files passed | Compiles | Speed vs reference¹ | Sanitizer issues |
|---|---|---|---|---|---|---|
| 1 | [Qwen2.5-Coder-7B-Instruct](https://huggingface.co/Qwen/Qwen2.5-Coder-7B-Instruct) | **9/10** | 202/209 (96.7%) | 10/10 | 3.81× | 0 |
| 2 | [phi-4](https://huggingface.co/microsoft/phi-4) | **9/10** | 197/209 (94.3%) | 10/10 | 4.77× | 1 |
| 3 | [Qwen2.5-Coder-14B-Instruct](https://huggingface.co/Qwen/Qwen2.5-Coder-14B-Instruct) | **9/10** | 184/209 (88.0%) | 9/10 | 6.92× | 0 |
| 4 | [Qwen3-8B](https://huggingface.co/Qwen/Qwen3-8B) (thinking) | 5/10 | 100/209 (47.8%) | 5/10 | 9.58× | 0 |
| 5 | [Qwen3-8B](https://huggingface.co/Qwen/Qwen3-8B) (no think) | 3/10 | 101/209 (48.3%) | 7/10 | 1.06×² | 2 |

¹ Geometric mean of *solution time ÷ reference time* on the heavy tests of accepted problems (1× = as fast).
² Only over its 3 accepted problems, all of which are easy to make fast. It is not comparable to the other rows.

Verdict per problem (AC = accepted, followed by its speed vs the reference; x/y = test files passed):

| Model | 4 | 10 | 23 | 25 | 30 | 32 | 37 | 41 | 42 | 44 |
|---|---|---|---|---|---|---|---|---|---|---|
| Qwen2.5-Coder-7B | ✅ 1.04× | ✅ 1.59× | ✅ 1.88× | ✅ 0.88× | ❌ RE 15/22 | ✅ 1.09× | ✅ 188× | ✅ 1.00× | ✅ 0.92× | ✅ 326× |
| phi-4 | ✅ 1.03× | ✅ 1.54× | ✅ 1.10× | ⏱ TLE 5/17 | ✅ 3.45× | ✅ 1.07× | ✅ 219× | ✅ 1.00× | ✅ 3.08× | ✅ 292× |
| Qwen2.5-Coder-14B | ⚠️ CE | ✅ 1.60× | ✅ 1.09× | ✅ 1.16× | ✅ 87× | ✅ 1.15× | ✅ 195× | ✅ 1.03× | ✅ 3.00× | ✅ 301× |
| Qwen3-8B (thinking) | ∅ no code | ∅ no code | ∅ no code | ∅ no code | ∅ no code | ✅ 1.19× | ✅ 207× | ✅ 1.01× | ✅ 0.85× | ✅ 383× |
| Qwen3-8B (no think) | ❌ WA 11/25 | ❌ WA 12/25 | ⚠️ CE | ⚠️ CE | ❌ WA 7/22 | ✅ 1.10× | ⚠️ CE | ✅ 1.25× | ✅ 0.87× | ⏱ TLE 10/26 |

**Takeaways**

- **Size isn't everything.** The 7B coder ties the 14B coder and phi-4 (14B) at 9/10 problems, and passes the most tests.
- **Each failure is a classic bug:**
  - unsigned-integer underflow (7B, #30);
  - a variable shadowing bug that loops forever (phi-4, #25);
  - a helper called before it is declared (14B, #4);
  - an `#include <priority_queue>` header that doesn't exist, plus a function defined inside a function (Qwen3 no think).
- **The reasoning model ran out of tokens.** Qwen3-8B with thinking produced no code for half the problems: its
  12,000-token budget was used up while it was still reasoning.
- **Correct is not the same as fast.** On Sudoku (#37) and Wildcard Matching (#44), every model wrote textbook brute
  force or full-table DP. Those are accepted, but 190×–380× slower than the best-known approach. The hardest sudoku
  takes them ~6 s, against 8 ms for the reference.
- **Following instructions:** 12 of 45 solutions don't compile without LeetCode's implicit headers
  (e.g. they use `INT_MIN` or `vector` without including their headers), even though the prompt asked for every needed
  `#include`.

Results from 2026-11-04 on an Intel i7-10700F with g++ 13.3 (`-O2`). The code was generated on an NVIDIA RTX 3060 (12 GB).

---

## Repository structure

```
LLM-Coding-Performance/
├── README.md
├── docs/
│   └── leetcode_problems.pdf      # the 10 problem statements
├── generation/
│   └── generate.py                # step 1: asks each Hugging Face model to solve the problems
├── generated_code/                # step 1 output: raw AI code, NEVER modified
│   └── <model>/
│       ├── <problem>_prompt0.cpp  #   extracted code
│       └── <problem>_prompt0.txt  #   full model answer (for diagnosis)
├── treated_generated_code/        # step 2: same code + I/O/timing harness (auto-generated)
├── benchmark/                     # steps 2–4: tests, run, statistics (see benchmark/README.md)
│   ├── run_benchmark.py           #   entry point
│   ├── gen_tests.py, oracle.py    #   test generator + brute-force answer checker
│   ├── reference/                 #   hand-written reference solutions
│   ├── harness/                   #   main() per problem: reads input, times the call, writes output
│   ├── make_report.py             #   statistics -> PDF / Markdown / CSV
│   └── results/                   #   report.pdf, summary.md, tests.csv, results.json
├── models/                        # Hugging Face cache (git-ignored, ~85 GB)
├── pyproject.toml, uv.lock        # Python dependencies (uv)
└── LICENSE
```

## How it works

```
generation/generate.py          benchmark/run_benchmark.py
─────────────────────           ──────────────────────────────────────────────────────────────
LeetCode statement  ──►  model ──►  generated_code/  ──►  treated copy  ──►  compile  ──►  run on
+ C++ starter                       (raw, untouched)      (+ harness)        (-O2,        221 test
                                                                             ASan/UBSan)  files
                                                                                            │
     report.pdf / summary.md / tests.csv  ◄──  compare with reference + oracle  ◄───────────┘
```

1. **Generation.** Each model gets the problem statement (from the
   [LeetCodeDataset](https://huggingface.co/datasets/newfacade/LeetCodeDataset)) and a C++ starter with the function
   signature. Decoding is greedy (no sampling), so the same prompt always gives the same code. The last
   ```` ```cpp ```` block of the answer is saved.
2. **Treatment.** The code is copied to `treated_generated_code/` **byte for byte**. Only two things are added around
   it: LeetCode's implicit preamble (`#include <bits/stdc++.h>`, `using namespace std;`) and a `main()` that reads
   the test, calls the function, times the call and writes the output. Wrong code is never fixed.
3. **Testing.** The program runs on every test of its problem. Outputs are compared with the reference solution, which
   is itself verified by brute-force oracles.
4. **Statistics.** The results are aggregated into the PDF report, Markdown tables and a CSV file.

### Models

All models were loaded in 4-bit (NF4, bitsandbytes), so they fit on a 12 GB RTX 3060.

| Model | Params | Type | Max new tokens |
|---|---|---|---|
| [Qwen2.5-Coder-14B-Instruct](https://huggingface.co/Qwen/Qwen2.5-Coder-14B-Instruct) | 14.7B | code model | 3,000 |
| [Qwen2.5-Coder-7B-Instruct](https://huggingface.co/Qwen/Qwen2.5-Coder-7B-Instruct) | 7.6B | code model | 3,000 |
| [phi-4](https://huggingface.co/microsoft/phi-4) | 14.7B | general model, strong at reasoning | 3,000 |
| [Qwen3-8B](https://huggingface.co/Qwen/Qwen3-8B) (thinking) | 8.2B | reasoning model: thinks before answering | 12,000 |
| [Qwen3-8B](https://huggingface.co/Qwen/Qwen3-8B) (no think) | 8.2B | same model, `/no_think` system prompt | 3,000 |

### Prompt

```
Solve the following LeetCode problem using C++.

Requirements:
- Write only the function that solves the problem (plus any helper functions it needs).
- Do not wrap the solution in a class and do not write a main function.
- Keep the given function signature and include every #include the code needs.
- Do not print explanations or debugging information.
- Return the solution inside a ```cpp code block.

Problem:
<LeetCode statement>

Complete the code:
```cpp
<starter: #includes + function signature>
```
```

### Problems

All 10 are LeetCode **Hard** ([statements](docs/leetcode_problems.pdf)).

| # | Problem | What makes it hard |
|---|---|---|
| 4 | [Median of Two Sorted Arrays](https://leetcode.com/problems/median-of-two-sorted-arrays/) | O(log(m+n)) binary search on partitions; empty arrays |
| 10 | [Regular Expression Matching](https://leetcode.com/problems/regular-expression-matching/) | `.` and `*` semantics; naive recursion is exponential |
| 23 | [Merge k Sorted Lists](https://leetcode.com/problems/merge-k-sorted-lists/) | needs a heap (O(N log k)) with up to 10⁴ lists |
| 25 | [Reverse Nodes in k-Group](https://leetcode.com/problems/reverse-nodes-in-k-group/) | pointer surgery; leftover group stays as is |
| 30 | [Substring with Concatenation of All Words](https://leetcode.com/problems/substring-with-concatenation-of-all-words/) | duplicate words; sliding window per offset |
| 32 | [Longest Valid Parentheses](https://leetcode.com/problems/longest-valid-parentheses/) | O(n) stack or DP |
| 37 | [Sudoku Solver](https://leetcode.com/problems/sudoku-solver/) | backtracking; very hard puzzles explode naive search |
| 41 | [First Missing Positive](https://leetcode.com/problems/first-missing-positive/) | O(n) time with O(1) memory |
| 42 | [Trapping Rain Water](https://leetcode.com/problems/trapping-rain-water/) | O(n) two pointers |
| 44 | [Wildcard Matching](https://leetcode.com/problems/wildcard-matching/) | `?` and `*`; greedy vs DP; backtracking traps |

---

## Benchmark

### Test tiers

The tests are generated deterministically (fixed seed), so every run uses exactly the same inputs.

| Tier | Files | Cases | What it is | Counts for *Accepted* |
|---|---|---|---|---|
| **edge** | 127 | 127 | One hand-written corner case per file: LeetCode's examples, empty inputs, single elements, extreme values (`INT_MIN`/`INT_MAX`), known traps | yes |
| **random** | 50 | 26,700 | Hundreds or thousands of small random cases per file, double-checked by brute force | yes |
| **heavy** | 32 | 226,915 | LeetCode's **maximum input sizes**, many cases per file, plus adversarial inputs: exponential-backtracking traps, the hardest known sudokus, a puzzle built to defeat brute force | yes |
| **stress** | 12 | 43 | **10× to 1000× beyond** LeetCode's limits (e.g. 10⁶ list nodes, 10⁷-char strings). Shows how solutions scale | no (reported apart) |

Expected answers come from hand-written **reference solutions** that use the best-known algorithms
([benchmark/reference](benchmark/reference)). On every edge and random case, those answers are checked again by simple
**brute-force Python oracles** ([benchmark/oracle.py](benchmark/oracle.py)). The run stops if they ever disagree.

### Metrics

| Metric | Meaning | How it's computed |
|---|---|---|
| **Problems accepted** | Solved like LeetCode's "Accepted" | Every edge, random and heavy test file passed. A single wrong case, crash or timeout fails the problem |
| **Test files passed** | How close a solution got | Passed files out of all edge + random + heavy files (209 per model). Missing or non-compiling code counts as 0 |
| **Compiles** | Code is valid C++ | Compiles with LeetCode's implicit headers (`bits/stdc++.h` + `using namespace std`) |
| **Standalone** | Followed the "include every #include" instruction | The untouched file compiles on its own, with only its own `#include`s |
| **Speed vs reference** | Efficiency of the algorithm | For each heavy test: solution time ÷ reference time. Then the geometric mean per problem, and over the accepted problems per model. 1× = as fast, 10× = ten times slower |
| **Memory vs reference** | Extra memory used | Peak memory the calls added (input excluded) ÷ the reference's, geometric mean. 64 KB is added to both sides so tiny values don't make huge ratios |
| **Stress passed** | Scalability | Stress tests passed (beyond LeetCode's limits). Doesn't affect *Accepted* |
| **Sanitizer issues** | Hidden memory bugs | Problems where AddressSanitizer / UndefinedBehaviorSanitizer reported out-of-bounds access, invalid values, etc. on edge + random tests. Such code may pass by luck |
| **Warnings** | Code hygiene | `-Wall -Wextra` warnings inside the AI code (e.g. signed/unsigned comparison) |
| **Avg LOC** | Size | Lines of code without blank lines and comments |

### Verdicts

Like LeetCode, a problem's verdict is the result of its **first failing test** (edge → random → heavy).

| Verdict | Meaning |
|---|---|
| ✅ **AC** Accepted | All counted tests passed |
| ❌ **WA** Wrong Answer | Ran, but at least one answer differs from the reference |
| ❌ **RE** Runtime Error | Crash (segfault, stack overflow), uncaught exception or abort |
| ⏱ **TLE** Time Limit Exceeded | A test file took more than 10 s (20 s for stress), including infinite loops |
| ❌ **MLE** Memory Limit Exceeded | Allocation failed above 4 GiB |
| ⚠️ **CE** Compile Error | Doesn't compile, even with LeetCode's implicit headers |
| ∅ **No code** | The answer had no C++ code block |

### How time and memory are measured

- **Time:** `steady_clock` around each call to the solution function only (reading the input is excluded). Heavy and
  stress tests run one at a time, 3 runs each, and the median is kept.
- **Memory:** the kernel's peak-memory counter is reset after the input is loaded and read again after the calls.
- **Limits:** 8 MiB stack (Linux default), 4 GiB address space, 10 s per test file.
- **Build:** `g++ -std=c++20 -O2 -Wall -Wextra`, plus an `-fsanitize=address,undefined` build.

### Limitations

- One prompt and one sample per model (greedy decoding). Other prompts or sampling could change the results.
- Times depend on the machine. Ratios vs the reference are more portable than absolute milliseconds.
- "Speed vs reference" only covers accepted problems, so models with few accepted problems aren't directly comparable.

---

## Reproducing

Requirements: Python ≥ 3.13 with [uv](https://docs.astral.sh/uv/), `g++`, `prlimit` (util-linux) and Chrome/Chromium
(for the PDF). Generation also needs an NVIDIA GPU.

```bash
uv sync                                          # install the Python dependencies

# 1. Generate code (slow: downloads the models into models/ and runs them)
#    Pick the models in the MODELS list of generation/generate.py
.venv/bin/python generation/generate.py

# 2. Benchmark: tests, build, run, report (~10 min, standard library only)
.venv/bin/python benchmark/run_benchmark.py

# Rebuild only the report from benchmark/results/results.json
.venv/bin/python benchmark/make_report.py
```

Options and details: [benchmark/README.md](benchmark/README.md).

## Previous experiment

An earlier version of this study asked small models (Qwen2.5-Coder 1.5B/3B, deepseek-coder-1.3b, Phi-4-mini, SmolLM3-3B)
for **complete C programs**. It also collected solutions from large hosted models (DeepSeek V4.1 Flash, Gemini 3,
GPT 5.6, Haiku 5.5). That code isn't benchmarked here, since it uses a different format (whole programs in C).
It is kept in the git history up to commit `06ca9ca`.

## License

[MIT](LICENSE)
