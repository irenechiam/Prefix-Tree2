/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
    Implementation for module which contains  
        Problem 2-related data structures and 
        functions.
    
    Sample solution implemented by Grady Fitzpatrick
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "problem.h"
#include "problemStruct.c"
#include "solutionStruct.c"
#include "prefixTree.h"

/* Number of words to allocate space for initially. */
#define INITIALWORDSALLOCATION 64

/* Denotes that the dimension has not yet been set. */
#define DIMENSION_UNSET (-1)

#define MAXIMUM_NEIGHBOURS (8)
#define BLANK_SPACE (' ')

struct problem;
struct solution;

/* Sets up a solution for the given problem. */
struct solution *newSolution(struct problem *problem);
/* 
    Reads the given dict file into a list of words 
    and the given board file into a nxn board.
*/
struct problem *readProblemA(FILE *dictFile, FILE *boardFile){
    struct problem *p = (struct problem *) malloc(sizeof(struct problem));
    assert(p);

    /* Part B onwards so set as empty. */
    p->partialString = NULL;

    int wordCount = 0;
    int wordAllocated = 0;
    char *dictText = NULL;
    char **words = NULL;

    /* Read in text. */
    size_t allocated = 0;
    /* Exit if we read no characters or an error caught. */
    int success = getdelim(&dictText, &allocated, '\0', dictFile);

    if(success == -1){
        /* Encountered an error. */
        perror("Encountered error reading dictionary file");
        exit(EXIT_FAILURE);
    } else {
        /* Assume file contains at least one character. */
        assert(success > 0);
    }

    char *boardText = NULL;
    /* Reset allocated marker. */
    allocated = 0;
    success = getdelim(&boardText, &allocated, '\0', boardFile);

    if(success == -1){
        /* Encountered an error. */
        perror("Encountered error reading board file");
        exit(EXIT_FAILURE);
    } else {
        /* Assume file contains at least one character. */
        assert(success > 0);
    }

    /* Progress through string. */
    int progress = 0;
    /* Table string length. */
    int dictTextLength = strlen(dictText);

    /* Count words present. */
    while(progress < dictTextLength){
        char *word = NULL;
        int nextProgress;
        /* Read each value into the dictionary. */
        if(progress == 0){
            /* First line. */
            int wordNumberGuess;
            assert(sscanf(dictText + progress, "%d %n", &wordNumberGuess, &nextProgress) == 1);
            /* Will fail if integer missing from the start of the words. */
            assert(nextProgress > 0);
            if(wordNumberGuess > 0){
                wordAllocated = wordNumberGuess;
                words = (char **) malloc(sizeof(char *) * wordAllocated);
                assert(words);
            }
            progress += nextProgress;
            continue;
        } else {
            assert(sscanf(dictText + progress, "%m[^\n] %n", &word, &nextProgress) == 1);
            assert(nextProgress > 0);
            progress += nextProgress;
        }

        /* Check if more space is needed to store the word. */
        if(wordAllocated <= 0){
            words = (char **) malloc(sizeof(char *) * INITIALWORDSALLOCATION);
            assert(words);
            wordAllocated = INITIALWORDSALLOCATION;
        } else if(wordCount >= wordAllocated){
            words = (char **) realloc(words, sizeof(char *) * 
                wordAllocated * 2);
            assert(words);
            wordAllocated = wordAllocated * 2;
            /* Something has gone wrong if there's not sufficient word 
                space for another word. */
            assert(wordAllocated > wordCount);
        }

        words[wordCount] = word;
        wordCount++;
    }
    
    /* Done with dictText */
    if(dictText){
        free(dictText);
    }
    
    /* Now read in board */
    progress = 0;
    int dimension = 0;
    int boardTextLength = strlen(boardText);
    /* Count dimension with first line */
    while(progress < boardTextLength){
        /* Count how many non-space characters appear in line. */
        if(boardText[progress] == '\n' || boardText[progress] == '\0'){
            /* Reached end of line. */
            break;
        }
        if(isalpha(boardText[progress])){
            dimension++;
        }
        progress++;
    }

    assert(dimension > 0);

    /* Check each line has the correct dimension. */
    for(int i = 1; i < dimension; i++){
        int rowDim = 0;
        if(boardText[progress] == '\n'){
            progress++;
        }
        while(progress < boardTextLength){
            /* Count how many non-space characters appear in line. */
            if(boardText[progress] == '\n' || boardText[progress] == '\0'){
                /* Reached end of line. */
                break;
            }
            if(isalpha(boardText[progress])){
                rowDim++;
            }
            progress++;
        }
        if(rowDim != dimension){
            fprintf(stderr, "Row #%d had %d letters, different to Row #1's %d letters.\n", i + 1, rowDim, dimension);
            assert(rowDim == dimension);
        }
    }

