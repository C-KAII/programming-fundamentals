// Programming Fundamentals - Word-Guessing Game Manipulation
// Designed and developed by Kobi Chambers - Griffith University

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_LEN 50

typedef struct {
  char **items;
  int remaining;
  int capacity;
} word_array;

typedef struct {
  word_array categories[MAX_WORD_LEN];
  int remaining_count[MAX_WORD_LEN];
} word_category;

// Define global variables
int word_length, num_guesses;
word_array words;

// Function prototypes
bool process_arguments(int argc, char *argv[], char **title);

void play(word_category *categories);
void categorise_words(word_category *categories, char letter, int iteration,
                      int selected_category);
int get_largest_category(word_category *categories);
int get_smallest_category(word_category *categories);
void update_words(word_category *categories, int selected_category);
bool word_in_category(word_array *selected_word_category, char *word);
char *get_random_word(word_array *category);

bool is_valid_int(const char *str);
void read_words(char *title);
bool contains_duplicates(char *word);
void init_word_array(word_array *array);
void insert_word_array(word_array *array, char *word);
void free_word_array(word_array *array);
void init_word_category(word_category *categories);
void free_word_category(word_category *categories);

////////////////////////////////
// MAIN AND UTILITY FUNCTIONS //
////////////////////////////////

int main(int argc, char *argv[]) {
  char *title;

  srand(time(NULL));

  if (process_arguments(argc, argv, &title)) {
    word_category categories;

    init_word_array(&words);
    init_word_category(&categories);
    read_words(title);

    if (!words.items[0]) {
      perror("No words of length: %d without duplicate letters.. (Try < 15)\n");
      exit(1);
    }

    play(&categories);

    free_word_array(&words);
    free_word_category(&categories);

    return 0;
  } else {
    return 1;
  }
}

bool process_arguments(int argc, char *argv[], char **title) {
  if (argc != 4) {
    printf("Format as: ./a.out dictionary.txt <word_length> <num_guesses>\n");
    return false;
  }

  if (!is_valid_int(argv[2])) {
    printf(
        "Error for input <word_length>: argv[2] should be a positive integer "
        "value.\n");
    return false;
  }

  if (!is_valid_int(argv[3])) {
    printf(
        "Error for input <num_guesses>: argv[3] should be a positive integer "
        "value.\n");
    return false;
  }

  // Set pointer values
  *title = argv[1];
  word_length = atoi(argv[2]);
  num_guesses = atoi(argv[3]);

  return true;
}

//////////////////////
// DRIVER FUNCTIONS //
//////////////////////

void play(word_category *categories) {
  char word_state[word_length + 1];
  char letters_guessed[26];  // English letters +1 for '\0'
  bool valid_letter = false;
  int selected_category = 0;
  int current_count = 0;

  // Initialise variables
  for (int i = 0; i < word_length; i++) {
    word_state[i] = '_';
  }
  word_state[word_length] = '\0';

  memset(letters_guessed, 0, sizeof(letters_guessed));

  // Loop users guesses
  for (int i = 0; i < num_guesses; i++) {
    char selected_letter = '\0';

    do {
      valid_letter = false;

      printf("%s", word_state);
      printf(" Enter letter: ");
      scanf(" %c", &selected_letter);

      if (selected_letter >= 'A' && selected_letter <= 'Z') {
        selected_letter = tolower(selected_letter);
      }

      if (selected_letter >= 'a' && selected_letter <= 'z') {
        for (int j = 0; j <= i; j++) {
          if (selected_letter != letters_guessed[j] && j == i) {
            letters_guessed[i] = selected_letter;
            letters_guessed[i + 1] = '\0';
            valid_letter = true;
            break;
          } else if (selected_letter == letters_guessed[j]) {
            printf("%c already used\n", selected_letter);
            j = i;  // Condition to end thee for loop
          }
        }
      } else {
        printf("%c is not a letter\n", selected_letter);
      }
    } while (!valid_letter);

    // Partition into new categories
    categorise_words(categories, selected_letter, i, selected_category);

    bool categories_not_empty = false;

    for (int j = 0; j <= word_length; j++) {
      if (categories->remaining_count[j] != 0) {
        categories_not_empty = true;
        break;
      }
    }

    if (categories_not_empty) {
      // Steer user towards larger category
      // selected_category = get_largest_category(categories);

      // Steer user towards obscure smallest category
      selected_category = get_smallest_category(categories);

      // Update words and word_state accordingly
      update_words(categories, selected_category);

      if (selected_category !=
          word_length)  // Letter did appear in selected category
      {
        // Don't remove guess if letter contained in selected category.
        // (This is how I interpreted the program statement)
        word_state[selected_category] = selected_letter;
        i--;
      }
    }

    current_count = words.remaining;

    // Display game state for user
    printf("Guess %d/%d, Words Left %d, Letters used = %s\n", i + 1,
           num_guesses, current_count, letters_guessed);

    bool word_not_found = false;

    for (int j = 0; j < word_length; j++) {
      if (word_state[j] == '_') {
        word_not_found = true;
      }
    }

    if (current_count == 1 && !word_not_found) {
      printf("Congratulations, you correctly guessed the word - %s\n",
             words.items[0]);
      exit(0);
    } else if (current_count > 1 && i == num_guesses - 1) {
      char *random_word =
          get_random_word(&categories->categories[selected_category]);

      if (random_word != NULL) {
        printf("Word was %s\n", random_word);
      }
    } else if (current_count == 1 && word_not_found && i == num_guesses - 1) {
      printf(
          "Unlucky! You were so close to the word, but couldn't figure it "
          "out...\n");
      printf("Word was - %s\n", words.items[0]);
      exit(0);
    }
  }
}

