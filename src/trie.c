#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

void __find_suggestions(char *prefix, CompressedTrieTree *tree, char *prefix_aux);

const char * get_suffix(CompressedTrieTree *tree, int letter){
    return tree->sufix[i * STRING_MAX_LENGTH];
}

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
    return letter;
}

void insert_word_to_compressed_trie(char *prefix, CompressedTrieTree *tree){
    int i;
    CompressedTrieTree *subtree;

    /* Does the node has any children? */
    if(tree->number_of_children > 0){  /* YES */
        #ifdef LOG
        printf(
            "%s:%d: Node has one or more children already.\n",
            __FILE__,
            __LINE__
        );
        #endif
        /* Does it already has a next node? */
        i = (int) prefix[0] - 97;
        if(tree->next[i] == NULL){  /* NO */
            /* Create new node */
            subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
            subtree->is_end_of_word = 1;
            subtree->numer_of_appearance = 1;
            strcpy(subtree->prefix, &prefix[1]);
            #ifdef LOG
            printf(
                "%s:%d: Created new node (%p) with prefix %s\n",
                __FILE__,
                __LINE__,
                (void *) subtree,
                subtree->prefix
            );
            #endif
            tree->next[i] = subtree;
            #ifdef LOG
            printf(
                "%s:%d: Added new node (%p) to tree (%p) at tree->next[%d].\n",
                __FILE__,
                __LINE__,
                (void *) subtree,
                (void *) tree,
                i
            );
            #endif
        }
        else{  /* YES */
            #ifdef LOG
            printf(
                "%s:%d: Begin of recursion for %s\n",
                __FILE__,
                __LINE__,
                &prefix[1]
            );
            #endif
            insert_word_to_compressed_trie(&prefix[1], tree->next[i]);
            #ifdef LOG
            printf(
                "%s:%d: End of recursion for %s\n",
                __FILE__,
                __LINE__,
                &prefix[1]
            );
            #endif
        }
    }
    else{  /* NO */
        #ifdef LOG
        printf(
            "%s:%d: Node has no children!.\n",
            __FILE__,
            __LINE__
        );
        #endif
        /* Is the same prefix? */
        if(strcmp(prefix, tree->prefix) == 0){  /* YES */
            tree->numer_of_appearance++;
        }
        else{  /* NO */
            /* Does both prefix start with the same letter? */
            if(prefix[0] == tree->prefix[0]){  /* YES */
            }
            else {  /* NO */
                /* Create new node for current one */
                if(strlen(tree->prefix) > 0 || tree->is_end_of_word == 1){
                    subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
                    i = (int) tree->prefix[0] - 97;
                    subtree->is_end_of_word = 1;
                    subtree->numer_of_appearance = tree->numer_of_appearance;
                    strcpy(subtree->prefix, &tree->prefix[1]);
                    #ifdef LOG
                    printf(
                        "%s:%d: Created new node (%p) with prefix \"%s\".\n",
                        __FILE__,
                        __LINE__,
                        (void *) subtree,
                        subtree->prefix
                    );
                    #endif
                    tree->next[i] = subtree;
                    #ifdef LOG
                    printf(
                        "%s:%d: Added new node (%p) to tree (%p) at tree->next[%d].\n",
                        __FILE__,
                        __LINE__,
                        (void *) subtree,
                        (void *) tree,
                        i
                    );
                    #endif
                    tree->number_of_children++;
                }

                /* Create new node for new word */
                subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
                i = (int) prefix[0] - 97;
                subtree->is_end_of_word = 1;
                subtree->numer_of_appearance = 1;
                strcpy(subtree->prefix, &prefix[1]);
                #ifdef LOG
                printf(
                    "%s:%d: Created new node (%p) with prefix \"%s\".\n",
                    __FILE__,
                    __LINE__,
                    (void *) subtree,
                    subtree->prefix
                );
                #endif
                tree->next[i] = subtree;
                #ifdef LOG
                printf(
                    "%s:%d: Added new node (%p) to tree (%p) at tree->next[%d].\n",
                    __FILE__,
                    __LINE__,
                    (void *) subtree,
                    (void *) tree,
                    i
                );
                #endif
                tree->number_of_children++;

                /* Reset current node */
                tree->is_end_of_word = 0;
                tree->numer_of_appearance = 0;
                tree->prefix[0] = '\0';
            }
        }
    }
}

void find_suggestions(char *prefix, CompressedTrieTree *tree){
    char prefix_aux[32];
    prefix_aux[0] = '\0';

    __find_suggestions(prefix, tree, prefix_aux);
}

void __find_suggestions(char *prefix, CompressedTrieTree *tree, char *prefix_aux){
    char suffix[32];
    char prefix_aux_tmp[32];
    int letter;
    int i;

    if(strlen(prefix) == 0){
        /* Need to print all nodes */
        #ifdef LOG
        printf(
            "%s:%d: Prefix has length zero. Returning all nodes.\n",
            __FILE__,
            __LINE__
        );
        #endif
        if(tree->is_end_of_word == 1){
            printf(
                "%s%s (%d)\n",
                prefix_aux,
                tree->prefix,
                tree->numer_of_appearance
            );
        }
        for(i = 0; i < 26; i++){
            if(tree->next[i] != NULL){
                #ifdef LOG
                printf(
                    "%s:%d: Begin of recursion for tree->next[%d] ...\n",
                    __FILE__,
                    __LINE__,
                    i
                );
                #endif
                strcpy(prefix_aux_tmp, prefix_aux);  /* Need to preserve prefix_aux */
                suffix[0] = (char) i + 97;
                suffix[1] = '\0';
                strcat(prefix_aux_tmp, suffix);
                __find_suggestions(prefix, tree->next[i], prefix_aux_tmp);
                #ifdef LOG
                printf(
                    "%s:%d: End of recursion for tree->next[%d].\n",
                    __FILE__,
                    __LINE__,
                    i
                );
                #endif
            }
        }
    }
    else{
        #ifdef LOG
        printf(
            "%s:%d: Inspecting node %p for \"%s\".\n",
            __FILE__,
            __LINE__,
            (void *) tree,
            prefix
        );
        #endif
        letter = (int) prefix[0];
        i = letter - 97;
        #ifdef LOG
        printf(
            "%s:%d: Testing tree->next[%d].\n",
            __FILE__,
            __LINE__,
            i
        );
        #endif
        if(tree->next[i] != NULL){
            #ifdef LOG
            printf(
                "%s:%d: Begin of recursion for \"%s\" at %p.\n",
                __FILE__,
                __LINE__,
                &prefix[1],
                (void *) tree->next[i]
            );
            #endif
            strncat(prefix_aux, prefix, 1);
            __find_suggestions(&prefix[1], tree->next[i], prefix_aux);
            #ifdef LOG
            printf(
                "%s:%d: End of recursion for \"%s\" at %p.\n",
                __FILE__,
                __LINE__,
                &prefix[1],
                (void *) tree->next[i]
            );
            #endif
        }
        else{
            /* Can this be substring?  */
            #ifdef LOG
            printf(
                "%s:%d: Comparing %s and %s.\n",
                __FILE__,
                __LINE__,
                prefix,
                tree->prefix
            );
            #endif
            if(strlen(prefix) <= strlen(tree->prefix)){  /* YES */
                /* Is this substring? */
                if(strncmp(prefix, tree->prefix, strlen(prefix)) == 0){
                    printf(
                        "%s%s (%d)\n",
                        prefix_aux,
                        tree->prefix,
                        tree->numer_of_appearance
                    );
                }
            }
            
        }
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