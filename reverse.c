#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

typedef struct linkedlist {
    char *buffer;
    struct linkedlist *pNext;
    struct linkedlist *pPrevious;
} LINKEDLIST;

LINKEDLIST *freeList (LINKEDLIST*);
LINKEDLIST *addLink (LINKEDLIST*);
void *null_check_free(void *ptr);

int main(int argc, char *argv[]) {
    FILE *inputFile = stdin;
    FILE *outputFile = stdout;
    size_t bufsize = 0;
    size_t characters = 0;
    LINKEDLIST *pStart = NULL, *pEnd = NULL;
    LINKEDLIST *pNew = NULL, *ptr = NULL;
    struct stat stat1, stat2;

    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return(1);
    }
    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        } 
        
        if ((inputFile = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        if ((outputFile = fopen(argv[2], "w")) == NULL) {
            fprintf(stderr, "reverse: cannot open file'%s'\n", argv[2]);
            exit(1);
        }
        
        // check hardlinkedness
        if (stat(argv[1], &stat1) == -1) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        if (stat(argv[2], &stat2) == -1) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        if (stat1.st_ino == stat2.st_ino) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
    } else if (argc == 2) {
        if ((inputFile = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    // Read input file lines, generate linked list
    while (characters != -1) {
        pNew = addLink(pNew);
        characters = getline(&pNew->buffer, &bufsize, inputFile);
        
        if (characters == -1) {
            pNew->buffer = null_check_free(pNew->buffer);
            pNew = null_check_free(pNew);
            break;
        }
        
        // Addition of newline to possible missing one on last row
        if (pNew->buffer[characters - 1] != '\n') {
            int newLength = characters + 2;
            if ((pNew->buffer = realloc(pNew->buffer, sizeof(char) * newLength)) == NULL ) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }

            pNew->buffer[newLength - 2] = '\n';
            pNew->buffer[newLength - 1] = '\0';
        }
        
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
        pNew = NULL; 
    }

    ptr = pEnd;
    while (ptr != NULL) {
        fprintf(outputFile, "%s", ptr->buffer);
        ptr = ptr->pPrevious;
    }
    
    // Memory housekeeping
    pStart = freeList(pStart);
    ptr = NULL;
    pEnd = NULL;
    pNew = null_check_free(pNew);
    fclose(inputFile);
    fclose(outputFile);
    return(0);
}

LINKEDLIST *addLink (LINKEDLIST *pNew) {
    if ((pNew = (LINKEDLIST*)malloc(sizeof(LINKEDLIST))) == NULL ){
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    pNew->buffer = NULL;
    pNew->pNext = NULL;
    pNew->pPrevious = NULL;
    return pNew;
}

LINKEDLIST *freeList (LINKEDLIST *pStart) {
    LINKEDLIST *ptr;
    ptr = pStart;
    while (ptr != NULL) {
        pStart = ptr->pNext;
        if (ptr->buffer != NULL) {
            free(ptr->buffer);
            ptr->buffer = NULL;
        }
        if (ptr != NULL) {
            free(ptr);
        }
        ptr = pStart;
    }
    return pStart;
}

void *null_check_free(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
        ptr = NULL;
    }
    return ptr;
}
