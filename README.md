# LLM-Coding-Performance

Quick study about some LLM models coding performance in Leetcode challenges. Work developed for university extension fair.

Click to view [Leetcode Dataset](https://huggingface.co/datasets/newfacade/LeetCodeDataset).

Click to view [generated LLM code](./src/generated_code/).

## Used LLMs

- [Qwen2.5-Coder-1.5B-Instruct](https://huggingface.co/Qwen/Qwen2.5-Coder-1.5B-Instruct)
- [Qwen2.5-Coder-3B-Instruct](https://huggingface.co/Qwen/Qwen2.5-Coder-3B-Instruct)
- [deepseek-coder-1.3b-instruct](https://huggingface.co/deepseek-ai/deepseek-coder-1.3b-instruct)
- [Phi-4-mini-instruct](https://huggingface.co/microsoft/Phi-4-mini-instruct)
- [SmolLM3-3B](https://huggingface-co.translate.goog/HuggingFaceTB/SmolLM3-3B)

## Prompts

Requirements (before the exercise)

```
Requirements:
- Generate a complete C program with only the function that solve the problem.
- Print only the final result to stdout.
- Do not print explanations or debugging information.
- Return the solution inside a ```c code block.
```

- `prompt0` - "Solve the following LeetCode problem using C (not C++). [Requirements] Problem: [Exercise]"
- `prompt1` - "You are an expert C programmer, that knows how to make clean and optimized memory/processing code. Solve the following LeetCode problem using C (not C++). [Requirements] Problem: [Exercise]"

## Selected problems (hard)

- 4 - [Median of Two Sorted Arrays](https://leetcode.com/problems/median-of-two-sorted-arrays/)
- 10 - [Regular Expression Matching](https://leetcode.com/problems/regular-expression-matching/)
- 23 - [Merge k Sorted Lists](https://leetcode.com/problems/merge-k-sorted-lists/)
- 25 - [Reverse Nodes in k-Group](https://leetcode.com/problems/reverse-nodes-in-k-group/)
- 30 - [Substring with Concatenation of All Words](https://leetcode.com/problems/substring-with-concatenation-of-all-words/)
- 32 - [Longest Valid Parentheses](https://leetcode.com/problems/longest-valid-parentheses/)
- 37 - [Sudoku Solver](https://leetcode.com/problems/sudoku-solver/)
- 41 - [First Missing Positive](https://leetcode.com/problems/first-missing-positive/)
- 42 - [Trapping Rain Water](https://leetcode.com/problems/trapping-rain-water/)
- 44 - [Wildcard Matching](https://leetcode.com/problems/wildcard-matching/)
