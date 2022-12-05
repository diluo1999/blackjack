/* 
 * bag.h - header file for CS50 'bag' module
 * 
 * A 'bag' is a collection of indistinguishable items; it starts empty, 
 * grows as the caller adds one item at a time, and shrinks as the caller 
 * extracts one item at a time. It could be empty, or could contain hundreds
 * of items. Since items are indistinguishable, the module is free to return
 * any item from the bag. 
 *
 * David Kotz, April 2016, 2017, 2019, 2021
 * CS 50, Fall 2022
 */

#ifndef __BAG_H
#define __BAG_H

#include <stdio.h>

/**************** global types ****************/
typedef struct bag bag_t;  // opaque to users of the module

/**************** functions ****************/

/**************** bag_new ****************/
/* Create a new (empty) bag.
 *
 * We return:
 *   pointer to a new bag, or NULL if error.
 * We guarantee:
 *   The bag is initialized empty.
 * Caller is responsible for:
 *   later calling bag_delete.
 */
bag_t* bag_new(void);

/**************** bag_insert ****************/
/* Add new item to the bag.
 *
 * Caller provides:
 *   a valid bag pointer and a valid item pointer.
 * We guarantee:
 *   a NULL bag is ignored; a NULL item is ignored.
 *   the item is added to the bag.
 * Caller is responsible for:
 *   not free-ing the item as long as it remains in the bag.
 */
void bag_insert(bag_t* bag, void* item);

/**************** bag_extract ****************/
/* Return any data item from the bag.
 *
 * Caller provides:
 *   valid bag pointer.
 * We return:
 *   return pointer to an item, or NULL if bag is NULL or empty.
 * We guarantee:
 *   the item is no longer in the bag.
 * Caller is responsible for:
 *   free-ing the item if it was originally allocated with malloc.
 * Note:
 *   the order in which items are removed from the bag is undefined.
 */
void* bag_extract(bag_t* bag);

/**************** bag_print ****************/
/* Print the whole bag. 
 *
 * Caller provides:
 *   a FILE open for writing, and a function to print items.
 * We guarantee:
 *   If fp==NULL; do nothing. If bag==NULL, print (null). 
 *   If itemprint==NULL, print nothing for each item.
 * We print: 
 *   A comma-separated list of items, surrounded by {brackets}.
 */
void bag_print(bag_t* bag, FILE* fp, 
               void (*itemprint)(FILE* fp, void* item));

/**************** bag_iterate ****************/
/* Iterate over the whole bag.
 * 
 * Caller provides
 *   valid bag and a function to handle each item.
 * We guarantee:
 *   nothing, if NULL bag or NULL itemfunc.
 *   otherwise, call the given function once for each item, with (arg, item).
 * Notes:
 *   the void*arg is an arbitrary pointer, passed along to the itemfunc.
 *   the order in which bag items are handled is undefined.
 *   the bag is unchanged by this operation.
 */
void bag_iterate(bag_t* bag, void* arg,
                 void (*itemfunc)(void* arg, void* item) );

/**************** bag_delete ****************/
/* Delete the whole bag.
 *
 * Caller provides:
 *   a valid bag pointer.
 *   a function that will delete one item (may be NULL).
 * We guarantee:
 *   we call itemdelete() on each item.
 *   we ignore NULL bag.
 * Notes:
 *   the order in which items are deleted is undefined.
 */
void bag_delete(bag_t* bag, void (*itemdelete)(void* item) );

#endif // __BAG_H
