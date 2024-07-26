#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <assert.h>

#define CAPASITY_IN_BYTES 64000
#define CAPASITY_IN_WORDS (CAPASITY_IN_BYTES / sizeof(uintptr_t))
#define CHUNK_CAPASITY 1024

#define UNIMPLEMENTED \
    do { \
        fprintf(stderr, "%s:%d: %s is not inmplemented", __FILE__, __LINE__, __func__); \
        abort(); \
    } while (0)

extern uintptr_t heap[];
extern size_t heap_size;

typedef struct
{
    uintptr_t *start;
    size_t size;
} Chunk;

typedef struct
{
    Chunk chunks[CHUNK_CAPASITY];
    size_t count;
} Chunk_list;

extern Chunk_list alloc_chunks;
extern Chunk_list freed_chunks;

void insert_chunk(Chunk_list *list, Chunk chunk);
void remove_chunk(Chunk_list *list, size_t pos);
void merge_chunk(Chunk_list *src);

void *heap_alloc(size_t size_in_bytes);
void heap_free(void *ptr); 

int find_chunk(Chunk_list *list, void *ptr);
void dump_alloc_chunks();
void dump_freed_chunks();


#endif