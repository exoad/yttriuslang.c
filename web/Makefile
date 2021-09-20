LIBS = -lpthread lib/liburiparser.a
CC = gcc -Iinclude -I./src
CFLAGS = -g -Wall

.PHONY: default all clean

default: crawler queue_test http_test
all: default

DEPS = src/html.h  src/http.h  src/list.h  src/queue.h  src/url.h  
OBJ = src/crawler.o  src/html.o src/http.o  src/list.o  src/queue.o src/url.o  

QUEUE_OBJ = src/queue.o test/queue_test.o
HTTP_OBJ = src/http.o test/http_test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

crawler: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

queue_test : $(QUEUE_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
	
http_test: $(HTTP_OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
	

clean:
	-rm -f src/*.o test/*.o
	-rm -f crawler queue_test http_test