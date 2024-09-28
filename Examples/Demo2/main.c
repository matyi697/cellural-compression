#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decode(char* inputFilename, char* outputFilename) {
    int count;
    unsigned char value;

    FILE *input = fopen(inputFilename, "rb");
    if (input == NULL) {
        perror("Hiba a bemeneti fileban!");
    }

    FILE *output = fopen(outputFilename, "wb");
    if (output == NULL) {
        perror("Hiba a file letrehozasban!");
        fclose(input);
    }

    while (!feof(input)) {
        count = fgetc(input);
        if (feof(input)) break;

        value = fgetc(input);
        if (feof(input)) break;

        for (int i = 0; i < count; i++) {
            fputc(value, output);
        }
    }
    printf("kicsomagolas sikeres!\n");
}

void compress(char* inputFilename, char* outputFilename) {
    int count;
    unsigned char current, next; //lehet nem kell unsigned char

    FILE *input = fopen(inputFilename, "rb");
    if (input == NULL) {
        perror("Hiba az input file megnyitasakor");
    }

    FILE *output = fopen(outputFilename, "wb");
    if (output == NULL) {
        perror("Hiba az output file eloallitasabans");
        fclose(input);
    }

    current = fgetc(input);

    while (!feof(input)) {
        count = 1;

        while ((next = fgetc(input)) == current && count < 255) {
            count++;
        }

        fputc(count, output);
        fputc(current, output);

        current = next;
    }
    printf("Tomorites sikeres!\n");
}

int main (int argc, char* argv[]) {
    if (argc < 3){
        printf("A korrekt hasznalat:\n\t-Compress: main -c input.bin output.bin\n\t-Decompress: main -d input.bin output.bin\n");
        return -1;
    }

    if (strcmp(argv[1], "-c") == 0){
        compress(argv[2], argv[3]);
    }
    else
    if (strcmp(argv[1], "-d") == 0){
        decode(argv[2], argv[3]);
    }
    else
        printf("Hibas argumentumok!\n");
    return 0;
}