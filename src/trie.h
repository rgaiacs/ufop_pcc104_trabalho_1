typedef struct TrieNode {
    int is_end_of_word[26];
    int numer_of_apperance[26];
    struct TrieNode *next[26];  /* 26 letters of the alphabet */
} TrieTree;

typedef struct CompressedTrieNode {
    int is_end_of_word;
    int numer_of_appearance;
    char prefix[32];
    struct CompressedTrieNode *next[26];  /* 26 letters of the alphabet */
} CompressedTrieTree;

void read_file(FILE *file, TrieTree *tree);
int add_word(FILE *file, TrieTree *tree);  /* Recursive function */
TrieTree* find_prefix(char *prefix, TrieTree *tree);
void find_suggestions(char *prefix, TrieTree *tree);

int read_word_from_file(FILE *file, char *prefix);
void insert_word_to_compressed_trie(char *prefix, CompressedTrieTree *tree);
void free_compressed_trie(CompressedTrieTree *tree);