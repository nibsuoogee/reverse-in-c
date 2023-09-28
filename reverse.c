#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    FILE *file;
    char *buffer = NULL;
    size_t bufsize = 64;
    size_t characters;
    
    if (argc > 3) {
        printf("too many args!\n");
        return(1);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "error: cannot open file %s\n", argv[1]);
        exit(1);
    }

    characters = getline(&buffer, &bufsize, file);
    while (characters != -1) {
        printf("%s", buffer);
        characters = getline(&buffer, &bufsize, file);
    }

    return(0);
}