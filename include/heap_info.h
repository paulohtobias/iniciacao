/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   heap_info.h
 * Author: fernanda
 *
 * Created on January 31, 2017, 12:57 PM
 */

#ifndef HEAP_INFO_H
#define HEAP_INFO_H

#ifdef __cplusplus
extern "C" {
#endif


/*** heap_info.h - Defines the universal heap_info_t data structure which each
 *** heap can provide, and algorithms can use.  An algorithm which uses the
 *** universal definition of a heap data structure can use different heaps
 *** interchangeably for testing or comparison purposes.
 ***/
/*
 *  Shane Saunders
 */

/* Structure to be provided by heaps and used by algorithms. */
typedef struct heap_info {
    int (*delete_min)(void *heap);
    void (*insert)(void *heap, int node, double key);
    void (*decrease_key)(void *heap, int node, double newkey);
    int (*n)(void *heap);
    double (*key_comps)(void *heap);
    void *(*alloc)(int n_items);
    void (*free)(void *heap);
    void (*dump)(void *heap);
} heap_info_t;


#ifdef __cplusplus
}
#endif

#endif /* HEAP_INFO_H */