    /* Define board. */
    char *boardFlat = (char *) malloc(sizeof(char) * dimension * dimension);
    assert(boardFlat);
    
    /* Reset progress. */
    progress = 0;
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            int nextProgress;
            assert(sscanf(boardText + progress, "%c %n", &boardFlat[i * dimension + j], &nextProgress) == 1);
            progress += nextProgress;
        }
    }

    char **board = (char **) malloc(sizeof(char **) * dimension);
    assert(board);
    for(int i = 0; i < dimension; i++){
        board[i] = &boardFlat[i * dimension];
    }

    if(boardText){
        free(boardText);
    }

    // fprintf(stderr, "\n");

    /* The number of words in the text. */
    p->wordCount = wordCount;
    /* The list of words in the dictionary. */
    p->words = words;

    /* The dimension of the board (number of rows) */
    p->dimension = dimension;

    /* The board, represented both as a 1-D list and a 2-D list */
    p->boardFlat = boardFlat;
    p->board = board;

    /* For Part B only. */
    p->partialString = NULL;

    p->part = PART_A;

    return p;
}

struct problem *readProblemB(FILE *dictFile, FILE *boardFile, 
    FILE *partialStringFile){
    /* Fill in Part A sections. */
    struct problem *p = readProblemA(dictFile, boardFile);

    char *partialString = NULL;

    /* Part B has a string that is partially given - we assume it follows 
        word conventions for the %s specifier. */
    assert(fscanf(partialStringFile, "%ms", &partialString) == 1);
    
    p->part = PART_B;
    p->partialString = partialString;

    return p;
}

struct problem *readProblemD(FILE *dictFile, FILE *boardFile){
    /* Interpretation of inputs is same as Part A. */
    struct problem *p = readProblemA(dictFile, boardFile);
    
    p->part = PART_D;
    return p;
}

/*
    Outputs the given solution to the given file. If colourMode is 1, the
    sentence in the problem is coloured with the given solution colours.
*/
void outputProblem(struct problem *problem, struct solution *solution, 
    FILE *outfileName){
    assert(solution);
    switch(problem->part){
        case PART_A:
        case PART_D:
            assert(solution->foundWordCount == 0 || solution->words);
            for(int i = 0; i < solution->foundWordCount; i++){
                fprintf(outfileName, "%s\n", solution->words[i]);
            }
            break;
        case PART_B:
            assert(solution->foundLetterCount == 0 || solution->followLetters);
            for(int i = 0; i < solution->foundLetterCount; i++){
                if(isalpha(solution->followLetters[i])){
                    fprintf(outfileName, "%c\n", solution->followLetters[i]);
                } else {
                    fprintf(outfileName, " \n");
                }
            }
            break;
    }
}

/*
    Frees the given solution and all memory allocated for it.
*/
void freeSolution(struct solution *solution, struct problem *problem){
    if(solution){
        if(solution->followLetters){
            free(solution->followLetters);
        }
        if(solution->words){
            free(solution->words);
        }
        free(solution);
    }
}

/*
    Frees the given problem and all memory allocated for it.
*/
void freeProblem(struct problem *problem){
    if(problem){
        if(problem->words){
            for(int i = 0; i < problem->wordCount; i++){
                if(problem->words[i]){
                    free(problem->words[i]);
                }
            }
            free(problem->words);
        }
        if(problem->board){
            free(problem->board);
        }
        if(problem->boardFlat){
            free(problem->boardFlat);
        }
        if(problem->partialString){
            free(problem->partialString);
        }
        free(problem);
    }
}

/* Sets up a solution for the given problem */
struct solution *newSolution(struct problem *problem){
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->foundWordCount = 0;
    s->words = NULL;
    s->foundLetterCount = 0;
    s->followLetters = NULL;
    
    return s;
}

