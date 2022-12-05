/* 
 * file utilities - reading a word, line, or entire file
 * 
 * David Kotz, 2016, 2017, 2019, 2021
 * CS 50, Fall 2022
 */

#ifndef __FILE_H
#define __FILE_H

#include <stdio.h>

/**************** file_numLines ****************/
/* Returns the number of lines in the given file,
 * i.e., the number of newlines in the file.
 * (If the file does not end with a newline, it will undercount by one.)
 * On return, the file pointer is back to beginning of file.
 * This will not work for pipes or stdin when it is attached to keyboard.
 */
int file_numLines(FILE* fp);

/**************** readuntil ****************/
/* 
 * Read characters from the file into a null-terminated string,
 * and return a pointer to it; caller must later free() the pointer.
 * Reading continues until 'stop' function returns non-zero or until EOF.
 * The character triggering 'stop' is discarded.
 * The stopfunc may be NULL, in which case it is considered always 0.
 * Returns empty string if stop occurs on the first character encountered.
 * Returns NULL if error, or EOF reached without reading anything.
 */
char* file_readUntil(FILE* fp,    int (*stopfunc)(int c) );

/**************** file_readFile ****************/
/* 
 * Read remainder of the file into a null-terminated string,
 * and return a pointer to it; caller must later free() the pointer.
 * Returns NULL if error, or if EOF reached without reading anything.
 * After the call, file pointer is at EOF.
 */
char* file_readFile(FILE* fp);

/**************** file_readLine ****************/
/* 
 * Read a line from the file into a null-terminated string,
 * and return a pointer to it; caller must later free() the pointer.
 * The string returned includes NO newline, and a terminating null.
 * Returns empty string if an empty line is read.
 * Returns NULL if error, or EOF reached without reading a line.
 */
char* file_readLine(FILE* fp);

/**************** file_readWord ****************/
/* 
 * Read a word from the file into a null-terminated string,
 * and return a pointer to it; caller must later free() the pointer.
 * A word is a sequence of non-whitespace characters; the first space after the word is consumed.
 * The string returned includes NO space, and a terminating null.
 * Returns empty string if the first character encountered is space or newline.
 * Returns NULL if error, or EOF reached without reading a word.
 */
char* file_readWord(FILE* fp);

#endif // __FILE_H
