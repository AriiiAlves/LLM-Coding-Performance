# Benchmark summary

Run 2026-11-04T01:58:25 on Intel(R) Core(TM) i7-10700F CPU @ 2.90GHz (g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0). Metrics are explained in [report.pdf](report.pdf) and [../README.md](../README.md).

## Models

| # | Model | Accepted | Tests passed | Compiles | Standalone | Speed vs ref | Memory vs ref | Stress passed | Sanitizer | Warnings | Avg LOC |
|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | Qwen2.5-Coder-7B-Instruct | 9/10 | 202/209 (96.7%) | 10/10 | 7/10 | 3.81x | 1.14x | 11/12 | 0 | 2 | 28 |
| 2 | phi-4 | 9/10 | 197/209 (94.3%) | 10/10 | 8/10 | 4.77x | 1.14x | 10/12 | 1 | 2 | 28 |
| 3 | Qwen2.5-Coder-14B-Instruct | 9/10 | 184/209 (88.0%) | 9/10 | 7/10 | 6.92x | 1.16x | 9/12 | 0 | 0 | 27 |
| 4 | Qwen3-8B | 5/10 | 100/209 (47.8%) | 5/10 | 5/10 | 9.58x | 1.19x | 4/12 | 0 | 1 | 27 |
| 5 | Qwen3-8B (no think) | 3/10 | 101/209 (48.3%) | 7/10 | 6/10 | 1.06x | 0.97x | 4/12 | 2 | 5 | 31 |

## Verdict per problem

AC = accepted (speed vs the reference), WA = wrong answer, RE = runtime error, TLE = time limit, CE = compile error, No code = no code generated; x/y = test files passed.

| Model | 4 | 10 | 23 | 25 | 30 | 32 | 37 | 41 | 42 | 44 |
|---|---|---|---|---|---|---|---|---|---|---|
| Qwen2.5-Coder-7B-Instruct | AC 1.04x | AC 1.59x | AC 1.88x | AC 0.88x | RE 15/22 | AC 1.09x | AC 188x | AC 1.00x | AC 0.92x | AC 326x |
| phi-4 | AC 1.03x | AC 1.54x | AC 1.10x | TLE 5/17 | AC 3.45x | AC 1.07x | AC 219x | AC 1.00x | AC 3.08x | AC 292x |
| Qwen2.5-Coder-14B-Instruct | CE | AC 1.60x | AC 1.09x | AC 1.16x | AC 87x | AC 1.15x | AC 195x | AC 1.03x | AC 3.00x | AC 301x |
| Qwen3-8B | No code | No code | No code | No code | No code | AC 1.19x | AC 207x | AC 1.01x | AC 0.85x | AC 383x |
| Qwen3-8B (no think) | WA 11/25 | WA 12/25 | CE | CE | WA 7/22 | AC 1.10x | CE | AC 1.25x | AC 0.87x | TLE 10/26 |
