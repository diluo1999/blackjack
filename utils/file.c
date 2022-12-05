/* 
 * file utilities - reading a word, line, or entire file
 * 
 * See file.h for documentation.
 * 
 * David Kotz - 2016, 2017, 2019, 2021
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "file.h"


/**************** file_numLines ****************/
int
file_numLines(FILE* fp)
{
  if (fp == NULL) {
    return 0;
  }

  rewind(fp);

  int nlines = 0;
  char c = '\0';
  while ( (c = fgetc(fp)) != EOF) {
    if (c == '\n') {
      nlines++;
    }
  }

  rewind(fp);
  
  return nlines;
}

/**************** utility stopfuncs ****************/
// for use with readuntil()
static int never(int c) { return (0); }
static int isnewline(int c) { return (c == '\n'); }

/**************** file_readFile ****************/
/* See file.h for documentation. */
char* file_readFile(FILE* fp) { return file_readUntil(fp, never); }

/**************** file_readLine ****************/
/* See file.h for documentation. */
char* file_readLine(FILE* fp) { return file_readUntil(fp, isnewline); }

/**************** readword ****************/
/* See file.h for documentation. */
char* file_readWord(FILE* fp) { return file_readUntil(fp, isspace); }

/**************** readuntil ****************/
/* See file.h for documentation. */
char* 
file_readUntil(FILE* fp, int (*stopfunc)(int c))
{
  if (stopfunc == NULL) {
    stopfunc = never;
  }

  // allocate buffer big enough for "typical" words/lines
  int len = 81;
  char* buf = malloc(len * sizeof(char));
  if (buf == NULL) {
    return NULL;
  }

  // Read characters from file until stop-character or EOF, 
  // expanding the buffer when needed to hold more.
  int pos;
  char c;
  for (pos = 0; (c = fgetc(fp)) != EOF && !(*stopfunc)(c); pos++) {
    // We need to save buf[pos+1] for the terminating null
    // and buf[len-1] is the last usable slot, 
    // so if pos+1 is past that slot, we need to grow the buffer.
    if (pos+1 > len-1) {
      char* newbuf = realloc(buf, ++len * sizeof(char));
      if (newbuf == NULL) {
        free(buf);
        return NULL;
      } else {
        buf = newbuf;
      }
    }
    buf[pos] = c;
  }

  if (pos == 0 && c == EOF) {
    // no characters were read and we reached EOF
    free(buf);
    return NULL;
  } else {
    // pos characters were read into buf[0]..buf[pos-1].
    buf[pos] = '\0'; // terminate string
    return buf;
  }
}

/* ********************************************************** */
/* a simple unit test of the code above */
#ifdef QUICKTEST
#include <stdbool.h>

bool testwords = false;       // whether to test file_readWord()
bool testlines = false;       // whether to test file_readLine()
bool testfile = true ;        // whether to test file_readFile()

int main(int argc, char* argv[])
{
  if (argc != 2) 
    exit(1);

  FILE* fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("can't open %s\n", argv[1]);
    exit(2);
  }

  if (testwords) {
    rewind(fp);
    char* word;
    while ( (word = file_readWord(fp)) != NULL) {
      printf("[%s] ", word);
      free (word);
    }
  } 

  if (testlines) {
    rewind(fp);
    char* line;
    while ( (line = file_readLine(fp)) != NULL) {
      printf("[%s]\n", line);
      free (line);
    }
  }

  if (testfile) {
    rewind(fp);
    char* file = file_readFile(fp);
    if (file != NULL) {
      printf("[%s]", file);
      free (file);
    }
  }
}
#endif
