#ifndef DECODER_H
#define DECODER_H
#include <stdio.h>
#include "heap.h"

huff *build();
void builder(int pre_ch[], int pre_freq[], int sym_ch[], int sym_freq[], int pre_init, int pre_fin, int sym_init, int sym_final, huff **root);
void decoder(char *fileName, huff *root);
void decode();

#endif