/*
    Solves the given problem according to Part A's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemA(struct problem *p){
    struct solution *s = newSolution(p);
    /* Fill in: Part A */
    s->words = (char**)malloc(sizeof(char*) * INITIALWORDSALLOCATION);

    /* Making the Prefix Trie */
    struct prefixTree *pt = newPrefixTree();
    for(int i = 0; i < p->wordCount; i++) {
        pt = addWordToTree(pt, p->words[i]);
    }


    /* Making the visited array */
    int** visited = (int**)malloc(sizeof(int*) * p->dimension);
    for(int i = 0; i < p->dimension; i++) {
        visited[i] = (int*)malloc(sizeof(int) * p->dimension);
        for(int j = 0; j < p->dimension; j++) {
            visited[i][j] = UNMARKED;
        }
    }

    /* Performing DFS search on the every letter on the board and finding*/
    char* word = (char*)malloc(sizeof(char) * INITIALWORDSALLOCATION);
    for(int i = 0; i < p->dimension; i++) {
        for(int j = 0; j < p->dimension; j++) {
            struct treeNode* curr_node = pt->root;
            char curr_l = tolower(p->board[i][j]);
            int wordLen = 0;
            dfs_search(curr_node, p->board, visited, s, word, wordLen, p->dimension, i, j, curr_l);
            resetWord(word, wordLen);
        }
    }

    freePrefixTree(pt);
    freeVisited(visited, p->dimension);
    free(word);

    /* Sorting the words in the array */
    qsort(s->words, s->foundWordCount, sizeof(char*), cmpWords);
    return s;
}

struct solution *solveProblemB(struct problem *p){
    struct solution *s = newSolution(p);
    s = solveProblemA(p);
    /* Fill in: Part B */
    s->followLetters = (char*)malloc(sizeof(char) * INITIALWORDSALLOCATION);
    /* Making the Prefix Trie */
    struct prefixTree *pt = newPrefixTree();
    for(int i = 0; i < p->wordCount; i++) {
        pt = addWordToTree(pt, p->words[i]);
    }

    /* Checking whether the partial string is part of the current word in the words array*/
    for(int i = 0; i < s->foundWordCount; i++) {
        char* curr_word = s->words[i];
        struct treeNode* curr_node = pt->root;
        for(int i = 0; i < strlen(p->partialString); i++) {
            char curr_l = p->partialString[i];
            if(curr_l == curr_word[i] && curr_node->children[curr_l - 'a'] != NULL) {
                curr_node = curr_node->children[curr_l - 'a'];
            } else {
                break;
            }

            /* If the word can be terminated */
            if(curr_node->endWord == TRUE) {
                if(isLetterInList(s->followLetters, s->foundLetterCount, BLANK_SPACE) == FALSE) {
                    s->followLetters[s->foundLetterCount] = BLANK_SPACE;
                    s->foundLetterCount++;
                }
            }

            /* Checking every cell if the current letter matches with the curr_word's next letter */
            for(int i = 0; i < ASCII_SIZE; i++) {
                if(curr_node->children[i] != NULL) {
                    char l = (char)(i + 'a');
                    if(l == curr_word[strlen(p->partialString)] && isLetterInList(s->followLetters, s->foundLetterCount, l) == FALSE) {
                        s->followLetters[s->foundLetterCount] = l;
                        s->foundLetterCount++;
                    }
                }
            }
        }
    }

    freePrefixTree(pt);

    return s;
}

struct solution *solveProblemD(struct problem *p){
    struct solution *s = newSolution(p);
    /* Fill in: Part D */
    s->words = (char**)malloc(sizeof(char*) * INITIALWORDSALLOCATION);

    /* Making the Prefix Trie */
    struct prefixTree *pt = newPrefixTree();
    for(int i = 0; i < p->wordCount; i++) {
        pt = addWordToTree(pt, p->words[i]);
    }

    /* Making the visited array */
    int** visited = (int**)malloc(sizeof(int*) * p->dimension);
    for(int i = 0; i < p->dimension; i++) {
        visited[i] = (int*)malloc(sizeof(int) * p->dimension);
        for(int j = 0; j < p->dimension; j++) {
            visited[i][j] = UNMARKED;
        }
    }

    char* word = (char*)malloc(sizeof(char) * INITIALWORDSALLOCATION);
    for(int i = 0; i < p->dimension; i++) {
        for(int j = 0; j < p->dimension; j++) {
            struct treeNode* curr_node = pt->root;
            char curr_l = tolower(p->board[i][j]);
            int wordLen = 0;
            dfs_search2(curr_node, p->board, visited, s, word, wordLen, p->dimension, i, j, curr_l);
            resetWord(word, wordLen);
        }
    }

    freePrefixTree(pt);
    freeVisited(visited, p->dimension);
    free(word);

    /* Sorting the words in the array */
    qsort(s->words, s->foundWordCount, sizeof(char*), cmpWords);

    return s;
}

