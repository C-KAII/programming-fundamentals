# Programming Fundamentals - High Performant Custom Data Structures

## Overview

This project involves developing a high-performance C program for managing a large set of integers. The program efficiently handles various operations such as finding, adding, deleting, and determining the predecessor and successor of integers in a data structure. The program also includes performance metrics and data compression techniques.

---

## Problem Description

The ACE software development company requires a program that:
1. Stores a large number of integers with potential duplicates.
2. Provides efficient operations:
  - Find
  - Add
  - Delete
  - Predecessor and successor determination
  - Max and min retrieval
3. Simulates random deletions and additions (10% of numbers).
4. Measures the execution time for each operation.

### Command Line Arguments
The program accepts three parameters: `n`, `r1`, and `r2` (where `r1 > 0` and `r2 > r1`):
  - **`n`**: Number of random integers to generate (if negative, enters a testing mode).
  - **`r1`, `r2`**: Range for generating integers.

### Example:
Command:
```bash
./analyse_nums 100000000 1 100000
```

Output format:

Op counts      Total time       Avg. Time
find           9995247          2.338336
add            10001423         2.346919
...

---

## Features

### Supported Operations
- **Find**: Locates occurrences of a number.
- **Add/Delete**: Inserts or removes a single occurrence of a number.
- **Predecessor/Successor**: Finds the preceding or succeeding number.
- **Min/Max**: Retrieves the smallest or largest number.

### Testing Mode
When `n` is negative, the program enters a mode demonstrating the correct results of all operations.

---

## Performance and Data Compression
- **Efficient Search Techniques**: Avoid sequential search for speed.
- **Dynamic Data Management**: Support efficient add/delete operations post-initialisation.
- **Real-time Min/Max Retrieval**: Always accessible in constant time.
- **Data Compression**: Investigate compression methods to reduce memory usage and assess their impact on performance.

---

## Sample Outputs

### Run 1:
```bash
./analyse_nums 100000000 1 100000
```

n = 100000000, r1 = 1, r2 = 100000, Memory used = 0.400000 Mbytes

        Op counts Total time  Avg. Time
find    9995247   2.338336    2.3394479396e-07
add     10001423  2.346919    2.3465850809e-07
delete  10005569  2.343808    2.3425034598e-07
succ    10000921  2.341604    2.3413883581e-07
pred    9994993   2.330232    2.3313993316e-07
min     10003022  2.224194    2.2235220516e-07
max     9998825   2.221678    2.2219390778e-07

### Run 2:
```bash
./analyse_nums 100000000 1 100000
```

n = 100000000, r1 = 1, r2 = 1000000000, Memory used = 440.000000 Mbytes

        Op counts Total time  Avg. Time
find    9998194   8.355908    8.3574173496e-07
add     10000976  8.804130    8.8032708008e-07
delete  10003965  8.395411    8.3920835389e-07
succ    9999987   8.356767    8.3567778638e-07
pred    9994952   8.354301    8.3585203811e-07
min     10001281  2.254240    2.2539512688e-07
max     10000645  2.253662    2.2535166482e-07

### Run 3:
```bash
./analyse_nums -10 1 100
```

n = 10, r1 = 1, r2 = 100, Memory used = 0.000044 Mbytes

Numbers : 10 10 24 31 41 50 50 59 73 74 74 : min 10 : max 74
find 10 2 : delete 10 1 : find 10 1 : delete 10 1 : add 10 1 : find 10 1 : succ 10 24 : pred 10 -1
find 24 1 : delete 24 1 : find 24 0 : delete 24 0 : add 24 1 : find 24 1 : succ 24 31 : pred 24 10
find 31 1 : delete 31 1 : find 31 0 : delete 31 0 : add 31 1 : find 31 1 : succ 31 41 : pred 31 24
find 41 1 : delete 41 1 : find 41 0 : delete 41 0 : add 41 1 : find 41 1 : succ 41 50 : pred 41 31
find 50 2 : delete 50 1 : find 50 1 : delete 50 1 : add 50 1 : find 50 1 : succ 50 59 : pred 50 41
find 59 1 : delete 59 1 : find 59 0 : delete 59 0 : add 59 1 : find 59 1 : succ 59 73 : pred 59 50
find 73 1 : delete 73 1 : find 73 0 : delete 73 0 : add 73 1 : find 73 1 : succ 73 74 : pred 73 59
find 74 2 : delete 74 1 : find 74 1 : delete 74 1 : add 74 1 : find 74 1 : succ 74 -1 : pred 74 73

Numbers : 10 24 31 41 50 59 73 74 : min 10 : max 74
add 10 1 : find 10 2
add 50 1 : find 50 2
add 74 1 : find 74 2

Numbers : 10 10 24 31 41 50 50 59 73 74 74 : min 10 : max 74

Format is: <op name> <op input> <op result>