#include <iostream>
#include <fstream>
#include <string>
#include "huffman_tree.h"
#include "encoder.h"

using namespace std;

int main (){
    int freq[256];
    string str;
    char ch;
    
    ifstream fin;
    fin.open("input.txt");
    
    for (int i =0; i < 256; i++){
        freq[i] = 0;
    }
    
    while (getline (fin, str)){
        for (int i = 0; i < str.length(); i++){
            if((int) str[i] >= 0) freq[(int) str[i]]++;
            else freq[256 + (int) str[i]]++;
        }
    }
    
    for(int i=0; i<256; i++)
        cout << freq[i] << endl;
        
    fin.close();
    
    return 0;
}