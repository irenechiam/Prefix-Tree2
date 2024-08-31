/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
     Implementation for module which contains Prefix Trie 
        data structures and functions.
*/
#include "prefixTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

struct prefixTree *newPrefixTree(){
    /* IMPLEMENT HERE */
    /* Structure of the prefix tree includes a dummy node. */
    struct prefixTree *retTree = (struct prefixTree*)malloc(sizeof(*retTree));
    retTree->root = newTreeNode();
    return retTree;
};

struct treeNode *newTreeNode() {
    struct treeNode* node = (struct treeNode*)malloc(sizeof(*node));
    node->endWord = FALSE;
    for(int i = 0; i < ASCII_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
};

struct prefixTree *addWordToTree(struct prefixTree *pt, char *word){
    /* FILL IN: IMPLEMENT */
    struct treeNode* node = pt->root;
    for(int i = 0; i < strlen(word); i++) {
        if(node->children[word[i]-'a'] == NULL) {
            node->children[word[i]-'a'] = newTreeNode();
        }
        node = node->children[word[i]-'a'];
    }
    node->endWord = TRUE;
    return pt;
};

void freePrefixTree(struct prefixTree* pt) {
    freeTreeNode(pt->root);
    free(pt);
}

void freeTreeNode(struct treeNode* tn) {
    if(tn == NULL) {
        return;
    }

    for(int i = 0; i < ASCII_SIZE; i++) {
        if(tn->children[i] != NULL) {
            freeTreeNode(tn->children[i]);
        }
    }
    free(tn);
}
