#include <stdio.h>
#include <assert.h>

#define CAPASITY 64000
#define CHUNK_CAPASITY 1024
typedef struct
{
    char *start;
    size_t size;
} Chunk;

typedef struct
{
    Chunk chunks[CHUNK_CAPASITY];
    size_t size;
} Chunk_list;


char heap[CAPASITY] = {0};
size_t heap_size = 0;

// Chunk alloc_chunks[CHUNK_CAPASITY] = {0};
// size_t alloc_chunks_size = 0;
Chunk_list alloc_chunks = {
    .size = 0
};

// Chunk freed_chunks[CHUNK_CAPASITY] = {
//     [0] = {.start = heap, .size=CAPASITY}
// };
// size_t freed_chunks_size = 1;
Chunk_list freed_chunks = {
    .chunks = {[0] = {.start = heap, .size = CAPASITY}},
    .size = 1
};


//list must be sorted
void insert_chunk(Chunk_list *list, Chunk chunk){
    list->chunks[list->size] = chunk;
    for (int i = list->size;i > 0 && list->chunks[i - 1].start > list->chunks[i].start;i--) {
        Chunk tmp = list->chunks[i];
        list->chunks[i] = list->chunks[i - 1];
        list->chunks[i - 1] = tmp;
    }
    list->size++;
}

void remove_chunk(Chunk_list *list, size_t pos){
    for (size_t i = pos;i < list->size;i++) {
        list->chunks[i] = list->chunks[i + 1];
    }
    list->size--;
}
void merge_chunk(Chunk_list *src) {
    Chunk_list dst = {
        .size = 0
    };
    for (size_t i = 0;i < src->size;i++) {
        if (i == 0) {
            insert_chunk(&dst, src->chunks[i]);
        }
        else {
            Chunk prev_chunk = dst.chunks[dst.size - 1];
            if (prev_chunk.start + prev_chunk.size == src->chunks[i].start) {
                dst.chunks[dst.size - 1].size += src->chunks[i].size;
            }
            else {
                insert_chunk(&dst, src->chunks[i]);
            }
        }
    }
    *src = dst;
}

void *heap_alloc(size_t size) {
    if (size > 0) {
        merge_chunk(&freed_chunks);
        for (size_t i = 0;i < freed_chunks.size;i++) {
            Chunk cur_chunk = freed_chunks.chunks[i];
            if (cur_chunk.size >= size) {
                remove_chunk(&freed_chunks, i);
                insert_chunk(&alloc_chunks, (Chunk){cur_chunk.start, size});
                insert_chunk(&freed_chunks, (Chunk){cur_chunk.start + size, cur_chunk.size - size});
                return cur_chunk.start;
            }
        }
        return NULL;
    }
    else {
        return NULL;
    }
}

int find_chunk(Chunk_list *list, char *ptr) {
    for (size_t i = 0;i < list->size;i++) {
        if (list->chunks[i].start == ptr) return i;
    }
    return -1;
}

void heap_free(char *ptr) {
    int ind = find_chunk(&alloc_chunks, ptr);
    assert(ind >= 0);
    Chunk cur_chunk = alloc_chunks.chunks[ind];
    remove_chunk(&alloc_chunks, ind);
    insert_chunk(&freed_chunks, cur_chunk);
}

void dump_alloc_chunks() {
    printf("alloc_chunks %lld:\n", alloc_chunks.size);
    for (size_t i = 0;i < alloc_chunks.size;i++) {
        printf("%p %lld\n", alloc_chunks.chunks[i].start, alloc_chunks.chunks[i].size);
    }
}

void dump_freed_chunks() {
    printf("freed_chunks %lld:\n", freed_chunks.size);
    for (size_t i = 0;i < freed_chunks.size;i++) {
        printf("%p %lld\n", freed_chunks.chunks[i].start, freed_chunks.chunks[i].size);
    }
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
    
    heap_alloc(10);
    dump_alloc_chunks();    
    dump_freed_chunks();    
    return 0;
}
