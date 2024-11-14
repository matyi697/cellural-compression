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
            int x1 = (x + 1 > MAX_X) ? 0 : x + 1;
            int y0 = y;
            int y1 = (y + 1 > MAX_Y) ? 0 : y + 1;

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

void transformFile(char* inputfilename, char* outputFilename, char* ruleFilename, int iteration) {
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
    //az argumentmok:
    char*       inputfilename   =   argv[1];
    char*       flag            =   argv[2];
    unsigned    depth           =   atoi(argv[3]);
    char*       ruleSetFilename =   argv[4];
    char*       compressExec =   argv[5];
    char*       outputFilename  =   argv[6];

    if (strcmp(flag, "-c") == 0) {
        char copyCommand[256];
        sprintf(copyCommand, "cp %s data/temp/file1.bin", inputfilename);
        system(copyCommand);

        for (int i = 0; i < depth; i++) {
            printf("%d\n", i);
            transformFile("data/temp/file1.bin", "data/temp/file2.bin", ruleSetFilename, i);
            remove("data/temp/file1.bin");
            rename("data/temp/file2.bin", "data/temp/file1.bin");
        }        
        char compressCommand[256];
        sprintf(compressCommand, "%s -c data/temp/file1.bin %s", compressExec, outputFilename);
        printf("%s\n", compressCommand);
        system(compressCommand);
        //rename("data/temp/file1.bin", outputFilename);
        printf("Tomorites sikeres!\n");
    } else 

    if (strcmp(flag, "-t") == 0) {
        transformFile("data/input.bin", "data/output.bin", ruleSetFilename, 0);
        transformFile("data/output.bin", "data/output2.bin", ruleSetFilename, 1);
        transformFile("data/output2.bin", "data/output3.bin", ruleSetFilename, 2);

        transformFile("data/output3.bin", "data/output4.bin", ruleSetFilename, 1);
        transformFile("data/output4.bin", "data/output5.bin", ruleSetFilename, 1);
        transformFile("data/output5.bin", "data/recovered.bin", ruleSetFilename, 0);
    } else

    if (strcmp(flag, "-d") == 0) {
        char copyCommand[256];
        sprintf(copyCommand, "cp %s data/temp/file1.bin", inputfilename);
        system(copyCommand); 

        for (int i = depth-1; i >=  0; i--) {
            printf("%d\n", i);
            transformFile("data/temp/file1.bin", "data/temp/file2.bin", ruleSetFilename, i);
            remove("data/temp/file1.bin");
            rename("data/temp/file2.bin", "data/temp/file1.bin");
        } 

        char compressCommand[256];
        sprintf(compressCommand, "%s -d data/temp/file1.bin %s", compressExec, outputFilename);
        printf("%s\n", compressCommand);
        system(compressCommand);
        //rename("data/temp/file1.bin", outputFilename);
        printf("Tomorites sikeres!\n");
    }
    return 0;
}