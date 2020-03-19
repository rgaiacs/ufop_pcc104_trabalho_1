/* Call to trie */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

int main(int argc, char *argv[]) {
    FILE *file;
    CompressedTrieTree *tree = calloc(1, sizeof(CompressedTrieTree)); /* initializes the memory to zeros */
    int letter;
    char prefix[32];

    tree->prefix[0] = '\0';

    if(argc <= 1) {
        printf("Usage:\n$ ./main.c filename.txt");
        }
    else {
        printf("Processing %s ...\n", argv[1]);

        file = fopen(argv[1], "r");
        do{
            letter = read_word_from_file(file, prefix);
            if((int) prefix[0] >= 97 /* a */ && (int) prefix[0] <= 122 /* z */){
                #ifdef LOG
                printf(
                    "%s:%d: Insering %s ...\n",
                    __FILE__,
                    __LINE__,
                    prefix
                );
                #endif
                insert_word_to_compressed_trie(prefix, tree);
                #ifdef LOG
                printf(
                    "%s:%d: %s insered.\n",
                    __FILE__,
                    __LINE__,
                    prefix
                );
                #endif
            }
        }while(letter != EOF);
        fclose(file);

        printf("%s was loaded successfully.\n", argv[1]);

        
        do{
            printf("Type the prefix or 0 to quit: ");
            scanf ("%32s", prefix);
            if(strncmp(prefix, "0", 1)){
                /* Do search */
                printf("Suggestions for %s:\n", prefix);
                find_suggestions(prefix, tree);
            }
        }while(strncmp(prefix, "0", 1));
    }
 
    free_compressed_trie(tree);

    return 0;
}