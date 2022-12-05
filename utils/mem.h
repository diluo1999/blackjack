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
 * 3. A generic 'assert' function that checks a given pointer and,
 *    if NULL, kicks out an error and exits; otherwise it passes
 *    the pointer through to its return value.  Useful in code
 *    that needs to defensively check function parameters that
 *    "should never be NULL".
 *
 * David Kotz, April 2016, 2017, 2019, 2021
 * CS 50, Fall 2022
 */

#ifndef __MEM_H
#define __MEM_H

#include <stdio.h>
#include <stdlib.h>

/**************** mem_assert **************************/
/* If pointer p is NULL, print error message to stderr and die,
 * otherwise, return p unchanged.  Works nicely as a pass-through:
 *   thingp = mem_assert(malloc(sizeof(thing)), "thing");
 * We assume:
 *   caller provides a message string suitable for printf.
 * We return:
 *   the value of p
 * We exit if p=NULL, after printing message to stderr.
 */
void* mem_assert(void* p, const char* message);

/**************** mem_assert_const **************************/
/* Identical to mem_assert, but suitable for p declared const.
 */
const void* mem_assert_const(const void* p, const char* message);

/**************** mem_malloc_assert() ****************/
/* Like malloc() but track the number of successful allocations,
 * and, if response is NULL, print error and die.
 * Unlike malloc(), it takes a 'message' parameter.
 * We assume:
 *   caller provides a message string suitable for printf.
 * We return:
 *   the pointer produced by malloc.
 * We exit if any error, after printing to stderr.
 */
void* mem_malloc_assert(const size_t size, const char* message);

/**************** mem_malloc() ****************/
/* Just like malloc() but track the number of successful allocations
 * Caller provides:
 *   the size of desired allocation, as in malloc().
 * We return
 *   pointer to allocated space, or NULL if failure.
 * We track the number of calls - see mem_net().
 */
void* mem_malloc(const size_t size);

/**************** mem_calloc_assert() ****************/
/* Just like calloc() but track the number of successful allocations
 * and, if response is NULL, print error and die.
 * Unlike calloc(), it takes a 'message' parameter.
 * We assume:
 *   caller provides a message string suitable for printf.
 * We return:
 *   the pointer produced by calloc.
 * We exit if any error, after printing to stderr.
 */
void* mem_calloc_assert(const size_t nmemb, const size_t size, 
                          const char* message);

/**************** mem_calloc() ****************/
/* Just like calloc() but track the number of successful allocations.
 * Caller provides:
 *   the number and size of desired allocation, as in calloc().
 * We return
 *   pointer to allocated space, or NULL if failure.
 * We track the number of calls - see mem_net().
 */
void* mem_calloc(const size_t nmemb, const size_t size);

/**************** mem_free() ****************/
/* Just like free() but track the number of calls.
 * We assume:
 *   caller provides pointer to space produced by mem_malloc or mem_calloc.
 * We track the number of calls - see mem_net().
 */
void mem_free(void* ptr);

/**************** mem_report() ****************/
/* Print a report of the current malloc/free counts.
 * We assume:
 *   caller provides a FILE open for writing, and message suitable for printf.
 * We format and print a report to that FILE, indicating the number of calls
 * to mem_malloc/calloc and of calls to mem_free, and the net difference.
 */
void mem_report(FILE* fp, const char* message);

/**************** mem_net() ****************/
/* Return the current net malloc-free counts.
 * We assume:
 *   caller has been using mem_malloc/calloc and mem_free. 
 * We return:
 *   returns positive if there are unfreed allocations,
 *   returns negative if there were more free's than alloc's (!),
 *   returns zero if they balance.
 */
int mem_net(void);

#endif // __MEM_H
