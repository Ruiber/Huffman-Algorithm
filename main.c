#include <stdio.h>
#include <stdlib.h>

typedef struct huff_tree{
    int freq, ch;
    struct huff_tree *left, *right;
}huff;

int *freq_table(char *fileName);
void insert(huff *heap, huff node);
void up(huff *heap, int pos);
huff *pop(huff *heap, huff node);
void down(huff *heap, int pos);
void populate(huff *heap, int *freq);

int main(){
    int *freq = freq_table("input.txt");
    int i;
    huff heap[257];
    
    populate(heap, freq);

    free(freq);
    
    return 0;
}

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

void insert(huff *heap, huff node){
    heap[++heap[0].freq] = node;
    up(heap, heap[0].freq);
}

void up(huff *heap, int pos){
    huff aux;
    if((heap[pos].freq < heap[pos/2].freq) && pos > 1){
        aux = heap[pos];
        heap[pos] = heap[pos/2];
        heap[pos/2] = aux;
        up(heap, pos/2);
    }
}

huff *pop(huff *heap, huff node){
    huff *aux = &heap[1];
    heap[1] = heap[heap[0].freq--];
    down(heap, 1);
    return aux;
}

void down(huff *heap, int pos){
    int qnt = heap[0].freq;
    int least;
    huff aux;
    if(qnt >= 2*pos + 1){
        if(heap[2*pos].freq > heap[2*pos + 1].freq) least = 2*pos + 1;
        else least = 2*pos;
        if(heap[least].freq < heap[pos].freq){
            aux = heap[pos];
            heap[pos] = heap[least];
            heap[least] = aux;
            down(heap, least);
        }
    }
    else if(qnt == 2*pos && heap[pos].freq > heap[2*pos].freq){
        aux = heap[pos];
        heap[pos] = heap[2*pos];
        heap[2*pos] = aux;
        down(heap, 2*pos);
    }
}

void populate(huff *heap, int *freq){
    int i;
    huff aux;
    heap[0].freq = 0;
    heap[0].ch = -2;
    heap[0].left = NULL;
    heap[0].right = NULL;
    for(i = 0; i < 256; i++){
        if(freq[i] == 0) continue;
        aux.freq = freq[i];
        aux.ch = i;
        aux.left = NULL;
        aux.right = NULL;
        insert(heap, aux);
    }
}