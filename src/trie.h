typedef struct CompressedTrieNode {
    int is_end_of_word;
    int numer_of_appearance;
    char prefix[32];
    struct CompressedTrieNode *next[26];  /* 26 letters of the alphabet */
} CompressedTrieTree;

int read_word_from_file(FILE *file, char *prefix);
void insert_word_to_compressed_trie(char *prefix, CompressedTrieTree *tree);
void free_compressed_trie(CompressedTrieTree *tree);