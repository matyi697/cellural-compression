#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_X 8
#define MAX_Y 8
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

void transformFile(char* inputfilename, char* outputFilename, char* ruleFilename, bool offset, int iterations) {
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
            for (int i = offset; i < iterations + offset; i++)
                generalisedRuleSet(rules, grid, i);
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

    if (gridIndex > 0) {
        for (int i = offset; i < iterations + offset; i++)
                generalisedRuleSet(rules, grid, i);

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

long getFileSize(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Hiba a fájl megnyitásakor");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

int main (int argc, char* argv[]) {
    if (argc != 7) {
        printf("A helyes hasznalat:\n./%s [input file] [rule] [iterations] [output filename] [compressor]\n");
        return -1;
    }
    char* inputFilename = argv[1];
    char* flag = argv[2];
    char* ruleFilename = argv[3];
    int iterations = atoi(argv[4]);
    char* outputFilename = argv[5];
    char* compressorProgram = argv[6];
    
    transformFile(inputFilename, "temp.bin", ruleFilename, 0, iterations);
    char compressCommand[256];
    
    sprintf(compressCommand, "%s -c temp.bin %s", compressorProgram, "compressed.bin");
    system(compressCommand);
    
    memset(compressCommand, '\0', sizeof(compressCommand));
    
    sprintf(compressCommand, "%s -c input.bin %s", compressorProgram, "compressed_original.bin");
    system(compressCommand);
    printf("A bemeneti file merete: %ld bajt\n", getFileSize(inputFilename));
    printf("A tomoritett file merete sejtautomataval %ld bajt\n", getFileSize("compressed.bin"));
    printf("A tomoritett file merete eredetileg %ld bajt\n", getFileSize("compressed_original.bin"));
    printf("A tomoritesi rata a sejtautomatat alkalmazva: %.3f\n",
    (float)getFileSize("compressed.bin")/(float)getFileSize(inputFilename));
    printf("A tomoritesi rata az eredeti fileal: %.3f\n",
    (float)getFileSize("compressed_original.bin")/(float)getFileSize(inputFilename));
    
    return 0;
}