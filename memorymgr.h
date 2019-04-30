// memorymgr.h:  Public functions implemented by memorymgr.c

#ifndef MEMORYMGR_H
#define MEMORYMGR_H

void  initmemory(int size);  // initialize a simulated heap
int*  myalloc(int length);   // allocate a block of length (or more) bytes
void  myfree(int *ptr);     // free an allocated block
void  coalesce();            // go through the heap, coalescing unallocated blocks
void  printallocation();  // print info about the simulated heap, for debugging purposes

// The following functions are needed for HW7.
// You should implement them now, as they will also be useful to help you implement HW6.

int   isAllocated(int *ptr);  // is the block at location p allocated?
int*  nextBlock(int *ptr);    // return a pointer to the block that follows p
int*  firstBlock();         // return a pointer to the first block on the heap
int*  lastBlock();          // return a pointer to the sentinel block

#endif

