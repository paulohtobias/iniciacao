#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_DEFAULT_MAX_SIZE
#define LIST_DEFAULT_MAX_SIZE 300
#endif

typedef struct ArrayList{
	int first;
	int last;
	int length;
	void **data;

	int max_size;
} ArrayList;

ArrayList *new_ArrayList();

ArrayList *new_ArrayList_max_size(int max);

void free_ArrayList(ArrayList *list, void (*free_data)(void *));

int arraylist_is_empty(ArrayList *list);

void arraylist_insert_first(ArrayList *list, void *data);

void arraylist_insert_last(ArrayList *list, void *data);

void *arraylist_get_first(ArrayList *list);

void *arraylist_get_last(ArrayList *list);

void *arraylist_get_index(ArrayList *list, int index);

void *arraylist_remove_first(ArrayList *list);

void *arraylist_remove_last(ArrayList *list);

void *arraylist_remove_index(ArrayList *list, int index);

#endif //ARRAY_LIST_H
