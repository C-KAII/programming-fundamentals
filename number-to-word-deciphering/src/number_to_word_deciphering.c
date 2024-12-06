// Programming Fundamentals - Number-to-Word Deciphering
// Designed and developed by Kobi Chambers - Griffith University

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LEN 50
#define NUM_CAPITALS 26
#define NUM_DIGITS 10

#define CHAR_TO_DIGIT(c) ((int)(c - '0'))
#define CHAR_TO_ALPHA_INDEX(c) ((int)(c - 'A'))

typedef struct trie_node {
  struct trie_node *children[NUM_CAPITALS];
  bool is_end;
} trie_node;

typedef struct {
  int number;
  int counts[NUM_DIGITS];
} word_number;

typedef struct {
  word_number *items;
  int count;
  int capacity;
} word_number_array;

// Define global variables
trie_node *root;
word_number_array number_array;

char assignments[NUM_DIGITS];
bool letter_used[NUM_CAPITALS];
bool distinct_digits[NUM_DIGITS] = {false};
unsigned long dict_lookups = 0, num_solutions = 0, permutations = 0;

// Function prototypes
bool check_arguments(int argc, char *argv[]);
bool is_valid_int(const char *str);
void process_arguments(int argc, char *argv[]);

void execute();
void assign_letters(char *number, int index);

trie_node *create_node(void);
void trie_insert(trie_node *root, const char *key);
bool trie_search(trie_node *root, const char *key, bool full_word);

void init_word_number_array(int initial_capacity);
void insert_word_number_array(int number);

void read_words(const char *dict_filename);

int compare_word_numbers(const void *a, const void *b);
unsigned long long calculate_permutations();

void free_trie(trie_node *node);
void free_word_number_array();
void clean_up();

////////////////////////////////
// MAIN AND UTILITY FUNCTIONS //
////////////////////////////////

int main(int argc, char *argv[]) {
  char *dict_filename = "dictionary.txt";

  if (!check_arguments(argc, argv)) {
    return 1;
  }

  atexit(clean_up);

  init_word_number_array(NUM_DIGITS);
  process_arguments(argc, argv);
  qsort(number_array.items, number_array.count, sizeof(word_number),
        compare_word_numbers);

  read_words(dict_filename);

  clock_t start = clock();
  execute();
  clock_t end = clock();

  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

  printf("Found %lu solutions, CPU Time = %f, Dictionary Look Ups = %lu\n",
         num_solutions, cpu_time_used, dict_lookups);

  unsigned long long possible_permutations = calculate_permutations();
  float perm_percentage = (float)(permutations) / possible_permutations * 100.0;

  printf(
      "Possible Permutations = %llu, Actual Completed Permutations = %lu "
      "(%.6f%%)\n",
      possible_permutations, permutations, perm_percentage);

  return 0;
}

bool check_arguments(int argc, char *argv[]) {
  if (argc < 2) {
    printf(
        "Format as: ./a.out <word_in_digits> "
        "<optional_extra_words_in_digits>\n");
    return false;
  }

  for (int i = 1; i < argc; i++) {
    if (strlen(argv[i]) < 2) {
      printf(
          "Error for input <word_in_digits>: %s should contain 2 or more "
          "digits.\n",
          argv[i]);
      return false;
    }

    if (!is_valid_int(argv[i])) {
      printf(
          "Error for input <word_in_digits>: %s should be a positive integer "
          "value.\n",
          argv[i]);
      return false;
    }
  }

  return true;
}

bool is_valid_int(const char *str) {
  char *end_ptr;
  int value = (int)strtol(str, &end_ptr, 10);

  return (*end_ptr == '\0' && value != 0 && value >= 0 && value <= INT_MAX);
}

void process_arguments(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    int number = atoi(argv[i]);
    insert_word_number_array(number);
  }
}

//////////////////////
// DRIVER FUNCTIONS //
//////////////////////

void execute() {
  for (int i = 0; i < number_array.count; i++) {
    char number_string[12];
    // Convert to string
    sprintf(number_string, "%d", number_array.items[i].number);

    memset(assignments, '\0', sizeof(assignments));
    memset(letter_used, false, sizeof(letter_used));

    assign_letters(number_string, 0);
  }
}

