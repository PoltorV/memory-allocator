#include "heap.h"
#include "stdio.h"
#include <string.h>

uintptr_t heap[CAPASITY_IN_WORDS] = {0};
size_t heap_size = 0;
int used_chunks[CHUNK_CAPASITY];

const uintptr_t *stack_base = 0;

Chunk_list alloc_chunks = {
    .count = 0
};

Chunk_list freed_chunks = {
    .chunks = {[0] = {.start = (uintptr_t*)heap, .size = CAPASITY_IN_WORDS}},
    .count = 1
};

static_assert(CAPASITY_IN_BYTES % sizeof(uintptr_t) == 0, "HEAP capasity is not dividable by uintptr_t size");



//list must be sorted
void insert_chunk(Chunk_list *list, Chunk chunk){
    list->chunks[list->count] = chunk;
    for (int i = list->count;i > 0 && list->chunks[i - 1].start > list->chunks[i].start;i--) {
        Chunk tmp = list->chunks[i];
        list->chunks[i] = list->chunks[i - 1];
        list->chunks[i - 1] = tmp;
    }
    list->count++;
}

void remove_chunk(Chunk_list *list, size_t pos){
    for (size_t i = pos;i < list->count;i++) {
        list->chunks[i] = list->chunks[i + 1];
    }
    list->count--;
}
void merge_chunk(Chunk_list *src) {
    Chunk_list dst = {
        .count = 0
    };
    for (size_t i = 0;i < src->count;i++) {
        if (i == 0) {
            insert_chunk(&dst, src->chunks[i]);
        }
        else {
            Chunk prev_chunk = dst.chunks[dst.count - 1];
            if (prev_chunk.start + prev_chunk.size == src->chunks[i].start) {
                dst.chunks[dst.count - 1].size += src->chunks[i].size;
            }
            else {
                insert_chunk(&dst, src->chunks[i]);
            }
        }
    }
    *src = dst;
}

void *heap_alloc(size_t size_in_bytes) {
    size_t size_in_words = (size_in_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
    if (size_in_words > 0) {
        merge_chunk(&freed_chunks);
        for (size_t i = 0;i < freed_chunks.count;i++) {
            Chunk cur_chunk = freed_chunks.chunks[i];
            if (cur_chunk.size >= size_in_words) {
                remove_chunk(&freed_chunks, i);
                insert_chunk(&alloc_chunks, (Chunk){cur_chunk.start, size_in_words});
                insert_chunk(&freed_chunks, (Chunk){cur_chunk.start + size_in_words, cur_chunk.size - size_in_words});
                return cur_chunk.start;
            }
        }
        return NULL;
    }
    else {
        return NULL;
    }
}

int find_chunk(Chunk_list *list, void *ptr) {
    for (size_t i = 0;i < list->count;i++) {
        if (list->chunks[i].start == ptr) return i;
    }
    return -1;
}

void heap_free(void *ptr) {
    int ind = find_chunk(&alloc_chunks, ptr);
    assert(ind >= 0);
    Chunk cur_chunk = alloc_chunks.chunks[ind];
    remove_chunk(&alloc_chunks, ind);
    insert_chunk(&freed_chunks, cur_chunk);
}

void dump_alloc_chunks() {
    printf("alloc_chunks %lld:\n", alloc_chunks.count);
    for (size_t i = 0;i < alloc_chunks.count;i++) {
        printf("%p %lld\n", (void *)alloc_chunks.chunks[i].start, alloc_chunks.chunks[i].size);
    }
}

void dump_freed_chunks() {
    printf("freed_chunks %lld:\n", freed_chunks.count);
    for (size_t i = 0;i < freed_chunks.count;i++) {
        printf("%p %lld\n", (void *)freed_chunks.chunks[i].start, freed_chunks.chunks[i].size);
    }
}

void mark_used_chunks(const uintptr_t *start, const uintptr_t *end) {
    for (uintptr_t *cur_ptr = (uintptr_t*)start;cur_ptr <= end;cur_ptr++) {
        uintptr_t *cur_bytes = (uintptr_t*)*cur_ptr;
        if (heap <= cur_bytes && cur_bytes < heap + CAPASITY_IN_WORDS) {
            // printf("found %p\n", (void*)cur_bytes);
            int ind = find_chunk(&alloc_chunks, cur_bytes);
            if (ind >= 0 && used_chunks[ind] != 1) {
                used_chunks[ind] = 1;
                mark_used_chunks((const uintptr_t*)alloc_chunks.chunks[ind].start, (const uintptr_t*)(alloc_chunks.chunks[ind].start + alloc_chunks.chunks[ind].size));
            }
        }
    }

}

void heap_collect() {
    const uintptr_t *cur_stack_frame = (const uintptr_t *)__builtin_frame_address(0);
    memset(&used_chunks, 0, sizeof(used_chunks));
    mark_used_chunks(cur_stack_frame, stack_base);
    for (size_t i = 0, pos_in_used = 0;i < alloc_chunks.count;pos_in_used++) {
        if (used_chunks[pos_in_used] == 0) {
            heap_free(alloc_chunks.chunks[i].start);
        }
        else {
            i++;
        }
    }
}
