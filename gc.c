// Garbage Collection Functionality
// Ryan Krawczyk (CSCI227103)

#include "memorymgr.h"
#include "gc.h"
#include <stdio.h>

// Returns pointer to bottom of callstack
void *stackBottom() {
    unsigned long bottom;
    FILE *statfp = fopen("/proc/self/stat", "r");
    fscanf(statfp,
           "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
           "%*u %*u %*u %*u %*u %*u %*d %*d "
           "%*d %*d %*d %*d %*u %*u %*d "
           "%*u %*u %*u %lu", &bottom);
    fclose(statfp);
    return (void *) bottom;
}

// Indicates mark by arithmetically turning on 2nd to last bit
void markBlock(int *ptr) {
	*(ptr - 1) += 2;
}

// Determines mark based on 2nd to last bit
int isMarked(int *ptr) {
	int size = getSize(ptr);
	if (size % 2 == 0) return 1;
	return size % 4 == 3;
}	

// Recursively walks through pointers in heap and marks allocated blocks
void mark(int *ptr) {
	if (ptr < firstBlock() || ptr >= lastBlock()) return;
	if (isAllocated(ptr)) {
		markBlock(ptr);
		int *end = nextBlock(ptr) - 1;
		while (ptr != end) {
			mark(*ptr);
			ptr++;
		}
	}
}

// Iterates through heap and deallocates all unmarked blocks
void sweep() {
	int *current = firstBlock();
	int *last = lastBlock();
	while (current != last) {
		if (!isMarked(current)) myfree(current);
		current = nextBlock(current);
	}
	coalesce();
}

// Identifies and frees inaccessible blocks
void gc() {
	int x = 0;
	int *current = &x;
	int *bottom = (int *) stackBottom();
	while (current != bottom) {
		mark(*current);
		current++;
	}
	sweep();
}

