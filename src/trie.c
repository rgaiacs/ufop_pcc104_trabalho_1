#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

int read_word_from_file(FILE *file, char *prefix){
    int i = 0;
    int letter;

    do{
        letter = fgetc(file);
        
        if(letter >= 65 /* A */ && letter <= 90 /* Z */){
           letter+=32; /* transform the letter to lowercase */
        }

        if(letter >= 97 /* a */ && letter <= 122 /* z */){
            prefix[i] = letter;
            i++;
        }
    }while(letter >= 97 /* a */ && letter <= 122 /* z */);

    prefix[i] = '\0';
    printf("%s\n", prefix);
    return letter;
}

void insert_word_to_compressed_trie(char *prefix, CompressedTrieTree *tree){
    int i;
    CompressedTrieTree *subtree;

    /* Does the node has any children? */
    if(tree->prefix[0] == '\0'){  /* YES */
        /* Need to follow next node */
        i = (int) prefix[0] - 97;
        if(tree->next[i] == NULL){
            /* Create new node */
            subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
            subtree->is_end_of_word = 1;
            subtree->numer_of_appearance = 1;
            strcpy(subtree->prefix, &prefix[1]);
            tree->next[i] = subtree;
        }
        else{
            insert_word_to_compressed_trie(&prefix[1], tree->next[i]);
        }
    }
    else{  /* NO */
        /* Create new node for current one */
        subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
        i = (int) tree->prefix[0] - 97;
        subtree->is_end_of_word = 1;
        subtree->numer_of_appearance = tree->numer_of_appearance;
        tree->next[i] = subtree;

        /* Reset current node */
        tree->is_end_of_word = 0;
        tree->numer_of_appearance = 0;
        tree->prefix[0] = '\0';

        /* Create new node for new word */
        subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
        i = (int) prefix[0] - 97;
        subtree->is_end_of_word = 1;
        subtree->numer_of_appearance = 1;
        tree->next[i] = subtree;
    }
}

void free_compressed_trie(CompressedTrieTree *tree){
    int i;
    for(i = 0; i < 26; i++){
        if(tree->next[i] != NULL){
            free_compressed_trie(tree->next[i]);
            tree->next[i] = NULL;
        }
    }
    free(tree);
}