// Heap Management Library
// Ryan Krawczyk (CSCI227103)

#include "memorymgr.h"
#include <stdio.h>
#include <string.h>

int *p, *q;

// Employs malloc to allocate given number of bytes in dynamic memory
void initmemory(int size) {
	size = nextEightMult(size);
	p = (int *) malloc(size + 16);
	q = p + size / 4 + 3;
	*(p + 2) = size;					
	*q = 0;
	p += 3;
}

// Iterates through memory until free block of requisite size is found
// Indicates allocated status by arithmetically turning on last bit
// Inserts header for subsequent block 
int* myalloc(int length) {
	int *a = p, *next, currentSize;
	length = nextEightMult(length);
	while (a != q) {
		currentSize = getSize(a);
		if (!isAllocated(a) && currentSize >= length) {
			setSize(a, length + 1);
			next = a + length / 4;
			if (next != q && *next == 0) *next = currentSize - length - 4;
			return a;
		} else a = nextBlock(a);																				
	}
	return NULL;
}

// Indicates unallocated status by arithmetically turning off last bit
void myfree(int *ptr) {
	*(ptr - 1) -= 1;
}

// Iterates through continuous unallocated memory from each free block
// Increases size of first block to logically skip over group of free blocks
void coalesce() {
	int *a = p, *b, total;
	while (a != q) {
		b = nextBlock(a);
		total = getSize(a);
		if (!isAllocated(a)) {
			while (!isAllocated(b) && b != q) {
				total += getSize(b) + 4;
				b = nextBlock(b);
			}
			setSize(a, total);
		}
		a = nextBlock(a);
	}
}

// Iterates though memory and outputs number and size for each block
void printallocation() {
	int *a = p, count = 0, blockSize;
	char *status = (char *) malloc(12 * sizeof(char));
	printf("\n");
	while (a != q) {
		if (isAllocated(a)) 	strcpy(status, "allocated");
		else					strcpy(status, "unallocated");
		blockSize = getSize(a);
		if (blockSize % 2 == 1) blockSize--;
		if (count < 10 && blockSize < 10) {
			printf("Block %d:    size %d %20s\n", count, blockSize, status);
		} else if (count < 10 && blockSize >= 10) {
			printf("Block %d:    size %d %19s\n", count, blockSize, status);
		} else if (count >= 10 && blockSize < 10) {
			printf("Block %d:   size %d %20s\n", count, blockSize, status);
		} else {
			printf("Block %d:   size %d %18s\n", count, blockSize, status);
		}
		a = nextBlock(a);
		count++;
	}
	printf("\n");
	free(status);
}

// Determines allocation status based on last bit
int isAllocated(int *ptr) {
	return getSize(ptr) % 2 == 1;
}

// Increments pointer to next block based on size of current block
int* nextBlock(int *ptr) {
	ptr += getSize(ptr) / 4;
	if (ptr == q)	return ptr;
	else			return ptr + 1;
}

// Returns global pointer to first block
int* firstBlock() {
	return p;
}

// Returns global pointer to sentinel block
int* lastBlock() {
	return q;
}

// Produces next smallest multiple of 8 for a given number
int nextEightMult(int n) {
	return (n % 8 == 0) ? n : n + 8 - (n % 8);
}

// Captures integer size contents from header
int getSize(int *ptr) {
	return *(ptr - 1);
}

// Updates contents of header to provided integer size
void setSize(int *ptr, int n) {
	*(ptr - 1) = n;
}