int** getNeighbours(struct treeNode* curr_node, char** board, int x, int y, int dimension) {
    int** neighbours = (int**)malloc(sizeof(int*) * MAXIMUM_NEIGHBOURS);
    int changeInX[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int changeInY[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(int i = 0; i < MAXIMUM_NEIGHBOURS; i++) {
        neighbours[i] = (int*)malloc(sizeof(int) * 2);
        int curr_x = x + changeInX[i];
        int curr_y = y + changeInY[i];
        if(curr_x >= 0 && curr_x < dimension && curr_y >= 0 && curr_y < dimension) {
            neighbours[i][0] = curr_x;
            neighbours[i][1] = curr_y;
        } else {
            free(neighbours[i]);
            neighbours[i] = NULL;
        }
    }
    return neighbours;
}

void dfs_search(struct treeNode* curr_node, char** board, int** visited, struct solution* s, char* word, int wordLen, int dimension, int x, int y, char c) {
    /* If c is a child */
    if(curr_node->children[c - 'a'] != NULL && visited[x][y] == UNMARKED) {
        word[wordLen] = c;
        visited[x][y] = MARKED;
        wordLen++;

        curr_node = curr_node->children[c - 'a'];
        if(curr_node->endWord == TRUE) {
            /* Check if the word is inside the words array */
            char* new = duplicateWord(word, wordLen);
            if(checkWords(s->words, new, s->foundWordCount) == FALSE) {
                s->words[s->foundWordCount] = new;
                s->foundWordCount++;
                if(s->foundWordCount == INITIALWORDSALLOCATION) {
                    s->words = (char **)realloc(s->words, sizeof(char *) * INITIALWORDSALLOCATION * 2);
                }
            } else {
                free(new);
            }
        }
        int** neighbours = getNeighbours(curr_node, board, x, y, dimension);   
        for(int i = 0; i < MAXIMUM_NEIGHBOURS; i++) {
            if(neighbours[i] != NULL) {
                int curr_x = neighbours[i][0];
                int curr_y = neighbours[i][1];
                char curr_char = tolower(board[curr_x][curr_y]);
                dfs_search(curr_node, board, visited, s, word, wordLen, dimension, curr_x, curr_y, curr_char);
                free(neighbours[i]);
            } else {
                continue;
            }
        }
        free(neighbours);
        visited[x][y] = UNMARKED;
    }
}

void dfs_search2(struct treeNode* curr_node, char** board, int** visited, struct solution* s, char* word, int wordLen, int dimension, int x, int y, char c) {
    if(curr_node->children[c - 'a'] != NULL && visited[x][y] == UNMARKED) {
        word[wordLen] = c;
        visited[x][y] = MARKED;
        wordLen++;
        curr_node = curr_node->children[c - 'a'];

        if(curr_node->endWord == TRUE) {
            char* new = duplicateWord(word, wordLen);
            if(checkWords(s->words, new, s->foundWordCount) == FALSE) {
                s->words[s->foundWordCount] = new;
                s->foundWordCount++;
                if(s->foundWordCount == INITIALWORDSALLOCATION) {
                    s->words = (char **)realloc(s->words, sizeof(char *) * INITIALWORDSALLOCATION * 2);
                }
            } else {
                free(new);
            }
        }

        int** neighbours = getNeighbours(curr_node, board, x, y, dimension);
        for(int i = 0; i < MAXIMUM_NEIGHBOURS; i++) {
            if(neighbours[i] != NULL) {
                int curr_x = neighbours[i][0];
                int curr_y = neighbours[i][1];
                char curr_char = tolower(board[curr_x][curr_y]);

                if(isLetterInList(word, wordLen, curr_char) == FALSE) {
                    dfs_search2(curr_node, board, visited, s, word, wordLen, dimension, curr_x, curr_y, curr_char);
                }
                free(neighbours[i]);
            } else {
                continue;
            }
        }
        free(neighbours);
        visited[x][y] = UNMARKED;
    }
}

void resetWord(char* word, int wordLen) {
    for(int i = 0; i <= wordLen; i++) {
        word[i] = '\0';
    }
    wordLen = 0;
}

char* duplicateWord(char* word, int wordLen) {
    char* new = (char*)malloc(sizeof(char) * INITIALWORDSALLOCATION);
    for(int i = 0; i < wordLen; i++) {
        new[i] = word[i];
    }
    new[wordLen] = '\0';
    return new;
}

int cmpWords(const void* a, const void* b) {
    char* wordA = *(char**)a;
    char* wordB = *(char**)b;
    return strcmp(wordA, wordB);
}

int checkWords(char** words, char* word, int wordsLen) {
    for(int i = 0; i < wordsLen; i++) {
        if(strcmp(words[i], word) == 0) {
            return TRUE;
        }
    }
    return FALSE;
}

int isLetterInList(char* letters, int lettersFound, char l) {
    for(int i = 0; i < lettersFound; i++) {
        if(letters[i] == l) {
            return TRUE;
        }
    }
    return FALSE;
}

void freeVisited(int** visited, int dimension) {
    for(int i = 0; i < dimension; i++) {
        free(visited[i]);
    }
    free(visited);
}

