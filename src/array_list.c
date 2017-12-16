#include "array_list.h"

ArrayList *new_ArrayList(){
	return new_ArrayList_max_size(LIST_DEFAULT_MAX_SIZE);
}

ArrayList *new_ArrayList_max_size(int max){
	ArrayList *list = malloc(sizeof(ArrayList));
	if(list == NULL){
		printf("ArrayList *list = malloc(sizeof(ArrayList));\n");
		exit(1);
	}

	list->length = 0;
	list->first = 0;
	list->last = -1;
	list->data = malloc(max * sizeof(void *));
	if(list->data == NULL){
		printf("list->data = malloc(max * sizeof(void *));\n");
		exit(1);
	}

	int i;
	for(i = 0; i < max; i++){
		list->data[i] = NULL;
	}
	list->max_size = max;

	return list;
}

void free_ArrayList(ArrayList *list, void (*free_data)(void *)){
	if(arraylist_is_empty(list)){
		return;
	}

	int i;
	for(i = list->first; i < list->length; i++){
		if(list->data[i] != NULL){
			free_data(list->data[i]);
			list->data[i] = NULL;
		}
	}
	free(list->data);
	list->data = NULL;
	list->first = 0;
	list->last = 0;
	list->length = 0;

	free(list);
}

int arraylist_is_empty(ArrayList *list){
	return(list->length == 0);
}

void arraylist_insert_first(ArrayList *list, void *data){
	//This whole if-else section consists in finding where to put the object
	if(!arraylist_is_empty(list)){
		if(list->first == 0){
			int i;
			//Looking for a free space.
			for(i = list->first + 1; i < list->max_size && list->data[i] != NULL; i++);

			if(i == list->max_size){
				//printf("ArrayLista cheia!!!!\n");
				list->max_size *= 2;
				list->data = realloc(list->data, list->max_size * sizeof(void *));
			}

			int j;
			for(j = i - 1; j >= list->first; j--){
				list->data[j + 1] = list->data[j];
			}
			list->first++;
		}
		list->first--;
	}

	list->data[list->first] = data;
	list->length++;
}

void arraylist_insert_last(ArrayList *list, void *data){
	if(!arraylist_is_empty(list)){
		if(list->last + 1 == list->max_size){
			//printf("ArrayList cheia!!!!\n");
			list->max_size *= 2;
			list->data = realloc(list->data, list->max_size * sizeof(void *));
		}
	}
	
	list->last++;
	list->data[list->last] = data;
	list->length++;
}

void *arraylist_get_first(ArrayList *list){
	return arraylist_get_index(list, 0);
}

void *arraylist_get_last(ArrayList *list){
	return arraylist_get_index(list, list->length - 1);
}

void *arraylist_get_index(ArrayList *list, int index){
	index += list->first;
	if(arraylist_is_empty(list) || index > list->last){
		return NULL;
	}

	return list->data[index];
}

void *arraylist_remove_first(ArrayList *list){
	void *data = NULL;

	if(!arraylist_is_empty(list)){
		data = list->data[list->first];
		list->data[list->first] = NULL;
		list->first++;
		list->length--;
	}
	return data;
}

void *arraylist_remove_last(ArrayList *list){
	void *data = NULL;

	if(!arraylist_is_empty(list)){
		data = list->data[list->last];
		list->data[list->last] = NULL;
		list->last--;
		list->length--;
	}
	return data;
}

void *arraylist_remove_index(ArrayList *list, int index){
	index += list->first;
	if(arraylist_is_empty(list) || index > list->last){
		return NULL;
	}
	
	int i;
	void *temp = list->data[index];
	
	for(i = index; i < list->last; i++){
		list->data[i] = list->data[i+1];
	}
	
	list->last--;
	list->length--;
	
	return temp;
}
