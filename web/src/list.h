#ifndef LIST_H
#define LIST_H

/*
 * List - doubly linked list type.
 * 
 * provided: pop_back, push_start so this list may be used as a queue.
 * NOTE: not intended as the building block for the concurrent queue.
 * 
 */

typedef struct ListStruct List;

/* pop_back 
 * 
 * Pop an element off the back of the list
 */

void *pop_back(List *stack);

/* push_start 
 * 
 * Create a new linked list
 */

void push_start(List *stack, void *data);


/* list_new 
 * 
 * Create a new linked list
 */

List *list_new();

/* list_free 
 * 
 * Free the linked list.
 * NOTE: user is responsible for freeing any 
 * contents of the list.
 * 
 */

void list_free(List *list);

#endif