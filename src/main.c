/* Call to trie */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trie.h"

int main(int argc, char *argv[]) {
    FILE *file;
    CompressedTrieTree *tree = calloc(1, sizeof(CompressedTrieTree));
    int letter;
    char prefix[32];

    if(argc <= 1) {
        printf("Usage:\n$ ./main.c filename.txt");
        }
    else {
        printf("Processing %s ...\n", argv[1]);

        file = fopen(argv[1], "r");
        do{
            letter = read_word_from_file(file, prefix);
            if(letter >= 97 /* a */ && letter <= 122 /* z */){
                insert_word_to_compressed_trie(prefix, tree);
            }
        }while(letter != EOF);
        fclose(file);

        printf("%s was loaded successfully.\n", argv[1]);

        
        do{
            printf("Type the prefix or 0 to quit: ");
            scanf ("%32s", prefix);
            if(prefix[0] != "0"){
                /* Do search */
            }
        }while(prefix[0] != "0");
    }
 
    free_compressed_trie(tree);

    return 0;
}