#include <stdio.h>
#include <stdlib.h>

char *read_file(char *path) {
    FILE *file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    fseek(file, 0, SEEK_SET);

    char *contents = malloc(file_size + 1);
    fread(contents, 1, file_size, file);
    contents[file_size] = '\0';

    fclose(file);

    return contents;
}
