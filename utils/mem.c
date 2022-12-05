/* 
 * memory - mem_malloc and related functions 
 * 
 * 1. Replacements for malloc(), calloc(), and free(),
 *    that count the number of calls to each,
 *    so you can print reports about the current balance of memory.
 * 
 * 2. Variants that 'assert' the result is non-NULL;
 *    if NULL occurs, kick out an error and die.
 *
 * David Kotz, April 2016, 2017, 2019, 2021
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include "mem.h"

/**************** file-local global variables ****************/
// track malloc and free across *all* calls within this program.
static int nmalloc = 0;         // number of successful malloc calls
static int nfree = 0;           // number of free calls
static int nfreenull = 0;       // number of free(NULL) calls


/**************** mem_assert ****************/
/* see mem.h for description */
void*
mem_assert(void* ptr, const char* message)
{
  if (ptr == NULL) {
    fprintf(stderr, "NULL POINTER: %s\n", message);
    exit (99);
  }
  return ptr;
}

/**************** mem_assert_const ****************/
/* see mem.h for description */
const void*
mem_assert_const(const void* ptr, const char* message)
{
  if (ptr == NULL) {
    fprintf(stderr, "NULL POINTER: %s\n", message);
    exit (99);
  }
  return ptr;
}

/**************** mem_malloc_assert() ****************/
/* see mem.h for description */
void*
mem_malloc_assert(const size_t size, const char* message)
{
  void* ptr = malloc(size);
  if (ptr == NULL) {
    fprintf(stderr, "Out of memory: %s\n", message);
    exit (99);
  }
  nmalloc++;
  return ptr;
}


/**************** mem_malloc() ****************/
/* see mem.h for description */
void*
mem_malloc(const size_t size)
{
  void* ptr = malloc(size);
  if (ptr != NULL) {
    nmalloc++;
  }
  return ptr;
}

/**************** mem_calloc_assert() ****************/
/* see mem.h for description */
void*
mem_calloc_assert(const size_t nmemb, const size_t size, const char* message)
{
  void* ptr = mem_assert(calloc(nmemb, size), message);
  nmalloc++;
  return ptr;
}

/**************** mem_calloc() ****************/
/* see mem.h for description */
void*
mem_calloc(const size_t nmemb, const size_t size)
{
  void* ptr = calloc(nmemb, size);
  if (ptr != NULL) {
    nmalloc++;
  }
  return ptr;
}

/**************** mem_free() ****************/
/* see mem.h for description */
void 
mem_free(void* ptr)
{
  if (ptr != NULL) {
    free(ptr);
    nfree++;
  } else {
    // it's an error to call free(NULL)!
    nfreenull++;
  }
}

/**************** mem_report() ****************/
/* see mem.h for description */
void 
mem_report(FILE* fp, const char* message)
{
  fprintf(fp, "%s: %d malloc, %d free, %d free(NULL), %d net\n", 
          message, nmalloc, nfree, nfreenull, nmalloc - nfree - nfreenull);
}

/**************** mem_net() ****************/
/* see mem.h for description */
int
mem_net(void)
{
  return nmalloc - nfree - nfreenull;
}
