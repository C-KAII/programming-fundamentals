// Programming Fundamentals - High Performant Custom Data Structures
// Designed and developed by Kobi Chambers - Griffith University

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define globals
#define MAX_TEST_SIZE 10

int *number_array = NULL;
int *counts_array = NULL;
int range, len;
int r1;

// Function prototypes
bool process_arguments(int argc, char *argv[], int *n, int *r2);

void generate_array(int n);
void store_counts(int n);
void store_numbers();

void drive(int *n, int *r2);

void functionality_test();
void print_array_state(int minimum, int maximum);
void run_tests(int index, int *counter);
void run_final_tests(int value);

int find(int value);
int add(int value);
int delete(int value);
int pred(int value);
int succ(int value);
int min();
int max();

bool is_valid_int(const char *str);
int rand_in_range();
int compare(const void *a, const void *b);
int binary_search(int value, int *count);
int find_nearest_space(int index);
int shift_array(int index, int insertion);
void free_memory();

////////////////////////////////
// MAIN AND UTILITY FUNCTIONS //
////////////////////////////////

int main(int argc, char *argv[]) {
  int n, r2;
  // srand(time(NULL)); // UNCOMMENT TO GET RANDOM SEED

  if (process_arguments(argc, argv, &n, &r2)) {
    range = (int)(r2 - r1 + 1);

    generate_array(abs(n));

    drive(&n, &r2);

    free_memory();

    return 0;
  } else {
    return 1;
  }
}

// Function to process user input arguments and perform error checking
bool process_arguments(int argc, char *argv[], int *n, int *r2) {
  if (argc != 4) {
    printf("Format as: ./analyse_nums <n> <r1> <r2>\n");
    return false;
  }

  if (!is_valid_int(argv[1])) {
    printf(
        "Error for input (n): argv[1] should be a non-zero integer value.\n");
    return false;
  }

  if (!is_valid_int(argv[2])) {
    printf(
        "Error for input (r1): argv[2] should be a positive integer value.\n");
    return false;
  }

  if (!is_valid_int(argv[3])) {
    printf(
        "Error for input (r2): argv[3] should be a positive integer value.\n");
    return false;
  }

  // Set pointer values
  *n = (int)strtol(argv[1], NULL, 10);
  r1 = (int)strtol(argv[2], NULL, 10);
  *r2 = (int)strtol(argv[3], NULL, 10);

  if (r1 >= *r2) {
    printf("Error: r2 should be greater than r1.\n");
    return false;
  }

  if (r1 < 1) {
    printf("Error: r1 & r2 should be positive integers.\n");
    return false;
  }

  return true;
}

// Utility function to generate a memory efficient array, and set global
// variable for array length
void generate_array(int n) {
  if (range < n) {
    len = range - 1;
    store_counts(n);
  } else {
    len = (int)ceil(n * 1.1);
    store_numbers(r1);
  }
}

