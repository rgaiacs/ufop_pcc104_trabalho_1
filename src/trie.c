#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

TrieTree* find_prefix(char *prefix, TrieTree *tree){
    int letter;
    int i;
    TrieTree *prefix_end;

    letter = prefix[0];
    i = letter - 97;

    if(strlen(prefix) == 1){
        return tree->next[i];
    }
    else{
        prefix_end = find_prefix(&prefix[1], tree->next[i]);
        return prefix_end;
    }
}

void find_suggestions(char *prefix, TrieTree *tree){
    char letter;
    int i;
    char new_prefix[32];

    for(i = 0; i < 26; i++){
        letter = (char) i + 97;

        if(tree->is_end_of_word[i]){
            printf("%s%c (%d)\n", prefix, letter, tree->numer_of_apperance[i]);
        }
        if(tree->next[i] != NULL){
            strcpy(new_prefix, prefix);
            strcat(new_prefix, &letter);
            find_suggestions(new_prefix, tree->next[i]);
        }
    }
}