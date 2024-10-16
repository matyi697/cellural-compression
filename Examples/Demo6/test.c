#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void createFile(char* filename, unsigned size) {
    FILE* outFile = fopen(filename, "wb");
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        char ch = rand() % 255;
        fwrite(&ch, sizeof(char), 1, outFile);
    }
    fclose(outFile);
}

int main (int argc, char* argv[]) {
    createFile(argv[1], atoi(argv[2]));
    return 0;
}