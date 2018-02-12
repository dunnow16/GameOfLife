#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "life_functions.h"

/**
 * All functions related to the life game are implemented here.
 *
 * Owen Dunn
 * 2/12/18
 */

struct boardState;

/**
 *  This function updates the state of the board one generation. The
 *  update is written to the board within the structure.
 *
 *  input: struct boardState*
 *  return: none
 *
 *  Based on source: 2/10/18, by Skanda Bhargav
 *  https://ide.geeksforgeeks.org/iVA9hRlmD8
 */
void gameOfLife(struct boardState* pUniverse) {

    int neighLiveCount = 0;
    int i = 0, j = 0, p = 0, q = 0, a = 0, b = 0;

    // used to store intermediate results to not lose data
    char boardCopy[pUniverse -> row][pUniverse -> col];

    // run the game logic to find new cell values
    for (i = 0; i < (pUniverse -> row); ++i) { // each row
        for (j = 0; j < (pUniverse -> col); ++j) { // each col
            neighLiveCount = 0; // reset count
            for (p = -1; p <= 1; ++p) { // row-offset (-1,0,1)
                for (q = -1; q <= 1; ++q) { // col-offset (-1,0,1)
                    // if row offset less than upper boundary
                    if ( ( (i + p) < 0 )  ||
                        // if row offset more than lower boundary
                        ( (i + p) > ((pUniverse -> row) - 1) ) ||
                        // if col offset less than left boundary
                        ( (j + q) < 0 ) ||
                        // if col offset more than right boundary
                        ( (j + q) > ((pUniverse -> col) - 1) ) )
                        // skip the neighbor (if not, then out of bounds)
                        continue;

                    // increment live counter for each live neighbor
                    neighLiveCount +=
                        (pUniverse -> board[i + p][j + q]);
                }
            }

            // remove self count
            neighLiveCount -= (pUniverse -> board[i][j]);

            switch (neighLiveCount) {
                case 0:
                case 1: // less than 2 neigh alive, set to dead
                    boardCopy[i][j] = 0;
                    break;
                case 2: // exactly 2 live neighbors, state unchanged
                    boardCopy[i][j] = (pUniverse -> board[i][j]);
                    break;
                case 3: // exactly 3 live neighbors, set to alive
                    boardCopy[i][j] = 1;
                    break;
                default: // more than 3 neighbors alive, set to dead
                    boardCopy[i][j] = 0;
            }
        }
    } // end of neighbor check for all cells

    // copy boardCopy data into universe data
    for (a = 0; a < (pUniverse -> row); ++a)
        for (b = 0; b < (pUniverse -> col); ++b)
            (pUniverse -> board[a][b]) = boardCopy[a][b];
}

/**
 *  This function prints the board as stored in a structure.
 *
 *  input: struct boardState*
 *  return: none
 */
void printBoard(struct boardState* pUniverse) {
    int i = 0, j = 0;

    printf("Enter 's' to save state, 'l' to load state\n");
    printf("-'c' to continue another generation\n");
    printf("-'g' to continue some number generations\n");
	printf("-'q' to quit\n");

    for (i = 0; i < (pUniverse -> row); ++i) {
        for (j = 0; j < (pUniverse -> col); ++j) {
            if (pUniverse -> board[i][j] == 0) { // dead
                printf(ANSI_COLOR_WHITE "* " ANSI_COLOR_RESET);
            }
            else { // alive
                printf(ANSI_COLOR_BLUE "* " ANSI_COLOR_RESET);
            }
        }
        printf("\n");
    }
}

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
struct boardState* create_universe(char* buffer) {
    int i = 0, j = 0;
    int r = 0;
    char* token; // used to store data from file string

    // pointer to structure of board state
    struct boardState* pUniverse =
        (struct boardState*)malloc(sizeof(struct boardState));

    // Read one line at a time, converting string to int
    // -assumes string is formatted properly
    token = strtok(buffer, " \n\t\0");
    while (token != NULL) {
        if (i < 2) { // first 2 lines have row, then column
            if (i == 0) {
                (pUniverse -> row) = atoi(token);
            }
            else {
                (pUniverse -> col) = atoi(token);
                // now create the 2D array
                pUniverse -> board =
                    (char**)malloc((pUniverse -> row) * sizeof(char*));
                for (j = 0; j < (pUniverse -> row); ++j)
                    pUniverse -> board[j] =
                        (char*)malloc((pUniverse -> col) * sizeof(char));
                //printf("row=%d col=%d\n", row, col); // test
            }
        }
        else {
            // write all cell values to the 2D array (buffer)
            //printf("r before: %d, i=%d,  mod: %d \t",
                     //r, i, (i - 2) % col);
            //printf("r: %d\n", r); // test
            pUniverse -> board[ r ][ (i - 2) % (pUniverse -> col) ] =
                atoi(token);

            // update row value every 'col' entries
            if ( ( (i - 2) % (pUniverse -> col) ) >=
                ( (pUniverse -> col) - 1 ) )
                ++r;
        }
        //printf("%s\n", token); // test
        token = strtok(NULL, " \n\t\0"); // read next entry
        ++i;
    }

    return pUniverse;
}
