# Programming Fundamentals - Word-Guessing Game Manipulation

## Overview

Develop a program that manipulates a word-guessing game by dynamically selecting word categories to maximise the computer's advantage over the human player.

## Game Rules
1. One player selects a secret word and displays dashes representing the word’s length.
2. The other player guesses letters.
3. If the letter is in the word, its position is revealed. Otherwise, the guess is marked wrong.
4. The game ends when all letters are revealed or guesses run out.

### Program Strategy
Instead of selecting a single word, the program categorises words based on guessed letters and chooses the largest remaining category to mislead the human player.

## Implementation Requirements
1. **Command-line Arguments**: Accept a dictionary file, word length, and number of guesses.
2. **Dictionary Filtering**: Load and filter words from `dictionary.txt` based on the specified word length (no duplicate letters).
3. **Gameplay Mechanics**:
  - Display remaining guesses, guessed letters, current word state, and number of possible words.
  - Validate single-letter input.
  - Partition words into categories and eliminate non-matching words.
  - Deduct a guess if no letters match the category.
  - End the game when the player wins or runs out of guesses.

## Examples

### Sample Command
```bash
./a.out dictionary.txt 6 10
```

### Sample Output
______  Enter letter: a
Guess 1/10, Words Left 3595, Letters used = a
______  Enter letter: e
Guess 2/10, Words Left 1154, Letters used = ae
____e_  Enter letter: i
Guess 3/10, Words Left 643, Letters used = aei
...
Word was confer