void assign_letters(char *number, int index) {
  if (number[index] == '\0') {
    char word[index + 1];

    // Check if word generated is valid
    for (int i = 0; i < index; i++) {
      word[i] = assignments[CHAR_TO_DIGIT(number[i])];
    }

    word[index] = '\0';

    if (trie_search(root, word, true)) {
      num_solutions++;
    }

    permutations++;
    return;
  }

  int digit = CHAR_TO_DIGIT(number[index]);
  char partial_word[index + 1];

  for (int i = 0; i < index; i++) {
    partial_word[i] = assignments[CHAR_TO_DIGIT(number[i])];
  }
  partial_word[index] = '\0';

  if (index > 1 && !trie_search(root, partial_word, false)) {
    return;
  }

  if (assignments[digit]) {
    assign_letters(number, index + 1);
    return;
  }

  for (int i = 0; i < NUM_CAPITALS; i++) {
    if (!letter_used[i]) {
      assignments[digit] = 'A' + i;
      letter_used[i] = true;

      assign_letters(number, index + 1);

      assignments[digit] = '\0';
      letter_used[i] = false;
    }
  }
}

//////////////////////
// STRUCT FUNCTIONS //
//////////////////////

trie_node *create_node(void) {
  trie_node *ptr_node = (trie_node *)malloc(sizeof(trie_node));
  ptr_node->is_end = false;

  for (int i = 0; i < NUM_CAPITALS; i++) {
    ptr_node->children[i] = NULL;
  }

  return ptr_node;
}

void trie_insert(trie_node *root, const char *key) {
  trie_node *ptr_crawl = root;
  int length = strlen(key);

  for (int level = 0; level < length; level++) {
    int index = CHAR_TO_ALPHA_INDEX(key[level]);

    if (!ptr_crawl->children[index]) {
      ptr_crawl->children[index] = create_node();
    }

    ptr_crawl = ptr_crawl->children[index];
  }

  ptr_crawl->is_end = true;
}

bool trie_search(trie_node *root, const char *key, bool full_word) {
  trie_node *ptr_crawl = root;
  int length = strlen(key);

  for (int level = 0; level < length; level++) {
    int index = CHAR_TO_ALPHA_INDEX(key[level]);
    if (!ptr_crawl->children[index]) {
      return false;
    }

    ptr_crawl = ptr_crawl->children[index];
    dict_lookups++;
  }

  if (full_word) {
    return (ptr_crawl != NULL && ptr_crawl->is_end);
  }

  return true;
}

void init_word_number_array(int initial_capacity) {
  number_array.capacity = initial_capacity;
  number_array.count = 0;
  number_array.items =
      (word_number *)malloc(initial_capacity * sizeof(word_number));

  if (!number_array.items) {
    perror("Error during array initialization");
    exit(1);
  }
}

void insert_word_number_array(int number) {
  if (number_array.count == number_array.capacity) {
    number_array.capacity *= 2;
    number_array.items = (word_number *)realloc(
        number_array.items, number_array.capacity * sizeof(word_number));

    if (!number_array.items) {
      perror("Error while resizing array");
      exit(1);
    }
  }

  number_array.items[number_array.count].number = number;
  memset(number_array.items[number_array.count].counts, 0,
         sizeof(int) * NUM_DIGITS);

  while (number) {
    int digit = number % 10;

    number_array.items[number_array.count].counts[digit]++;
    distinct_digits[digit] = true;
    number /= 10;
  }

  number_array.count++;
}

///////////////////////
// UTILITY FUNCTIONS //
///////////////////////

void read_words(const char *dict_filename) {
  FILE *dictionary = fopen(dict_filename, "r");

  if (!dictionary) {
    perror("Error opening dictionary file\n");
    exit(1);
  }

  char word[MAX_WORD_LEN];
  root = create_node();

  while (fscanf(dictionary, "%45s", word) != EOF) {
    if (strlen(word) <= MAX_WORD_LEN) {
      for (int i = 0; word[i]; i++) {
        word[i] = toupper(word[i]);
      }

      trie_insert(root, word);
    }
  }

  fclose(dictionary);
}

int compare_word_numbers(const void *a, const void *b) {
  word_number *num1 = (word_number *)a;
  word_number *num2 = (word_number *)b;

  int weight1 = 0, weight2 = 0;

  for (int i = 0; i < NUM_DIGITS; i++) {
    weight1 += num1->counts[i];
    weight2 += num2->counts[i];
  }

  return weight2 - weight1;  // Descending
}

unsigned long long calculate_permutations() {
  int distinct_digits_count = 0;

  for (int i = 0; i < NUM_DIGITS; i++) {
    if (distinct_digits[i]) {
      distinct_digits_count++;
    }
  }

  unsigned long long result = 1;

  for (int i = 0; i < distinct_digits_count; i++) {
    result *= (26 - i);
  }

  return result;
}

void free_trie(trie_node *node) {
  if (node == NULL) {
    return;
  }

  for (int i = 0; i < NUM_CAPITALS; i++) {
    free_trie(node->children[i]);
  }

  free(node);
}

void free_word_number_array() { free(number_array.items); }

void clean_up() {
  free_trie(root);
  free_word_number_array();
}
