
/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
     Header for module which contains Prefix Trie 
        data structures and functions.
*/
struct prefixTree;

#ifndef PREFIXTREESTRUCT
#define PREFIXTREESTRUCT
#define CHILD_COUNT (1 << 8)
#define UNMARKED (0)
#define MARKED (1)
#define ASCII_SIZE (256)
#define TRUE (1)
#define FALSE (0)

struct prefixTree;
struct treeNode;

struct prefixTree {
    /* FILL IN: IMPLEMENT */
    struct treeNode* root;
};

struct treeNode {
    struct treeNode* children[ASCII_SIZE];
    int endWord;
};

#endif

struct prefixTree *newPrefixTree();

struct treeNode *newTreeNode();

struct prefixTree *addWordToTree(struct prefixTree *pt, char *word);

void freePrefixTree(struct prefixTree* pt);

void freeTreeNode(struct treeNode* tn);
