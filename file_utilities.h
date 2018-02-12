#ifndef FILEUTIL_FUNCTIONS
#define FILEUTIL_FUNCTIONS

/**
 * All functions related to file input/output.
 *
 * Owen Dunn and Aron Ockerse
 * 2/12/18
 */

#define LINE_SIZE  100
#define ARRAY_SIZE 4096

/**
 *  Read the text from a file into a string. Return the size of the
 *  file read in bytes.
 *
 *  input: char* filename (string of file to read)
 *         char** buffer (double pointer of buffer to write to)
 *  return: int size (number of bytes read)
 */
int read_file( char* filename, char** buffer );

/**
 *  This function writes a buffer to a file.
 *
 *  input: char* filename (file to create and write to)
 *         char* buffer (data to write to file)
 *         int size (amount of bytes to write to file)
 *  return: error code (0 if no problems)
 */
int write_file( char* filename, char* buffer, int size );

#endif
