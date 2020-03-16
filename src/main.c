/* Call to trie */

#include <stdio.h>
#include <stdlib.h>
#include "trie.h"

int main(int argc, char *argv[]) {
    FILE *file;
    TrieTree *tree = calloc(1, sizeof(TrieTree));
    char prefix[32];

    if(argc <= 1) {
        printf("Usage:\n$ ./main.c filename.txt");
        }
    else {
        printf("Processing %s ...\n", argv[1]);

        file = fopen(argv[1], "r");
        read_file(file, tree);
        fclose(file);

        printf("%s was loaded successfully.\n", argv[1]);

        printf("Type the prefix or 0 to quit: ");
        scanf ("%32s", prefix);
        find_suggestions(
            prefix,
            find_prefix(prefix, tree)
        );
    }
    
    return 0;
}