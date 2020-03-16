#include <stdlib.h>
#include <stdio.h>
#include "trie.h"

void read_file(FILE *file, TrieTree *tree){
    int c;

    if(file == NULL){
        perror("File is NULL.");
    }
    else{
        do{
            c = add_word(file, tree);
        }while(c != EOF);
    }
}

int add_word(FILE *file, TrieTree *tree){
    int letter, next_letter;
    int i;

    letter = fgetc(file);
    if(letter >= 65 /* A */ && letter <= 90 /* Z */){
        letter+=32; /* transform the letter to lowercase */
    }

    if(letter >= 97 /* a */ && letter <= 122 /* z */){
        i = letter - 97;

        if(tree->next[i] == NULL) {
            tree->next[i] = calloc(1, sizeof(TrieTree)); /* initializes the memory to zeros */
        }

        next_letter = add_word(file, tree->next[i]);

        if(next_letter < 97 /* a */ || next_letter > 122 /* z */){
            tree->is_end_of_word[i] = 1;
            tree->numer_of_apperance[i]+=1;
        }
        
        return next_letter;
    }
    else{
        /* Terminate grow */
        return letter;
    }
}