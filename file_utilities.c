#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utilities.h"

/**
 * All functions related to file input/output are implemented here.
 *
 * Owen Dunn and Aron Ockerse
 * 2/12/18
 */

/**
 *  Read the text from a file into a string. Return the size of the
 *  file read in bytes.
 *
 *  input: char* filename (string of file to read)
 *         char** buffer (double pointer of buffer to write to)
 *  return: int size (number of bytes read)
 */
int read_file( char* filename, char** buffer ) {

    FILE* fp = fopen(filename, "r"); // open file for input ops
    char* str = (char*)malloc(LINE_SIZE*sizeof(char));
    int size;

    if (fp == NULL) {
        printf("***Error: file does't exist or failed to open.\n");
        free(str);
        return -1;
    }

    // append each line to buffer
    while (fgets(str, LINE_SIZE, fp) != NULL) { // until eof
        //printf("%s\n", *buffer);
        //printf("%s\n", str);
        strcat(*buffer, str);
    }

    // find the size of the file in bytes (see source next line)
    // https://stackoverflow.com/questions/238603/ ...
    // how-can-i-get-a-files-size-in-c
    fseek(fp, 0L, SEEK_END); // point to the end of the file stream
    size = ftell(fp); // bytes from beginning to end of file stream

    fclose(fp); // close the file
    free(str); // release str memory

    return size;
}

/**
 *  This function writes a buffer to a file.
 *
 *  input: char* filename (file to create and write to)
 *         char* buffer (data to write to file)
 *         int size (amount of bytes to write to file)
 *  return: error code (0 if no problems)
 */
int write_file( char* filename, char* buffer, int size ) {

    FILE* fp = fopen(filename, "w");
    int k;

    if (fp == NULL) {
        printf("***Error: file failed to write.\n");
        return -1;
    }

  	for (k = 0; k < size; ++k) {
    		fwrite(buffer + k, 1, 1, fp);
  	}

    fclose(fp); // close the file

	return 0; // no error
}
