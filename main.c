#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct huff_tree{
    int freq, ch;
    struct huff_tree *left, *right;
}huff;

int *freq_table(char *fileName);
void insert(huff *heap[], huff *node);
void up(huff *heap[], int pos);
huff *pop(huff *heap[]);
void down(huff *heap[], int pos);
void populate(huff *heap[], int *freq);
huff *huffman(huff *heap[]);
void pre(huff *root, FILE *file);
void sym(huff *root, FILE *file);
void print_tree(huff *root);
void char_coder(huff *root, char code[], char *codes[]);
void encoder (char *fileName, char *codes[]);
huff *build();
void builder(int pre_ch[], int pre_freq[], int sym_ch[], int sym_freq[], int pre_init, int pre_fin, int sym_init, int sym_final, huff **root);

int main(){
    huff *root, *heap[257], *root2;
    char *codes[256], code[256];
    int *freq = freq_table("input.txt"), i;
    code[0] = '\0';
    
    populate(heap, freq);
    root = huffman(heap);
    print_tree(root);
    char_coder(root, code, codes);
    encoder("input.txt", codes);
    root2 = build();

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

void insert(huff *heap[], huff *node){
    heap[++heap[0]->freq] = node;
    up(heap, heap[0]->freq);
}

void up(huff *heap[], int pos){
    huff *aux;
    if((heap[pos]->freq < heap[pos/2]->freq) && pos > 1){
        aux = heap[pos];
        heap[pos] = heap[pos/2];
        heap[pos/2] = aux;
        up(heap, pos/2);
    }
}

huff *pop(huff *heap[]){
    huff *aux = heap[1];
    heap[1] = heap[heap[0]->freq--];
    down(heap, 1);
    return aux;
}

void down(huff *heap[], int pos){
    int qnt = heap[0]->freq;
    int least;
    huff *aux;
    if(qnt >= 2*pos + 1){
        if(heap[2*pos]->freq > heap[2*pos + 1]->freq) least = 2*pos + 1;
        else least = 2*pos;
        if(heap[least]->freq < heap[pos]->freq){
            aux = heap[pos];
            heap[pos] = heap[least];
            heap[least] = aux;
            down(heap, least);
        }
    }
    else if(qnt == 2*pos && heap[pos]->freq > heap[2*pos]->freq){
        aux = heap[pos];
        heap[pos] = heap[2*pos];
        heap[2*pos] = aux;
        down(heap, 2*pos);
    }
}

void populate(huff *heap[], int *freq){
    int i;
    huff *aux;
    heap[0] = (huff *)malloc(sizeof(huff));
    heap[0]->freq = 0;
    heap[0]->ch = -2;
    heap[0]->left = NULL;
    heap[0]->right = NULL;
    for(i = 0; i < 256; i++){
        if(freq[i] == 0) continue;
        aux = (huff *)malloc(sizeof(huff));
        aux->freq = freq[i];
        aux->ch = i;
        aux->left = NULL;
        aux->right = NULL;
        insert(heap, aux);
    }
}

huff *huffman(huff *heap[]){
    huff *root, *aux1, *aux2;
    while(heap[0]->freq > 1){
        aux1 = pop(heap);
        aux2 = pop(heap);
        root = (huff *)malloc(sizeof(huff));
        root->ch = -1;
        root->freq = aux1->freq + aux2->freq;
        root->right = aux1;
        root->left = aux2;
        insert(heap, root);
    }
    return root;
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
    FILE *file = fopen("arvhuf.txt", "w");
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
    FILE *encfile = fopen("texto.txt", "w");
    char c;
    
    while((c = fgetc(file)) != EOF){
        if((int) c >= 0) fprintf(encfile, codes[(int) c]);
        else fprintf(encfile, codes[256 + (int) c]);
    }
}

huff *build(){
    FILE *hf = fopen("arvhuf.txt", "r");
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