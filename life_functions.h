#ifndef LIFE_GAME_FUNCTIONS
#define LIFE_GAME_FUNCTIONS

/**
 * All functions related to the life game.
 *
 * Owen Dunn and Aron Ockerse
 * 2/12/18
 */

#define ARRAY_SIZE       4096
#define ASCII_SQUARE     254
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_COLOR_BLUE  "\x1b[34m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

/**
 * Defines the state of the board for life game.
 */
struct boardState{
    char** board;    /* used to create dynamic 2D array */
    int row, col;    /* rows, columns of board          */
};

/**
 *  This function updates the state of the board one generation. The
 *  update is written to the board within the structure.
 *
 *  input: struct boardState
 *  return: none
 *
 *  Based on source: 2/10/18, by Skanda Bhargav
 *  https://ide.geeksforgeeks.org/iVA9hRlmD8
 */
void gameOfLife(struct boardState* pUniverse);

/**
 *  Read the rows and columns from a file entered from the command line.
 *  A buffer is provided with the data of the file.
 *
 *  File format: first two lines are the rows and columns. The remaining
 *  lines are all cell values. All lines end with a newline character.
 *  A pointer to a structure containing the board, rows, and columns is
 *  returned.
 *
 *  input: char* buffer (file in format needed to create game board)
 *  return: struct boardState* (new created board state struct)
 */
struct boardState* create_universe(char* buffer);

/**
 *  This function prints the board as stored in a structure.
 *
 *  input: struct boardState*
 *  return: none
 */
void printBoard(struct boardState* pUniverse);

#endif
