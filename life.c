#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utilities.h"
#include "life_functions.h"
#include <unistd.h> // used for sleep(seconds) (Linux only)

/**
 * Game of Life: The game starts by loading a file to get the initial
 * state of the board. Before each generation passes and prints the
 * user will be given the option to save, load, continue another
 * generation, continue a certain number of generations, or quit.
 *
 * Owen Dunn
 * CIS 343, 2/12/18, Project 1
 */

int main(int argc, char** argv) {

    char* arr = (char*)malloc(ARRAY_SIZE*sizeof(char));
    int size = 0; // number of bytes of the loaded file
    struct boardState* pUniverse;
    char ch = ' ';
    int gens; // genertions
    int gensPassed = 0; // number of generations that have passed
    char* str = (char*)malloc(3*sizeof(char));
    char* token;
    int i = 0, j = 0, k = 0;
    char* filename = (char*)malloc(100*sizeof(char));

    // load text file to get init starting world via --load on shell
    if (argc > 1) { // if provided command line parameters
        if ( strcmp(argv[1], "--load") == 0 ) { // if loading a file

            // if reading successful
            if ( (size = read_file(argv[2], &arr)) >= 6 ) {

                // Create the board from the file.
                pUniverse = create_universe(arr);
            }
            else {
                printf("***Error: file must contain at least row, col");
                printf(", and one entry line (for 1x1 board)\n");
                free(arr);
                free(filename);
                free(str);
                return -1;
            }
        }
        else {
            printf("***Error: enter in format: \"<exe> --load ");
                printf("<filename>\"\n");
            free(arr);
            free(filename);
            free(str);
            return -1;
        }
    }
    else {
        printf("***Error: enter in format: \"<exe> --load ");
            printf("<filename>\"\n");
        free(arr);
        free(filename);
        free(str);
        return -1;
    }

    // print the board starting point
    printf(ANSI_COLOR_BLUE "***************Game of Life***************");
    printf("\n" ANSI_COLOR_RESET);
    printBoard(pUniverse);

    // give user options on what to do next - loop until quit
    printf(">");
    ch = getchar();
    while (ch != 'q') {
        switch (ch) {
            case 'c': // pass 1 generation
                gameOfLife(pUniverse); // updates board 1 generation
                printBoard(pUniverse);
                ++gensPassed; // another generation has passed
                printf("Generations: %d\n", gensPassed);
                break;

            case 'g': // pass >=1 generation
                printf("Enter Number of Generations (1-99): ");

                // retrieve data from input
                while ((ch = getchar()) != '\n' && ch != EOF);
                do {
                    fgets(str, LINE_SIZE, stdin);
                    token = strtok(str, " \n\t\0");
                    gens = atoi(token);
                    memset(str, 0, 3); // clear old data
                    if (gens < 1 || gens > 99 )
                        printf("Please enter 1 to 99 generations.\n");
                }while ( gens < 1 || gens > 99 );

                // run game 'gens' generations (with 1 s pause between)
                for (i = 0; i < gens; ++i) {
                    gameOfLife(pUniverse); // updates board 1 gen
                    printBoard(pUniverse);
                    ++gensPassed;
                    printf("Generations: %d\n", gensPassed);
                    sleep(1); // wait 1 second (Linux library function)
                }
                ch = 'g'; // used below (to not pause for unused input)
                break;

            case 's': // save board to a file
                printf("Please enter file name to create: ");
                memset(arr, 0, ARRAY_SIZE); // clear old file data
                memset(filename, 0, 100); // clear old filename
                getchar();
                fgets(filename, LINE_SIZE, stdin);
                filename = strtok(filename, " \n\t\0");

                // Write board to formatted string first.
                // lines: 1 for row and col, 1 for each cell
                // Create an array with same format as file.
                // -(later copied to file)
                char* temp =  (char*)malloc(10*sizeof(char));

                // add rows line
                sprintf(temp, "%d", (pUniverse -> row));
                strcat(temp, "\n");
                int lines = (int)strlen(temp); // lines in a string
                int pos = lines; // position in array
                //printf("%s, length=%d\n", temp, (int)strlen(temp));
                strcat(arr, temp);
                memset(temp, 0, lines);

                // add cols line
                sprintf(temp, "%d", (pUniverse -> col)); // int to str
                strcat(temp, "\n"); // add newline to string
                lines = (int)strlen(temp);
                pos += lines;
                strcat(arr, temp);
                free(temp);
                lines = (pUniverse -> row)*(pUniverse -> col);
                //printf("lines=%d\n", lines);

                // now add all cell values
                int a = 0, b = 0, w = 0;
                for (j = 0; j < (lines*2); ) {
                    arr[pos + j++] =
                        (pUniverse -> board[a][b++ % (pUniverse -> col)])
                        + '0';
                    arr[pos + j++] = '\n';
                    if ( (w++ % (pUniverse -> col) ) >=
                        ((pUniverse -> col) - 1) )
                        ++a;
                }
                arr[pos + j] = '\0';
                //printf("j=%d\n", j);
                //printf("%s", arr); // test
                if ( write_file( filename, arr, (lines+2)*2 ) != 0 ) {
                    printf("***Error: file not written.\n");
                    for (k = 0; k < (pUniverse -> row); ++k)
                        free(pUniverse -> board[k]);
                    free(pUniverse -> board);
                    free(pUniverse);
                    free(arr);
                    free(filename);
                    free(str);
                    return -1;
                }
                printf("Board saved to file \"%s\".\n", filename);
                ch = 's';
                break;

            case 'l': // load board from a file
                while ((ch = getchar()) != '\n' && ch != EOF);
                printf("Please enter file name to load: ");
                memset(arr, 0, ARRAY_SIZE); // clear old file data
                memset(filename, 0, 100); // clear old filename
                fgets(filename, LINE_SIZE, stdin); // includes newline ch
                filename = strtok(filename, " \n\t\0"); // rm newline ch
                if ( (size = read_file(filename, &arr)) >= 6 ) {
                    for (k = 0; k < (pUniverse -> row); ++k)
                        free(pUniverse -> board[k]);
                    free(pUniverse -> board);
                    free(pUniverse);
                    pUniverse = create_universe(arr); // load new state
                    printf("New universe loaded.\n");
                    gensPassed = 0; // reset counter
                    printBoard(pUniverse); // print new state
                }
                else {
                    printf("***Error: file must contain at least");
                        printf(" row, col, and one entry line");
                        printf(" (for 1x1 board)\n");
                    free(arr);
                    free(filename);
                    free(str);
                    for (k = 0; k < (pUniverse -> row); ++k)
                        free(pUniverse -> board[k]);
                    free(pUniverse -> board);
                    free(pUniverse);
                    return -1;
                }
                ch = 'l'; // used below
                break;

            case '\n': // ignore newline
                break;

            default:
                printf("Please enter a valid character.\n");
        }

        // Clear leftover input, source:
        // https://stackoverflow.com/questions/7898215/ ...
        // how-to-clear-input-buffer-in-c
        if (ch != 'g' && ch != 'l' && ch != 's')
            while ((ch = getchar()) != '\n' && ch != EOF);
        printf(">");
        //fflush(stdin); // doesn't seem to work to clear input
        ch = getchar();
    }

    free(str);
    free(arr);
    free(filename);
    for (k = 0; k < (pUniverse -> row); ++k)
        free(pUniverse -> board[k]);
    free(pUniverse -> board);
    free(pUniverse); // free memory block

    return 0;
}
