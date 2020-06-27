#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/decoder.h"
#include "../include/heap.h"

void decode(){
    huff *root;

    root = build();
    decoder("../I-O/texto.txt", root);
}

huff *build(){
    FILE *hf = fopen("../I-O/arvhuf.txt", "r");
    huff *root = NULL;
    int pre_ch[512], pre_freq[512], sym_ch[512], sym_freq[512], n = 0, aux;
    
    //Contagem
    while(!feof (hf)){
        fscanf(hf, "%d", &aux);
        n++;
    }
    n /= 4;
    rewind(hf);
    
    //Preenchendo os arrays
    for(aux = 0; aux < n; aux++){
        fscanf(hf, "%d %d", &pre_ch[aux], &pre_freq[aux]);
    }
    for(aux = 0; aux < n; aux++){
        fscanf(hf, "%d %d", &sym_ch[aux], &sym_freq[aux]);
    }

    builder(pre_ch, pre_freq, sym_ch, sym_freq, 0, n-1, 0, n-1, &root);

    return root;
}

void builder(int pre_ch[], int pre_freq[], int sym_ch[], int sym_freq[], int pre_init, int pre_fin, int sym_init, int sym_fin, huff **root){
    int pos_sym, left_size, right_size;
    if(pre_fin - pre_init < 0){
        *root = NULL;
    }
    else{
        *root = (huff *)malloc(sizeof(huff));
        (*root)->ch = pre_ch[pre_init];
        (*root)->freq = pre_freq[pre_init];
        for(pos_sym = sym_init; pos_sym <= sym_fin; pos_sym++){
            if((sym_freq[pos_sym] == (*root)->freq) && (sym_ch[pos_sym] == (*root)->ch)) break;
        }
        
        left_size = pos_sym - sym_init;
        right_size = sym_fin - pos_sym;
        
        builder(pre_ch, pre_freq, sym_ch, sym_freq, pre_init + 1, pre_init + left_size, sym_init, pos_sym - 1, &((*root)->left));
        builder(pre_ch, pre_freq, sym_ch, sym_freq, pre_init + left_size + 1, pre_fin, pos_sym + 1, sym_fin, &((*root)->right));
    }
}

void decoder(char *fileName, huff *root){
    FILE *file = fopen(fileName, "r");
    FILE *out = fopen("../I-O/saida.txt", "w");
    int ch;
    char c;
    huff *aux = root;
    
    while((c = fgetc(file)) != EOF){
        if(c == '1') aux = aux->right;
        else aux = aux->left;
        if(aux->ch >= 0){
            ch = aux->ch;
            if(ch > 127) ch -= 256;
            fputc((char) ch, out);
            aux = root;
        }
    }
}