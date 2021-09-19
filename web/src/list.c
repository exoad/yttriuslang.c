
#include <stdlib.h>
#include "list.h"

typedef struct LinkStruct Link;

typedef struct LinkStruct {
  void *data;
  
  Link *prev;
  Link *next;
  
} Link;


typedef struct ListStruct {
  Link *start;
  Link *end;
  
} List;

/* list_new 
 * 
 * Create a new linked list
 */

List *list_new() {
  
  List *list = (List*)malloc(sizeof(List));
  list->start = list->end = NULL;
  return list;
}


void list_free(List *list) {
  free(list);
}

/* push_start 
 * 
 * Push an element onto the start of the linked list
 */

void push_start(List *list, void *data) {
  
  Link *link = (Link*)malloc(sizeof(Link));
  link->data = data;
  
  if(!list->start) {
    list->start = list->end = link;
    link->next = link->prev = NULL;
    
  } else {
      
    link->next = list->start;
    list->start->prev = link;
    link->prev = NULL;
    
    list->start = link;
  }
}

/* pop_back 
 * 
 * Pop an element off the back of the list
 */

void *pop_back(List *list) {
  
  Link *link = list->end;
  
  if(link == NULL)
    return NULL;
  
  if(list->end == list->start) {
    list->end = list->start = NULL; 
  } else {
    list->end = list->end->prev;
    list->end->next = NULL;
  }
  
  void *data = link->data;
  
  free(link);
  return data;
}


/* pop_start 
 * 
 * Pop an element off the start of the linked list
 * NOTE: this is for you to implement
 * 
 */

void *pop_start(List *list) {
  Link *link = list->start;
  
  if(link == NULL)
    return NULL;
  
  if(list->end == list->start) {
    list->end = list->start = NULL; 
  } else {
    list->start = list->start->next;
    list->start->prev = NULL;
  }
  
  void *data = link->data;
  
  free(link);
  return data;
  
  return NULL;
}
