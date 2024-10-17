#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_X 256
#define MAX_Y 256
#define SET_SIZE 6

void generalisedRuleSet(unsigned rules[SET_SIZE][2], bool grid[MAX_X][MAX_Y], int iteration) {
    bool offset = iteration % 2;
    for (int y = offset; y < MAX_Y - offset; y += 2) {
        for (int x = offset; x < MAX_X - offset; x += 2) {
            int x0 = x;
            int x1 = x + 1 > MAX_X ? 0 : x + 1;
            int y0 = y;
            int y1 = y + 1 > MAX_Y ? 0 : y + 1;

            int currentState = grid[x0][y0] + (grid[x1][y0] * 2) + (grid[x0][y1] * 4) + (grid[x1][y1] * 8);
            for (int n = 0; n < SET_SIZE; n++)
                if (currentState == rules[n][0]) {
                    grid[x0][y0] = (1 & rules[n][1]) != 0 ? 1 : 0;
                    grid[x1][y0] = (2 & rules[n][1]) != 0 ? 1 : 0;
                    grid[x0][y1] = (4 & rules[n][1]) != 0 ? 1 : 0;
                    grid[x1][y1] = (8 & rules[n][1]) != 0 ? 1 : 0;
                    break;
                }
        }
    }
}

void readRule(char *filename, unsigned rules[SET_SIZE][2]) {
    FILE *ruleFile = fopen(filename, "r");
    if (ruleFile == NULL) {
        perror("Hibas bemeneti rule file!\n");
        return;
    }
    unsigned ruleData = 0;
    unsigned resultData = 0;
    for (int i = 0; (fscanf(ruleFile, "%u %u", &ruleData, &resultData) != 0) && i < SET_SIZE; i++) {
        rules[i][0] = ruleData;
        rules[i][1] = resultData;
    }
    fclose(ruleFile);
}

void transformFile(char* inputfilename, char* outputFilename, char* ruleFilename, bool iteration) {
    FILE* input = fopen(inputfilename, "rb");
    if (input == NULL) {
        perror("Hibas a bemenet file!\n");
        return;
    }
    FILE* output = fopen(outputFilename, "wb");
    if (output == NULL) {
        perror("Hiba a kimeneti file letrehozasakor!\n");
        fclose(input);
        return;
    }

    unsigned rules[SET_SIZE][2];
    readRule(ruleFilename, rules);
    unsigned char temp = 0;
    bool grid[MAX_X][MAX_Y] = {0};
    unsigned gridIndex = 0;

    while (fread(&temp, sizeof(unsigned char), 1, input) == 1) {
        for (int i = 0; i < 8; i++) {
            int y = floor(gridIndex / MAX_X);
            int x = gridIndex % MAX_X;
            grid[x][y] = temp & 1;
            temp >>= 1;
            gridIndex++;
        }

       
        if (gridIndex == (MAX_X * MAX_Y)) {
            generalisedRuleSet(rules, grid, iteration);
            unsigned char byteBuffer = 0;
            int bitCounter = 0;
            for (int y = 0; y < MAX_Y; y++) {
                for (int x = 0; x < MAX_X; x++) {
                    byteBuffer <<= 1;
                    byteBuffer |= grid[x][y];
                    bitCounter++;
                    if (bitCounter == 8) {
                        fwrite(&byteBuffer, sizeof(unsigned char), 1, output);
                        byteBuffer = 0;
                        bitCounter = 0;
                    }
                }
            }

            if (bitCounter > 0) {
                byteBuffer <<= (8 - bitCounter);
                fwrite(&byteBuffer, sizeof(unsigned char), 1, output);
            }

            gridIndex = 0;
            memset(grid, 0, sizeof(grid));
        }
    }

    // Ha maradtak adatok a gridben
    if (gridIndex > 0) {
        generalisedRuleSet(rules, grid, iteration);

        unsigned char byteBuffer = 0;
        int bitCounter = 0;
        for (int y = 0; y < MAX_Y; y++) {
            for (int x = 0; x < MAX_X; x++) {
                byteBuffer <<= 1;
                byteBuffer |= grid[x][y];
                bitCounter++;
                if (bitCounter == 8) {
                    fwrite(&byteBuffer, sizeof(unsigned char), 1, output);
                    byteBuffer = 0;
                    bitCounter = 0;
                }
            }
        }

        if (bitCounter > 0) {
            byteBuffer <<= (8 - bitCounter);
            fwrite(&byteBuffer, sizeof(unsigned char), 1, output);
        }
    }

    fclose(output);
    fclose(input);
}


void decode(char *inputFilename, char *outputFilename) {
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
}

void compress(char *inputFilename, char *outputFilename) {
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
}

int main (int argc, char* argv[]) {

    return 0;
}