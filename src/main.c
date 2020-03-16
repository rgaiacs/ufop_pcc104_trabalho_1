/* Call to trie */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    //printf("%d", argc);
    
    if(argc <= 1) {
        printf("Usage:\n$ ./main.c filename.txt");
        }
    else {
        printf("Processing %s....\n", argv[1]);
    }
    
    return 0;
}