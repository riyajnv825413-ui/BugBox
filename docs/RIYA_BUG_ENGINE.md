# Riya - Bug Generation Engine

## Purpose

The Bug Generation Engine creates programming challenges
containing intentionally buggy C++ code.

## Supported Difficulties

### Easy
- Logic Error
- Syntax Error
- Loop Error

### Medium
- Array Error
- Condition Error

### Hard
- Nested Loop Error
- Recursion Error

## BugData

Every generated bug returns:

- Bug Type
- Difficulty
- Buggy Code
- Expected Output
- Hint
- Solution

## Main Interface

```cpp
BugData generateBug(Difficulty difficulty);