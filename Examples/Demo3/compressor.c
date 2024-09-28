#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void decode(char* inputFilename, char* outputFilename) {
    int count;
    unsigned char value;

    FILE *input = fopen(inputFilename, "rb");
    if (input == NULL) {
        perror("Hiba a bemeneti fileban! (RLE)!\n");
    }

    FILE *output = fopen(outputFilename, "wb");
    if (output == NULL) {
        perror("Hiba a file letrehozasban! (RLE)!\n");
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
    printf("Kicsomagolas sikeres!\n");
}

void compress(char* inputFilename, char* outputFilename) {
    int count;
    unsigned char current, next; //lehet nem kell unsigned char

    FILE *input = fopen(inputFilename, "rb");
    if (input == NULL) {
        perror("Hiba az input file megnyitasakor (RLE)!\n");
    }

    FILE *output = fopen(outputFilename, "wb");
    if (output == NULL) {
        perror("Hiba az output file eloallitasabans (RLE)!\n");
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