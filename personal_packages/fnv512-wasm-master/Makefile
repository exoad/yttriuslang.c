CXX=clang++
CFLAGS=-I. -g -O0 -Wall -lstdc++
CFLAGS=-I. -O3 -Wall -lstdc++ -DNDEBUG

DEPS=fnv512.h

all: test

test: fnv512.o
	$(CXX) $(CFLAGS) -o test uint512_llvm.o fnv512.o main.cpp

fnv512.o: fnv512.cpp fnv512.hpp uint512_llvm.o
	$(CXX) $(CFLAGS) -o fnv512.o uint512_llvm.o fnv512.cpp -c

uint512_llvm.o: uint512_llvm.hpp uint512_llvm.cpp
	$(CXX) $(CFLAGS) uint512_llvm.cpp -c

.PHONY: clean

clean:
	rm -f ./*.o
	rm -f test


