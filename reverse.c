#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linkedlist {
    char *buffer;
    struct linkedlist *pNext;
    struct linkedlist *pPrevious;
} LINKEDLIST;

LINKEDLIST *freeList (LINKEDLIST*);
LINKEDLIST *addLink (LINKEDLIST*);

int main(int argc, char *argv[]) {
    FILE *inputFile;
    FILE *outputFile = stdout;
    char *buffer = NULL;
    size_t bufsize = 0;
    size_t characters;
    LINKEDLIST *pStart = NULL, *pEnd = NULL;
    LINKEDLIST *pNew, *ptr;
    
    if (argc > 3 || argc < 2) {
        printf("usage: reverse <input> <output>\n");
        return(1);
    }

    if (argc == 3) {
        if ((outputFile = fopen(argv[2], "w")) == NULL) {
            fprintf(stderr, "error: cannot open file %s\n", argv[2]);
            exit(1);
        }
    }
    
    if (argc == 3 && strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "Input and output file must differ\n");
        exit(1);
    } 

    if ((inputFile = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "error: cannot open file %s\n", argv[1]);
        exit(1);
    }
    
    while (characters != -1) {
        LINKEDLIST *pNew;
        pNew = addLink(pNew);
        characters = getline(&pNew->buffer, &bufsize, inputFile);
        if (pStart == NULL) {
            pStart = pNew;
            ptr = pStart;
            pEnd = pStart;
        } else {
            ptr->pNext = pNew;
            pNew->pPrevious = ptr;
            ptr = pNew;
            pEnd = ptr;
        }
    }

    ptr = pEnd;
    while (ptr != NULL) {
        fprintf(outputFile, "%s", ptr->buffer);
        ptr = ptr->pPrevious;
    }
    
    pStart = freeList(pStart);
    free(buffer);
    pEnd, ptr, pNew, buffer = NULL;
    fclose(inputFile);
    return(0);
}

LINKEDLIST *addLink (LINKEDLIST *pNew) {
    if ((pNew = (LINKEDLIST*)malloc(sizeof(LINKEDLIST))) == NULL ){
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    if ((pNew->buffer = (char*)malloc(sizeof(char))) == NULL ){
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    return pNew;
}

LINKEDLIST *freeList (LINKEDLIST *pStart) {
    LINKEDLIST *ptr;
    ptr = pStart;
    while (ptr != NULL) {
        pStart = ptr->pNext;
        free(ptr->buffer);
        free(ptr);
        ptr = pStart;
    }
    return pStart;
}