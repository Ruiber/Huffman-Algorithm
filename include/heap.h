#ifndef HEAP_H
#define HEAP_H

typedef struct huff_tree{
    int freq, ch;
    struct huff_tree *left, *right;
}huff;

void insert(huff *heap[], huff *node);
void up(huff *heap[], int pos);
huff *pop(huff *heap[]);
void down(huff *heap[], int pos);
void populate(huff *heap[], int *freq);
huff *huffman(huff *heap[]);

#endif