// Generate and store random number counts in an array
void store_counts(int n) {
  counts_array = (int *)calloc(range, sizeof(int));

  if (!counts_array) {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  for (int i = 0; i < n; ++i) {
    int number = rand_in_range();
    ++counts_array[number - r1];
  }
}

// Generate and store random numbers in an array
void store_numbers() {
  // Allow 10% additional memory
  number_array = (int *)malloc(len * sizeof(int));

  if (!number_array) {
    printf("Failed to allocate memory for number_array.\n");
    exit(1);
  }

  for (int i = 0; i < len; ++i) {
    number_array[i] = rand_in_range();
  }

  qsort(number_array, len, sizeof(int), compare);

  // Remove 10% of numbers stored
  if (len < 11) {
    delete (number_array[len / 2]);
  } else {
    for (int i = 9; i < len; i += 10) {
      delete (number_array[i]);
    }
  }
}

//////////////////////
// DRIVER FUNCTIONS //
//////////////////////

void drive(int *n, int *r2) {
  size_t int_size, storage_size;
  double memoryUsed;

  bool testingFlag = false;

  if (*n < 0) {
    testingFlag = true;
    *n = abs(*n);
  }

  // Calculate approximate memory usage
  int_size = sizeof(int);

  storage_size = len * int_size;
  memoryUsed = (double)storage_size / (1024 * 1024);

  printf("n = %d, r1 = %d, r2 = %d, Memory used = %.6f Mbytes\n", *n, r1, *r2,
         memoryUsed);

  // User entered -n value, run functionality tests
  if (testingFlag) {
    functionality_test();
  }

  // Initialise 7 counters and timers for the 7 operations
  int operation_counts[7] = {0};
  double total_time[7] = {0.0};
  int num_operations = 7 * (*n / 10);  // 10% for each operation

  for (int i = 0; i < num_operations; ++i) {
    int operation = rand() % 7;
    clock_t start_t = clock();

    switch (operation) {
      case 0:
        find(rand_in_range());
        break;

      case 1:
        add(rand_in_range());
        break;

      case 2:
        delete (rand_in_range());
        break;

      case 3:
        succ(rand_in_range());
        break;

      case 4:
        pred(rand_in_range());
        break;

      case 5:
        min();
        break;

      case 6:
        max();
        break;
    }

    clock_t end_t = clock();
    total_time[operation] += ((double)(end_t - start_t)) / CLOCKS_PER_SEC;

    ++operation_counts[operation];
  }

  // Calculate and display outputs

  printf("         Op counts    Total time    Avg. Time\n");
  char *operation_names[] = {"find", "add", "delete", "succ",
                             "pred", "min", "max"};

  for (int i = 0; i < 7; ++i) {
    printf("%-8s %-12d %-13f %-18e\n", operation_names[i], operation_counts[i],
           total_time[i], total_time[i] / operation_counts[i]);
  }
}

// Function for testing program operational capacity
void functionality_test() {
  // Initialise test_counters array
  int *test_counters = calloc(MAX_TEST_SIZE, sizeof(int));
  for (int i = 0; i < len; ++i) {
    ++test_counters[number_array[i]];
  }

  // Test operations
  for (int i = 0; i < len; ++i) {
    run_tests(i, test_counters);
  }

  // Initial state
  print_array_state(min(), max());

  // Remaining operations
  for (int i = 0; i < MAX_TEST_SIZE; ++i) {
    if (test_counters[i] > 1) {
      run_final_tests(number_array[i]);
    }
  }

  // Final state
  print_array_state(min(), max());

  free(test_counters);
}

void print_array_state(int minimum, int maximum) {
  printf("\nNumbers : ");
  for (int i = 0; i < len; ++i) {
    printf("%d ", number_array[i]);
  }

  printf(": min %d : max %d\n", minimum, maximum);
}

void run_tests(int index, int *counter) {
  int value = number_array[index];

  printf("find %d %d : ", value, find(value));
  printf("delete %d %d : ", value, delete (value));
  printf("find %d %d : ", value, find(value));
  printf("delete %d %d : ", value, delete (value));
  printf("add %d %d : ", value, add(value));
  printf("find %d %d : ", value, find(value));
  printf("succ %d %d : ", value, succ(value));
  printf("pred %d %d\n", value, pred(value));

  ++counter[value];
}

void run_final_tests(int value) {
  printf("add %d %d : ", value, add(value));
  printf("find %d %d\n", value, find(value));
}

/////////////////////////
// OPERATIVE FUNCTIONS //
/////////////////////////

// Function to return number of times the value occurs in stored numbers
int find(int value) {
  if (counts_array) {
    return counts_array[value - r1];
  }

  // number_array functionality
  int count;
  if (binary_search(value, &count) < 0) {
    return 0;
  } else {
    return count;
  }
}

// Function to add the value to stored numbers
int add(int value) {
  if (counts_array) {
    ++counts_array[value - r1];
    return 1;
  }

  // number_array functionality
  int nearest_space;
  int index = binary_search(value, NULL);

  int insertion = index >= 0 ? index : -index;
  if (insertion == len + 1) {
    insertion = 0;  // Out-of-bounds index indicates value < min number in array
  }

  if (number_array[insertion] == -1) {
    number_array[insertion] = value;
    return 1;
  }

  nearest_space = find_nearest_space(insertion);
  if (nearest_space >= 0) {
    int direction = shift_array(insertion, nearest_space);
    if (direction < 0) {
      number_array[insertion - 1] = value;
    } else if (direction > 0) {
      number_array[insertion] = value;
    }
  }

  return 0;
}

// Function to delete a single value from stored numbers
int delete(int value) {
  if (counts_array) {
    if (counts_array[value - r1] > 0) {
      --counts_array[value - r1];
      return 1;
    }
    return 0;
  }

  // number_array functionality
  int index = binary_search(value, NULL);

  if (index >= 0) {
    number_array[index] = -1;
    return 1;
  } else {
    return 0;
  }
}

// Function to return the number before the value appears in stored numbers
int pred(int value) {
  if (counts_array) {
    // Initialise loop at value - 2 for predecessor
    for (int i = value - 2; i >= 0; --i) {
      if (counts_array[i] > 0) {
        return i + r1;
      }
    }
    return -1;
  }

  // number_array functionality
  int index = binary_search(value, NULL);
  if (index == 0 || index == -len - 1) {
    return -1;  // Exit cases where we are already at beginning of array
  } else if (index < 0) {
    // Negative index == index value would be found at -index
    return number_array[-index - 1];
  }

  int left = binary_search(number_array[index] - 1, NULL);
  if (left >= 0) {
    // The array value at index left is the predecessor to value
    return number_array[left];
  } else if (left == -len - 1) {
    // Value would go at start of array
    return -1;
  } else if (left < 0) {
    // bsearch finds leftmost index for where value would go, so left - 1 is
    // predecessor
    return number_array[-left - 1];
  }

  return -1;
}

// Function to return the number after the value appears in stored numbers
int succ(int value) {
  if (counts_array) {
    // Initialise loop at value for successor
    for (int i = value; i < range; ++i) {
      if (counts_array[i] > 0) {
        return i + r1;
      }
    }
    return -1;
  }

  // number_array functionality
  int count;
  int index = binary_search(value, &count);
  if (index == len - 1) {
    return -1;  // Exit case, already at end of array
  } else if (index >= 0) {
    int next = index + count;
    while (next < len && number_array[next] == -1) {
      ++next;
    }

    if (next < len) {
      return number_array[next];
    }
  } else {
    int i = -index + 1;  // -index, iterate right to find next non -1 value
    while (i < len && number_array[i] == -1) {
      ++i;
    }

    if (i < len) {
      return number_array[i];
    }
  }

  return -1;
}

// Function to find minimum number in the array
int min() {
  // This is probably computationally faster for number_array
  // than my implementation.. maybe except for very very large arrays
  if (counts_array) {
    for (int i = 0; i < range; ++i) {
      if (counts_array[i] > 0) {
        return i + r1;
      }
    }
    return -1;
  }

  // number_array functionality
  int predecessor = 0;
  int left = 0, mid, right = len - 1;
  while (left <= right) {
    mid = left + (right - left) / 2;

    if (number_array[mid] != -1 && (mid == 0 || number_array[mid - 1] == -1)) {
      predecessor = pred(number_array[mid]);

      if (predecessor == -1) {
        return number_array[mid];
      } else {
        if (predecessor == number_array[0]) {
          return predecessor;  // next value is min
        } else if (succ(number_array[mid]) == -1) {
          // Only number in array if both pred and succ == -1
          return number_array[mid];
        } else {
          right = binary_search(predecessor, NULL);
          left = 0;
        }
      }
    } else if (number_array[mid] == -1) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  // Will return either the last valid number found, or -1 if failed
  return number_array[right];
}

// Function to find maximum number in the array
int max() {
  // This is probably computationally faster for number_array
  // than my implementation.. maybe except for very very large arrays
  if (counts_array) {
    for (int i = range - 1; i >= 0; --i) {
      if (counts_array[i] > 0) {
        return i + r1;
      }
    }
    return -1;
  }

  // number_array functionality
  int successor = 0;
  int left = 0, mid, right = len - 1;

  while (left <= right) {
    mid = left + (right - left) / 2;

    if (number_array[mid] != -1 &&
        (mid == len - 1 || number_array[mid + 1] == -1)) {
      successor = succ(number_array[mid]);

      if (successor == -1) {
        return number_array[mid];
      } else {
        if (successor == number_array[len - 1]) {
          return successor;  // next value is max
        } else if (pred(number_array[mid]) == -1) {
          // Only number in array if both pred and succ == -1
          return number_array[mid];
        } else {
          left = binary_search(successor, NULL);
          right = len - 1;
        }
      }
    } else if (number_array[mid] == -1) {
      right = mid - 1;
    } else {
      left = mid + 1;
    }
  }

  // Will return either the last valid number found, or -1 if failed
  return number_array[left];
}

///////////////////////
// UTILITY FUNCTIONS //
///////////////////////

// Utility function to check user input valid integers
bool is_valid_int(const char *str) {
  char *endptr;
  int value = (int)strtol(str, &endptr, 10);

  return (*endptr == '\0' && value != 0 && value >= INT_MIN &&
          value <= INT_MAX);
}

// Utility function to generate a random number in defined range
int rand_in_range() { return rand() % range + r1; }

// Utility function to provide comparison for qsort
int compare(const void *a, const void *b) {
  int val1 = *(const int *)a;
  int val2 = *(const int *)b;

  return (val1 > val2) - (val1 < val2);
}

// Utility function to find first occurance of value in numbers array
int binary_search(int value, int *count) {
  int left = 0, mid, right = len - 1;

  while (left <= right) {
    mid = (left + right) / 2;

    if (number_array[mid] == -1) {
      // Consider both directions from -1
      int mid_left = mid - 1, mid_right = mid + 1;

      while (mid_left >= left && number_array[mid_left] == -1) {
        --mid_left;
      }

      while (mid_right <= right && number_array[mid_right] == -1) {
        ++mid_right;
      }

      if (mid_left >= left && number_array[mid_left] == value) {
        mid = mid_left;
      } else if (mid_right <= right && number_array[mid_right] == value) {
        mid = mid_right;
      } else {
        // Adjust bounds if value not found
        if (mid_left >= left && number_array[mid_left] > value) {
          right = mid_left - 1;
        } else if (mid_right <= right && number_array[mid_right] < value) {
          left = mid_right + 1;
        } else {
          // Check if unfound value would insert between two values with a -1
          // between then
          if (mid_left >= left && mid_right <= right &&
              number_array[mid_left] < value &&
              number_array[mid_right] > value) {
            left = mid_left + 1;  // Adjust to correct insertion point
          }

          break;  // Value not found - correct insertion point found
        }

        continue;
      }
    }

    if (number_array[mid] == value) {
      if (mid == left || number_array[mid - 1] != value) {
        // Count occurances of value if count pointer was parsed in call
        if (count != NULL) {
          int first_occ = mid, last_occ = mid;
          while (first_occ > 0 && number_array[first_occ - 1] == value) {
            --first_occ;
          }

          while (last_occ < len - 1 && number_array[last_occ + 1] == value) {
            ++last_occ;
          }

          *count = last_occ - first_occ + 1;
        }
        return mid;  // Value found and is first occ
      } else {
        right = mid - 1;  // Keep searching for first occ
      }
    }

    if (number_array[mid] < value)  // Search left or right half
    {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  if (left == 0) {
    return -len - 1;  // Return unreachable value if not found but index == 0
  } else {
    return -left;  // Return -index where val would be
  }
}

// Utility function to find the closest -1 or 'deleted' value to insertion index
int find_nearest_space(int start) {
  int count;
  int left = start - 1, right = start;

  while (left >= 0 || right <= len - 1) {
    if (right <= len - 1) {
      count = 0;
      if (binary_search(number_array[right], &count) < 0) {
        return right;
      }
      right += count;
    }

    if (left >= 0) {
      count = 0;
      if (binary_search(number_array[left], &count) < 0) {
        return left;
      }
      left -= count;
    }
  }

  return -1;
}

// Utility function to shift section of array in specified direction
int shift_array(int insertion, int nearest_space) {
  // Shift right
  if (nearest_space - insertion > 0) {
    for (int i = nearest_space; i > insertion; --i) {
      number_array[i] = number_array[i - 1];
    }

    return 1;  // Positive indicates right shift
  } else {
    // Shift left
    for (int i = nearest_space; i < insertion - 1; ++i) {
      number_array[i] = number_array[i + 1];
    }

    return -1;  // Negative indicates left shift
  }

  return 0;
}

// Utility function to free allocated memory after use
void free_memory() {
  if (number_array) {
    free(number_array);
    number_array = NULL;
  }

  if (counts_array) {
    free(counts_array);
    counts_array = NULL;
  }
}