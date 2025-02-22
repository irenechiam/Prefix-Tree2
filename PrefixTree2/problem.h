
/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
     Header for module which contains Problem 2-related 
        data structures and functions.
*/
#include <stdio.h>

struct problem;
struct solution;
struct prefixTree;
struct treeNode;

/* 
    Reads the given dictionary file into a set of words
        and the given board file into a 1-D and 2-D representation.
*/
struct problem *readProblemA(FILE *dictFile, FILE *boardFile);

/* 
    Reads the given dictionary file into a set of words,
        the given board file into a 1-D and 2-D representation and
        the given partial string file as a partial string.
*/
struct problem *readProblemB(FILE *dictFile, FILE *boardFile, 
    FILE *partialStringFile);

/*
    Same as Problem A, but part is set for Part D.
*/
struct problem *readProblemD(FILE *dictFile, FILE *boardFile);

/*
    Solves the given problem according to Part A's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemA(struct problem *p);

/*
    Solves the given problem according to Part B's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemB(struct problem *p);

/*
    Solves the given problem according to Part D's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemD(struct problem *p);

/*
    Outputs the given solution to the given file.
*/
void outputProblem(struct problem *problem, struct solution *solution, 
    FILE *outfileName);

/*
    Frees the given solution and all memory allocated for it.
*/
void freeSolution(struct solution *solution, struct problem *problem);

/*
    Frees the given problem and all memory allocated for it.
*/
void freeProblem(struct problem *problem);

int** getNeighbours(struct treeNode* curr_node, char** board, int x, int y, int dimension);

void dfs_search(struct treeNode* curr_node, char** board, int** visited, struct solution* s, char* word, int wordLen, int dimension, int x, int y, char c);

void dfs_search2(struct treeNode* curr_node, char** board, int** visited, struct solution* s, char* word, int wordLen, int dimension, int x, int y, char c);

void resetWord(char* word, int wordLen);

char* duplicateWord(char* word, int wordLen);

int cmpWords(const void* a, const void* b);

int checkWords(char** words, char* word, int wordsLen);

int isLetterInList(char* letters, int lettersFound, char l);

void freeVisited(int** visited, int dimension);
