#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

void __find_suggestions(char *prefix, CompressedTrieTree *tree, char *prefix_aux);

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
    int letter, letter_aux;
    int i;
    char suffix_aux[STRING_MAX_LENGTH];
    CompressedTrieTree *subtree;

    letter = ASCII2INT(prefix[0]);

    #ifdef LOG
    printf(
        "%s:%d: Trying to insert \"%s\" to node (%p).\n"
        "\ttree->next[%d] = (%p)\n"
        "\ttree->is_end_of_word[%d] = %d\n",
        __FILE__,
        __LINE__,
        prefix,
        (void *) tree,
        letter,
        (void *) tree->next[letter],
        letter,
        tree->is_end_of_word[letter]
    );
    #endif
    /* Do we already have one word with the same first letter? */
    if(tree->next[letter] == NULL && 
            tree->is_end_of_word[letter] == 0
    ){  /* NO, we can store the word. */
        strcpy(&tree->suffix[SUFFIX_BEGINS_AT(letter)], &prefix[1]);
        tree->is_end_of_word[letter] = 1;
        tree->numer_of_appearance[letter] = 1;
        #ifdef LOG
        printf(
            "%s:%d: Added \"%s\" to node (%p).\n",
            __FILE__,
            __LINE__,
            prefix,
            (void *) tree
        );
        #endif
    }
    else{  /* YES */
        /* Is the same word? */
        if(strcmp(&tree->suffix[SUFFIX_BEGINS_AT(letter)], &prefix[1]) == 0){  /* YES */
            tree->numer_of_appearance[letter]++;
            #ifdef LOG
            printf(
                "%s:%d: Increment numer_of_appearance for \"%s\" in node (%p).\n",
                __FILE__,
                __LINE__,
                prefix,
                (void *) tree
            );
            #endif
        }
        else{  /* NO */
            /* Need to calculate the common suffix */
            i = 0;
            while(tree->suffix[SUFFIX_BEGINS_AT(letter) + i] == prefix[1 + i]){
                i++;
            }
            if(i == 0){
                suffix_aux[0] = '\0';
            }
            else{
                strncpy(suffix_aux, &prefix[1], i);
            }
            #ifdef LOG
            printf(
                "%s:%d: \"%s\" and \"%s\" have \"%s\" in common.\n",
                __FILE__,
                __LINE__,
                &prefix[1],
                &tree->suffix[SUFFIX_BEGINS_AT(letter)],
                suffix_aux
            );
            #endif

            /* Create new node for existing word */
            letter_aux = ASCII2INT(tree->suffix[SUFFIX_BEGINS_AT(letter) + i]);

            subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
            subtree->is_end_of_word[letter_aux] = tree->is_end_of_word[letter];
            subtree->numer_of_appearance[letter_aux] = tree->numer_of_appearance[letter];
            strcpy(
                &subtree->suffix[SUFFIX_BEGINS_AT(letter_aux)],
                &tree->suffix[SUFFIX_BEGINS_AT(letter) + i]
            );
            #ifdef LOG
            printf(
                "%s:%d: Created new node (%p) with suffix \"%s\"\n",
                __FILE__,
                __LINE__,
                (void *) subtree,
                &subtree->suffix[SUFFIX_BEGINS_AT(letter_aux)]
            );
            #endif
            tree->next[letter] = subtree;
            #ifdef LOG
            printf(
                "%s:%d: Added new node (%p) to tree (%p) at tree->next[%d].\n",
                __FILE__,
                __LINE__,
                (void *) subtree,
                (void *) tree,
                letter
            );
            #endif

            /* Updated parent node */
            tree->is_end_of_word[letter] = 0;
            tree->numer_of_appearance[letter] = 0;
            tree->suffix[SUFFIX_BEGINS_AT(letter) + i] = '\0';

            /* Create new node for new word */
            letter_aux = ASCII2INT(prefix[i + 1]);
            subtree->is_end_of_word[letter_aux] = 1;
            subtree->numer_of_appearance[letter_aux] = 1;
            strcpy(
                &subtree->suffix[SUFFIX_BEGINS_AT(letter_aux)],
                &prefix[i + 1]
            );
            #ifdef LOG
            printf(
                "%s:%d: Created new node (%p) with suffix \"%s\"\n",
                __FILE__,
                __LINE__,
                (void *) subtree,
                &subtree->suffix[SUFFIX_BEGINS_AT(letter_aux)]
            );
            #endif
        }
    }

    // /* Does the node has any children? */
    // if(tree->number_of_children > 0){  /* YES */
    //     #ifdef LOG
    //     printf(
    //         "%s:%d: Node has one or more children already.\n",
    //         __FILE__,
    //         __LINE__
    //     );
    //     #endif
    //     /* Does it already has a next node? */
    //     i = (int) prefix[0] - 97;
    //     if(tree->next[i] == NULL){  /* NO */
    //         /* Create new node */
    //         subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
    //         subtree->is_end_of_word = 1;
    //         subtree->numer_of_appearance = 1;
    //         strcpy(subtree->prefix, &prefix[1]);
    //         #ifdef LOG
    //         printf(
    //             "%s:%d: Created new node (%p) with prefix %s\n",
    //             __FILE__,
    //             __LINE__,
    //             (void *) subtree,
    //             subtree->prefix
    //         );
    //         #endif
    //         tree->next[i] = subtree;
    //         #ifdef LOG
    //         printf(
    //             "%s:%d: Added new node (%p) to tree (%p) at tree->next[%d].\n",
    //             __FILE__,
    //             __LINE__,
    //             (void *) subtree,
    //             (void *) tree,
    //             i
    //         );
    //         #endif
    //     }
    //     else{  /* YES */
    //         #ifdef LOG
    //         printf(
    //             "%s:%d: Begin of recursion for %s\n",
    //             __FILE__,
    //             __LINE__,
    //             &prefix[1]
    //         );
    //         #endif
    //         insert_word_to_compressed_trie(&prefix[1], tree->next[i]);
    //         #ifdef LOG
    //         printf(
    //             "%s:%d: End of recursion for %s\n",
    //             __FILE__,
    //             __LINE__,
    //             &prefix[1]
    //         );
    //         #endif
    //     }
    // }
    // else{  /* NO */
    //     #ifdef LOG
    //     printf(
    //         "%s:%d: Node has no children!.\n",
    //         __FILE__,
    //         __LINE__
    //     );
    //     #endif
    //     /* Is the same prefix? */
    //     if(strcmp(prefix, tree->prefix) == 0){  /* YES */
    //         tree->numer_of_appearance++;
    //     }
    //     else{  /* NO */
    //         /* Does both prefix start with the same letter? */
    //         if(prefix[0] == tree->prefix[0]){  /* YES */
    //         }
    //         else {  /* NO */
    //             /* Create new node for current one */
    //             if(strlen(tree->prefix) > 0 || tree->is_end_of_word == 1){
    //                 subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
    //                 i = (int) tree->prefix[0] - 97;
    //                 subtree->is_end_of_word = 1;
    //                 subtree->numer_of_appearance = tree->numer_of_appearance;
    //                 strcpy(subtree->prefix, &tree->prefix[1]);
    //                 #ifdef LOG
    //                 printf(
    //                     "%s:%d: Created new node (%p) with prefix \"%s\".\n",
    //                     __FILE__,
    //                     __LINE__,
    //                     (void *) subtree,
    //                     subtree->prefix
    //                 );
    //                 #endif
    //                 tree->next[i] = subtree;
    //                 #ifdef LOG
    //                 printf(
    //                     "%s:%d: Added new node (%p) to tree (%p) at tree->next[%d].\n",
    //                     __FILE__,
    //                     __LINE__,
    //                     (void *) subtree,
    //                     (void *) tree,
    //                     i
    //                 );
    //                 #endif
    //                 tree->number_of_children++;
    //             }

    //             /* Create new node for new word */
    //             subtree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
    //             i = (int) prefix[0] - 97;
    //             subtree->is_end_of_word = 1;
    //             subtree->numer_of_appearance = 1;
    //             strcpy(subtree->prefix, &prefix[1]);
    //             #ifdef LOG
    //             printf(
    //                 "%s:%d: Created new node (%p) with prefix \"%s\".\n",
    //                 __FILE__,
    //                 __LINE__,
    //                 (void *) subtree,
    //                 subtree->prefix
    //             );
    //             #endif
    //             tree->next[i] = subtree;
    //             #ifdef LOG
    //             printf(
    //                 "%s:%d: Added new node (%p) to tree (%p) at tree->next[%d].\n",
    //                 __FILE__,
    //                 __LINE__,
    //                 (void *) subtree,
    //                 (void *) tree,
    //                 i
    //             );
    //             #endif
    //             tree->number_of_children++;

    //             /* Reset current node */
    //             tree->is_end_of_word = 0;
    //             tree->numer_of_appearance = 0;
    //             tree->prefix[0] = '\0';
    //         }
    //     }
    // }
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
        for(i = 0; i < 26; i++){
            if(tree->is_end_of_word[i] == 1){
                printf(
                    "%s%s (%d)\n",
                    prefix_aux,
                    &tree->suffix[SUFFIX_BEGINS_AT(i)],
                    tree->numer_of_appearance
                );
            }
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
                strcat(prefix_aux_tmp, &tree->suffix[SUFFIX_BEGINS_AT(i)]);
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
        i = ASCII2INT(letter);
        if(tree->is_end_of_word[i] == 1 && strlen(prefix) == 1){
            printf(
                "%s%s%s (%d)\n",
                prefix_aux,
                prefix,
                &tree->suffix[SUFFIX_BEGINS_AT(i)],
                tree->numer_of_appearance[i]
            );
        }
        #ifdef LOG
        printf(
            "%s:%d: Testing tree->next[%d].\n",
            __FILE__,
            __LINE__,
            i
        );
        #endif
        if(tree->next[i] != NULL &&
                strncmp(&prefix[1], &tree->suffix[SUFFIX_BEGINS_AT(i)], strlen(&tree->suffix[SUFFIX_BEGINS_AT(i)]) == 0)
        ){
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
            strcat(prefix_aux, &tree->suffix[SUFFIX_BEGINS_AT(i)]);
            __find_suggestions(&prefix[1 + strlen(&tree->suffix[SUFFIX_BEGINS_AT(i)])], tree->next[i], prefix_aux);
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