#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "heap.h"


//#define struct node node; //?????

typedef struct node node;

uintptr_t *stack_base;

struct node {
    char x;
    node *left;
    node *right;
};


node *generate_tree(size_t cur_level, size_t max_level) {
    if (cur_level < max_level) {
        node *root = heap_alloc(sizeof(node));
        root->x = ('a' + cur_level);
        // printf("%lld %c", cur_level, root->x);
        root->left = generate_tree(cur_level + 1, max_level);
        root->right = generate_tree(cur_level + 1, max_level);
        return root;
    }
    return NULL;
}

void print_tree(node* node) {
    printf("%c\n", node->x);
    if (node->left != NULL) {
        printf("left: ");
        print_tree(node->left);
    }

    if (node->right != NULL) {
        printf("right: ");
        print_tree(node->right);
    }
}

int used_chunks[CHUNK_CAPASITY];

void mark_used_chunks(uintptr_t *start, uintptr_t *end) {
    for (uintptr_t *cur_ptr = start;cur_ptr <= end;cur_ptr++) {
        uintptr_t *cur_bytes = (uintptr_t*)*cur_ptr;
        if (heap <= cur_bytes && cur_bytes < heap + CAPASITY_IN_WORDS) {
            printf("found %p\n", (void*)cur_bytes);
            int ind = find_chunk(&alloc_chunks, cur_bytes);
            if (ind >= 0 && used_chunks[ind] != 1) {
                used_chunks[ind] = 1;
                mark_used_chunks(alloc_chunks.chunks[ind].start, alloc_chunks.chunks[ind].start + alloc_chunks.chunks[ind].size);
            }
        }
    }

}

void heap_collect() {
    uintptr_t *cur_stack_frame = (uintptr_t *)__builtin_frame_address(0);
    memset(&used_chunks, 0, sizeof(used_chunks));
    mark_used_chunks(cur_stack_frame, stack_base);
    for (size_t i = 0;i < alloc_chunks.count;) {
        if (used_chunks[i] == 0) {
            heap_free(alloc_chunks.chunks[i].start);
        }
        else {
            i++;
        }
    }
}

int main() {
    stack_base = (uintptr_t *)__builtin_frame_address(0);
    
    node *root = generate_tree(0, 3);
    printf("%p %lld %lld\n", (void *)root, sizeof(*root), sizeof(node));
    root = NULL;
    
    
    printf("-------------------------\n");
    heap_collect();
    heap_alloc(8);
    // printf("-------------------------\n");

    dump_alloc_chunks();    
    dump_freed_chunks();    
    return 0;
}
