#include "queue.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

typedef struct LinkStruct Link;
typedef struct LinkStruct {
	void *data;  
	Link *prev;
	Link *next;
} Link;

typedef struct QueueStruct {
	Link *start;
	Link *end;
	sem_t add;
	sem_t remove;
	pthread_mutex_t threads;
	int length;
} Queue;

Queue* queue_alloc(int size) {
	Queue* queue = malloc(sizeof(Queue));
	sem_init(&queue -> add, 0, size);
	sem_init(&queue -> remove, 0, 0);
	pthread_mutex_t init = PTHREAD_MUTEX_INITIALIZER;
	queue -> threads = init;
	queue -> start = queue -> end = NULL;
	queue -> length = size;
	return queue;
}

void queue_free(Queue *queue) {
	free(queue);
}

void queue_put(Queue *queue, void *item) {
	sem_wait(&queue -> add);
	pthread_mutex_lock(&queue -> threads);  
    Link* link = (Link*)malloc(sizeof(Link));
    link -> data = item;
    if(!queue -> start) {
		queue -> start = queue -> end = link;
		link -> next = link -> prev = NULL;
    }
    else {      
		link -> next = queue -> start;
		queue -> start -> prev = link;
		link -> prev = NULL;
		queue -> start = link;
    }
    pthread_mutex_unlock(&queue -> threads);
    sem_post(&queue -> remove);
  }

void* queue_get(Queue* queue) {
	sem_wait(&queue -> remove);
	pthread_mutex_lock(&queue -> threads);
	Link *link = queue -> end;
	void *Result = link -> data;
	if(link == NULL) return NULL;
	if(queue -> end == queue -> start){
		queue -> end = queue -> start = NULL;
	}
	else{
		queue -> end = queue -> end -> prev;
		queue -> end -> next = NULL;
	}
	pthread_mutex_unlock(&queue -> threads);
	sem_post(&queue -> add); free(link);
	return Result;
}
