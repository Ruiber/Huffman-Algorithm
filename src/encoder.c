#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/encoder.h"
#include "../include/heap.h"

int *freq_table(char *fileName){
    FILE *file = fopen(fileName, "r");
    int *freq = (int *)calloc(256, sizeof(int));
    char c;
    
    while((c = fgetc(file)) != EOF){
        if((int) c >= 0) freq[(int) c]++;
        else freq[256 + (int) c]++;
    }
    
    fclose(file);
    return freq;
}

void pre(huff *root, FILE *file){
    if(root != NULL){
        fprintf(file, "%d %d ", root->ch, root->freq);
        pre(root->left, file);
        pre(root->right, file);
    }
}

void sym(huff *root, FILE *file){
    if(root != NULL){
        sym(root->left, file);
        fprintf(file, "%d %d ", root->ch, root->freq);
        sym(root->right, file);
    }
}

void print_tree(huff *root){
    FILE *file = fopen("I-O/arvhuf.txt", "w");
    pre(root, file);
    fprintf(file, "\n");
    sym(root, file);
    fclose(file);
}

void char_coder(huff *root, char code[], char *codes[]){
    int size = strlen(code);
    if(root->ch != -1){
        codes[root->ch] = (char *)malloc(sizeof(char)*256);
        strcpy(codes[root->ch], code);
    }
    else{
        code[size + 1] = '\0';
        code[size] = '0';
        char_coder(root->left, code, codes);
        code[size + 1] = '\0';
        code[size] = '1';
        char_coder(root->right, code, codes);
    }
}

void encoder (char *fileName, char *codes[]){
    FILE *file = fopen(fileName, "r");
    FILE *encfile = fopen("I-O/texto.txt", "w");
    char c;
    
    while((c = fgetc(file)) != EOF){
        if((int) c >= 0) fprintf(encfile, "%s", codes[(int) c]);
        else fprintf(encfile, "%s", codes[256 + (int) c]);
    }
}

void encode(char *fileName){
    huff *root, *heap[257];
    char *codes[256], code[256];
    int *freq = freq_table(fileName);
    code[0] = '\0';
    
    populate(heap, freq);
    root = huffman(heap);
    print_tree(root);
    char_coder(root, code, codes);
    encoder(fileName, codes);
    free(freq);
}