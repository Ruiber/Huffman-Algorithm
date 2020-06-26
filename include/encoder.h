#ifndef ENCODER_H
#define ENCODER_H
#include <stdio.h>
#include "heap.h"

int *freq_table(char *fileName);
void pre(huff *root, FILE *file);
void sym(huff *root, FILE *file);
void print_tree(huff *root);
void char_coder(huff *root, char code[], char *codes[]);
void encoder (char *fileName, char *codes[]);
void encode(char *fileName);

#endif