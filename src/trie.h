typedef struct TrieNode {
    int is_end_of_word[26];
    int numer_of_apperance[26];
    struct TrieNode *next[26];  /* 26 letters of the alphabet */
} TrieTree;

void read_file(FILE *file, TrieTree *tree);
int add_word(FILE *file, TrieTree *tree);  /* Recursive function */
TrieTree* find_prefix(char *prefix, TrieTree *tree);
void find_suggestions(char *prefix, TrieTree *tree);