void categorise_words(word_category *categories, char letter, int iteration,
                      int selected_category) {
  char *word;
  int current_count = 0;

  current_count = words.remaining;

  for (int i = 0; i <= word_length; i++) {
    categories->remaining_count[i] = 0;
  }

  for (int i = 0; i < current_count; i++) {
    // If pos == word_length after loop, should add to
    // category with no letter appearance
    int pos = word_length;
    word = words.items[i];

    // Find position in current word
    for (int j = 0; j < word_length; j++) {
      if (word[j] == letter) {
        pos = j;
        break;
      }
    }

    // Categorise word
    insert_word_array(&categories->categories[pos], word);
    categories->remaining_count[pos]++;
  }
}

int get_largest_category(word_category *categories) {
  int selected_category = 0;
  int max_count = 0;

  for (int i = 0; i <= word_length; i++) {
    int current_count = categories->remaining_count[i];

    if (current_count > max_count) {
      selected_category = i;
      max_count = current_count;
    }
  }

  return selected_category;
}

int get_smallest_category(word_category *categories) {
  int selected_category = 0;
  int min_count = INT_MAX;

  for (int i = 0; i <= word_length; i++) {
    int current_count = categories->remaining_count[i];

    if (current_count < min_count && current_count != 0) {
      selected_category = i;
      min_count = current_count;
    }
  }

  return selected_category;
}

void update_words(word_category *categories, int selected_category) {
  word_array selected_word_category = categories->categories[selected_category];

  word_array new_words;
  init_word_array(&new_words);

  for (int i = 0; i < words.remaining; i++) {
    char *word = words.items[i];

    if (word_in_category(&selected_word_category, word)) {
      char *new_word = strdup(word);
      if (new_word) {
        insert_word_array(&new_words, new_word);
      }
    }
  }

  for (int i = 0; i < words.remaining; i++) {
    free(words.items[i]);
  }
  free(words.items);

  words = new_words;
}

bool word_in_category(word_array *selected_word_category, char *word) {
  for (int i = 0; i < selected_word_category->remaining; i++) {
    if (strcmp(word, selected_word_category->items[i]) == 0) {
      return true;
    }
  }

  return false;
}

char *get_random_word(word_array *category) {
  int word_count = category->remaining;

  if (word_count <= 0) {
    return NULL;
  }

  int index = rand() % word_count;

  return category->items[index];
}

///////////////////////
// UTILITY FUNCTIONS //
///////////////////////

bool is_valid_int(const char *str) {
  char *endptr;
  int value = (int)strtol(str, &endptr, 10);

  return (*endptr == '\0' && value != 0 && value >= 0 && value <= INT_MAX);
}

void read_words(char *title) {
  FILE *dictionary = fopen(title, "r");
  if (!dictionary) {
    perror("Error opening dictionary file\n");
    exit(1);
  }

  char word[46];

  while (fscanf(dictionary, "%45s", word) != EOF) {
    if (strlen(word) == word_length && !contains_duplicates(word)) {
      insert_word_array(&words, word);
    }
  }

  fclose(dictionary);
}

bool contains_duplicates(char *word) {
  for (int i = 0; i < word_length; i++) {
    for (int j = i + 1; j < word_length; j++) {
      if (word[i] == word[j]) {
        return true;
      }
    }
  }
  return false;
}

void init_word_array(word_array *array) {
  array->capacity = MAX_WORD_LEN;
  array->remaining = 0;
  array->items = (char **)malloc(MAX_WORD_LEN * sizeof(char *));

  if (!array->items) {
    perror("Error during array initialisation\n");
    exit(1);
  }

  for (int i = 0; i < MAX_WORD_LEN; i++) {
    array->items[i] = NULL;
  }
}

void insert_word_array(word_array *array, char *word) {
  if (array->remaining == array->capacity) {
    array->capacity *= 2;
    array->items =
        (char **)realloc(array->items, array->capacity * sizeof(char *));

    if (!array->items) {
      perror("Error while resizing array\n");
      exit(1);
    }

    for (int i = array->remaining; i < array->capacity; i++) {
      array->items[i] = NULL;
    }
  }

  array->items[array->remaining] = strdup(word);

  if (!array->items[array->remaining]) {
    perror("Error while copying word\n");
    exit(1);
  }

  array->remaining++;
}

void free_word_array(word_array *array) {
  for (int i = 0; i < array->remaining; i++) {
    free(array->items[i]);
  }
  free(array->items);
}

void init_word_category(word_category *categories) {
  for (int i = 0; i < MAX_WORD_LEN; i++) {
    init_word_array(&categories->categories[i]);
    categories->remaining_count[i] = 0;
  }
}

void free_word_category(word_category *categories) {
  for (int i = 0; i < MAX_WORD_LEN; i++) {
    free_word_array(&categories->categories[i]);
  }
}