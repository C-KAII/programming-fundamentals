# Programming Fundamentals - Number-to-Word Deciphering

## Overview

Create a program that maps numeric sequences to valid words by assigning unique digits to letters using a given dictionary.

---

## Requirements
1. **Command-line Input**: Accept sequences of numbers.
2. **Letter-Digit Mapping**: Assign each digit (0-9) to a unique uppercase letter.
3. **Dictionary Validation**: Verify that generated words exist in an uppercase version of `dictionary.txt`.
4. **Performance Metrics**: Display total solutions found, CPU time, dictionary lookups, and completed permutations.

---

## Complexity
Given `r` distinct digits, the number of possible permutations is:

26! / (26 - r)!

---

## Sample Commands & Outputs

### Example 1
```bash
./a.out 9567 1085 10652
```

**Output:**

Found 68013 solutions, CPU Time = 0.428983, Dictionary Look Ups = 5410962
Possible Permutations = 6.2990928e+10, Actual Completed Permutations = 3494100 (0.005547%)


### Example 2
```bash
./a.out 12345 54321
```

**Output:**

Found 104 solutions, CPU Time = 0.757589, Dictionary Look Ups = 7899225
Possible Permutations = 2.8405900e+07, Actual Completed Permutations = 7893600 (27.788593%)