#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

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
        /*
        -az input filet lemasoljuk a data/temp -be mint file 1
        -vegrehajtjuk az iteraciokat a file 1 el
        -a kimenetet kimentjuk az 
        */
        char copyCommand[256];
        sprintf(copyCommand, "cp %s data/temp/file1.bin", inputfilename);
        system(copyCommand);
        for (int i = 0; i < depth; i++) {
            transformFile("data/temp/file1.bin", "data/temp/file2.bin", ruleSetFilename, i);
            rename("data/temp/file2.bin", "data/temp/file1.bin");
        }        
        char compressCommand[256];
        sprintf(compressCommand, "%s -c data/temp/file1.bin data/temp/%s.bin", compressExec, outputFilename);
        printf("Tomorites sikeres!\n");
    }



/*


if (!strcmp(argv[4], "-t")) {
        long minSize = 0;
        int minIndex = -1;
        long startSize = getFileSize(argv[1]);
        char copyCommand[256];
        sprintf(copyCommand, "cp %s Data/file1.bin", argv[1]);  //csinalunk egy file1.bin copyt az eredeti filerol
        system(copyCommand);
        system("python huffman.py -c Data/file1.bin Data/baseline.bin"); //ez a file a sima tomorites eredemyne a baseline.bin
        
        for (int i = 0; i < depth; i++) {
            transformFile("Data/file1.bin", "Data/file2.bin", argv[3], i);
            system("python huffman.py -c Data/file2.bin Data/compressed.bin");
            long sizeFile = getFileSize("Data/compressed.bin");
            printf("%d \n", sizeFile);
            if (sizeFile <= minSize || minSize == 0) {
                minIndex = i;
                minSize = sizeFile;
                rename("Data/compressed.bin", "Data/compressed_final.bin");
            }
            remove("Data/file1.bin");
            rename("Data/file2.bin", "Data/file1.bin");
        }
        printf("Az eredet file méret: %lu\n", getFileSize("Data/compressed.bin"));
        printf("A baseline meret: %lu\n", getFileSize("Data/baseline.bin"));
        printf("A kompresszios rata: %f \nA kimeneti meret: %lu \nAz index: %d", ((double)startSize/(double)minSize), minSize, minIndex);
    }
*/
    return 0;
}