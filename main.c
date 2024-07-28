#include <stdio.h>

#include "heap.h"


typedef struct node node;


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


int main() {
    stack_base = (const uintptr_t*)__builtin_frame_address(0);
    
    node *root = generate_tree(0, 3);
    printf("%p %lld %lld\n", (void *)root, sizeof(*root), sizeof(node));
    root = NULL;
    
    
    printf("-------------------------\n");
    uintptr_t *ptrs[10];
    for (int i = 1;i <= 10;i++) ptrs[i - 1] = heap_alloc(i);
    // for (int i = 1;i <= 10;i++) if (i % 2 == 0) heap_free(ptrs[i - 1]);
    printf("%p\n", (void*)ptrs[0]);
    heap_collect();

   // printf("-------------------------\n");

    dump_alloc_chunks();    
    dump_freed_chunks();    
    return 0;
}
