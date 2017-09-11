/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bheap.h
 * Author: fernanda
 *
 * Created on January 31, 2017, 12:56 PM
 */

#ifndef BHEAP_H
#define BHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

/*** Header File for the Binary Heap Implementation ***/
/*
 *   Shane Saunders
 */
#include "heap_info.h"  /* Defines the universal heap structure type. */

/* This implementation stores the binary heap in a 1 dimensional array. */


/*** Structure Definitions ***/

/* Frontier set items in Dijkstra's algorithm stored in the binary heap
 * structure.
 */
typedef struct {
    int item;  /* vertex number is used for the item. */
    double key;  /* distance is used as the key. */
} bheap_item_t;

/* Binary heap structure for frontier set in Dijkstra's algorithm.
 * a[] - stores (distance, vertex) pairs of the binary heap.
 * p[] - stores the positions of vertices in the binary heap a[].
 * n - is the size of the binary heap.
 */
typedef struct {
    bheap_item_t *a;
    int *p;
    int n;
    double key_comps;
} bheap_t;


/*** Function prototypes. ***/

/* Binary heap functions. */

/* bh_alloc() allocates space for a binary heap of size n and initialises it.
 * Returns a pointer to the binary heap.
 */
bheap_t *bh_alloc(int n);

/* bh_free() frees the space taken up by the binary heap pointed to by h.
 */
void bh_free(bheap_t *h);

/* bh_min() returns the item with the minimum key in the binary heap pointed to
 * by h.
 */
int bh_min(bheap_t *h);

/* bh_insert() inserts an item and its key value into the binary heap pointed
 * to by h.
 */
void bh_insert(bheap_t *h, int item, double key);

/* bh_delete() deletes an item from the binary heap pointed to by h.
 */
void bh_delete(bheap_t *h, int item);

/* bh_empty() returns if the heap is empty.
 */
int bh_empty(bheap_t *h);

/* bh_decrease_key() decreases the value of 'item's key and then performs
 * sift-down until 'item' has been relocated to the correct position in the
 * binary heap.
 */
void bh_decrease_key(bheap_t *h, int item, double new_key);

void bh_dump(bheap_t *h);

/*** Alternative interface via the universal heap structure type. ***/
extern const heap_info_t BHEAP_info;


#ifdef __cplusplus
}
#endif

#endif /* BHEAP_H */

