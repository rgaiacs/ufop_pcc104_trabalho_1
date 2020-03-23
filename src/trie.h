#define ALPHABET_LENGTH 26
#define STRING_MAX_LENGTH 32
#define SUFFIX_BEGINS_AT(i) (i * STRING_MAX_LENGTH)
#define ASCII2INT(letter) ((int) letter - 97)

typedef struct CompressedTrieNode {
    int is_end_of_word[ALPHABET_LENGTH];
    int numer_of_appearance[ALPHABET_LENGTH];
    char prefix[STRING_MAX_LENGTH];
    char suffix[ALPHABET_LENGTH * STRING_MAX_LENGTH];
    struct CompressedTrieNode *next[ALPHABET_LENGTH];
} CompressedTrieTree;

int read_word_from_file(FILE *file, char *prefix);
void insert_word_to_compressed_trie(char *prefix, CompressedTrieTree *tree);
void find_suggestions(char *prefix, CompressedTrieTree *tree);
void free_compressed_trie(CompressedTrieTree *tree);