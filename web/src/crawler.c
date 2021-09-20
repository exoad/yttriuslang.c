#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "html.h"
#include "http.h"
#include "url.h"
#include "list.h"
#include "queue.h"

#define BUF_SIZE 1024

typedef struct LinkStruct Link;
typedef struct LinkStruct {
	void *data;	Link *prev;	Link *next;  
} Link;

typedef struct ListStruct {
	Link *start; Link *end;  
} List;

typedef struct {
	char path[BUF_SIZE]; char url[BUF_SIZE];
	int depth;
} Path;

typedef struct QueueStruct {
	Link *start; Link *end;
	sem_t add; sem_t remove;
	pthread_mutex_t full;
	int length;
} Queue;

typedef struct ContextStruct Context;
typedef struct ContextStruct {
	Queue* http_queue; Queue* html_queue;
	int depth;
} Context;

/*	 url_filename:
 *  Convert a url into a filename for saving to local disk.
 *  e.g. http://www.cosc.canterbury.ac.nz/dept/viscom.shtml  ->  www.cosc.canterbury.ac.nz/dept|viscom.shtml
 */
void create_directory(char* host){
	struct stat dirs = {0};
	if (stat(host, &dirs) == -1) mkdir(host, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void url_filename(char* buffer, int size, char* url) { 
	char path[BUF_SIZE]; char host[BUF_SIZE];
	get_path(path, BUF_SIZE, url);
	get_host(host, BUF_SIZE, url);
	if(*path == '\0') strncpy(path, "/", BUF_SIZE);
	char *c = path;
	while(*c != '\0') {
		if(*c == '/') *c = '|'; 
		++c;
	}
	snprintf(buffer, size, "%s/%s", host, path);
}   // Lines 12

void add_page(List* list, char* url, int depth){
	char buf[BUF_SIZE]; char path[BUF_SIZE];
	if( url[0] != 'h') add_scheme(url, BUF_SIZE, url);
	url_filename(buf, BUF_SIZE, url); get_path(path, BUF_SIZE, url);
	Path *path_struct = (Path*)malloc(sizeof(Path)); path_struct -> depth = depth;
	strncpy(path_struct -> path, path, BUF_SIZE);
	strncpy(path_struct -> url, url, BUF_SIZE);
	push_start(list, path_struct);
}   // Line 8

void* Threads(void* arg){
	Context* context = (Context* )arg;
	Queue* html_queue = context -> html_queue;
	Queue* http_queue = context -> http_queue;
	Path* path = queue_get(http_queue);
	int depth = path -> depth;
	while (depth > 0){
		//printf("Worker thread: depth: %d\n", depth);
		char* url = path -> url;
		char host[BUF_SIZE]; get_host(host, BUF_SIZE, url);
		char page[BUF_SIZE]; get_path(page, BUF_SIZE, url);
		char* response = http_query(host,page,80);
		if(response != '\0') {
			char* content = http_split_content(response);
			queue_put(html_queue, content);
			path = queue_get(http_queue); depth = path -> depth;
		}
	} pthread_exit(0);
}   //Lines 18

void Open_File(const char* content, char* filename, Queue* http_queue, Path* path1){
	FILE* file; file = fopen(filename, "w");
	int length = strlen(content);
	fwrite(content,sizeof(char), length, file);
	fclose(file); queue_put(http_queue, path1);
}

void Depth_First_Traversal(const char *content, int depth, List* list, Context* context) {
	int Bytes_Send = 0; int Bytes_Received = 0;
	Queue* http_queue = context->http_queue;
	Queue* html_queue = context->html_queue;
	List* reserve = list_new();
	char absolute_url[BUF_SIZE]; char** Content_Links = extract_links(content);
	if (Content_Links != NULL && depth > 0) {
		int i = 0; int SearchToEnd = 0;
		while (Content_Links[i] != NULL || SearchToEnd == 0) {
			if (Bytes_Send == Bytes_Received && Content_Links[i] == '\0') SearchToEnd = 1;
			if (Bytes_Received < Bytes_Send) {
				char* content1 = queue_get(html_queue);
				Bytes_Received += 1; push_start(reserve, content1);}
			if (Content_Links[i] != '\0') {
				//printf("Send bytes: %d, Received bytes: %d\n", Bytes_Send, Bytes_Received);
				Link* link2 = list -> end; Path* path2 = link2 -> data;
				char* url2 = path2 -> url; //link the path2 to the initial url address
				make_absolute(absolute_url, BUF_SIZE, path2->url, Content_Links[i]);
				add_page(list, absolute_url, depth);
				Link* link1 = list -> start; Path* path1 = link1 -> data;
				char* url1 = path1 -> url; //link the path1 with initial url address
				char host[BUF_SIZE]; get_host(host, BUF_SIZE, url1);
				char page[BUF_SIZE]; get_path(page, BUF_SIZE, url1);
				if (match_hosts(url1, url2) ) {
					if (Bytes_Send - Bytes_Received < http_queue -> length && content != '\0') {
						char filename[BUF_SIZE]; url_filename(filename, BUF_SIZE, url1);
						if (access(filename, F_OK) == -1) {
							Open_File(content,filename, http_queue,path1); Bytes_Send += 1;
						} i += 1;
					}
				}
				else i += 1;
			}
		}
	}
	if (depth > 1) {
		while (reserve -> start != NULL) {
			content = pop_back(reserve); Depth_First_Traversal(content, depth - 1, list, context);
		}
	} list_free(reserve);
}   // Lines 40

Context* crawl (List* list, int num_workers) {
	// crawl + spawn_worker two function together
	Queue* http_queue = queue_alloc(num_workers);
	Queue* html_queue = queue_alloc(num_workers);
	Context* context = malloc(sizeof(Context));
	context -> http_queue = http_queue;
	context -> html_queue = html_queue;
	pthread_t thread[num_workers];
	int i;
	for (i = 0; i < num_workers; ++i) {
		pthread_create(&thread[i], '\0', Threads, context);
    }
	Link* link = list -> start; Path* path = link -> data;
	int depth = path -> depth - 1; 
	queue_put(http_queue, path);
	char* content = queue_get(html_queue);
	Depth_First_Traversal(content, depth, list, context);
	Path* exit = malloc(sizeof(Path));
	exit -> depth = 0;
	for (i = 0; i < http_queue -> length; ++i) {
		queue_put(http_queue, exit);
	}
	int size = (http_queue -> length);
	for (i = 0; i < size; ++i) {
      pthread_join(thread[i], '\0'); // Waiting for every single thread finish
    }
    return context;
}   // Lines 27

int main(int argc, char **argv) {
	if(argc != 4){
		fprintf(stderr, "usage: ./crawler url depth num_workers\n"); exit(1);
	}
	char url[BUF_SIZE];
	add_scheme(url, BUF_SIZE, argv[1]); // Add http:// if address doesnt has that format
	char host[BUF_SIZE]; get_host(host, BUF_SIZE, url);
	char page[BUF_SIZE]; get_path(page, BUF_SIZE, url);
	int num_workers = atoi(argv[3]); int depth = atoi(argv[2]); 
	//printf("Track %s by depth of %d with %d thread workers.\n", url, depth, num_workers);
	//   create pages queue
	List *pages = list_new();
	add_page(pages, url, depth);
	create_directory(host);
	//   spawn threads and create work queue(s)  
	Context* context = crawl(pages, num_workers);
	//   cleanup
	free(context);list_free(pages);	puts("ALL DONE ^_^ !!!"); // Print out finish message
	return 0;  
}
