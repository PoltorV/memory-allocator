#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include "heap.h"


void heap_collect() {
    UNIMPLEMENTED;
}

int main() {
    // char *root = heap_alloc(26);
    // for (int i = 0;i <= 25;i++) {
    //     root[i] = 'A' + i;
    // }
    //printf("%p %d", freed_chunks[0].start, freed_chunks[0].size);


    // for (int i = 1;i <= 10;i++) {
    //     char *ptr = heap_alloc(i);
    //     if (i % 2 == 0) heap_free(ptr);
    // }
    // // merge_chunk(&freed_chunks);
    
    void *ptrs[10];
    for (int i = 1;i <= 10;i++) ptrs[i - 1] = heap_alloc(i);
    for (int i = 1;i <= 10;i++) 
        if (i % 2 == 0) 
            heap_free(ptrs[i - 1]);
    
    // heap_alloc(10);

    // for (int i = 11;i <= 20;i++) {
    //     char *ptr = heap_alloc(i);
    //     if (i % 2 == 0) heap_free(ptr);
    // }
    dump_alloc_chunks();    
    dump_freed_chunks();    
    return 0;
}
