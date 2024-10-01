#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_DEPTH 100
#define MAX_X 256
#define MAX_Y 256

void rotate(bool grid[MAX_X][MAX_Y], int iteration, char* direction) {
    bool offset = iteration % 2; // lehet currentIteration & 1
    for (int y = offset; y < MAX_Y - offset; y += 2) {
        for (int x = offset; x < MAX_X - offset; x += 2) {
            // Túlindexelési problémák elkerülése
            int x0 = x;
            int x1 = x + 1 > MAX_X ? 0 : x + 1;
            int y0 = y;
            int y1 = y + 1 > MAX_Y ? 0 : y + 1;

            if (strcmp(direction, "right")) {
                if (grid[x0][y0] && !grid[x1][y0] && !grid[x1][y1] && !grid[x0][y1]) {
                    grid[x0][y0] = 0;
                    grid[x1][y0] = 1;
                } else if (!grid[x0][y0] && grid[x1][y0] && !grid[x1][y1] && !grid[x0][y1]) {
                    grid[x1][y0] = 0;
                    grid[x1][y1] = 1;
                } else if (!grid[x0][y0] && !grid[x1][y0] && grid[x1][y1] && !grid[x0][y1]) {
                    grid[x1][y1] = 0;
                    grid[x0][y1] = 1;
                } else if (!grid[x0][y0] && !grid[x1][y0] && !grid[x1][y1] && grid[x0][y1]) {
                    grid[x0][y1] = 0;
                    grid[x0][y0] = 1;
                }
            } else if (strcmp(direction, "left")){
                if (grid[x0][y0] && !grid[x1][y0] && !grid[x1][y1] && !grid[x0][y1]) {
                    grid[x0][y0] = 0;
                    grid[x0][y1] = 1;
                } else if (!grid[x0][y0] && !grid[x1][y0] && !grid[x1][y1] && grid[x0][y1]) {
                    grid[x0][y1] = 0;
                    grid[x1][y1] = 1;
                } else if (!grid[x0][y0] && !grid[x1][y0] && grid[x1][y1] && !grid[x0][y1]) {
                    grid[x1][y1] = 0;
                    grid[x1][y0] = 1;
                } else if (!grid[x0][y0] && grid[x1][y0] && !grid[x1][y1] && !grid[x0][y1]) {
                    grid[x1][y0] = 0;
                    grid[x0][y0] = 1;
                }
            } else {
                perror("Nem helyes irany a forgatasra!\n");
                return;
            }
        }
    }
}

void drawRect(bool grid[MAX_X][MAX_Y], int x0, int y0, int x1, int y1) {
    for (int x = x0; x <= x1; x++) {
        grid[x][y0] = 1;
        grid[x][y1] = 1;
    }

    for (int y = y0; y <= y1; y++) {
        grid[x0][y] = 1;
        grid[x1][y] = 1;
    }
}

void writeGridToFile(char *filename, bool grid[MAX_X][MAX_Y]) {
    FILE *output = fopen(filename, "w");
    if (output == NULL) {
        perror("Hiba a kimeneti file letrehozasaban!\n");
        return;
    }

    for (int y = 0; y < MAX_Y; y++) {
        for (int x = 0; x < MAX_X; x++)
            fprintf(output, " %c", grid[x][y] == 0 ? ' ' : 'O');
        fprintf(output, "\n");
    }
    fclose(output);
}

/*
void transformFile(char *inputFilename, char *outputFilename, char *direction, unsigned generation) {
    FILE *inputFile = fopen(inputFilename, "r");
    if (inputFile == NULL) {
        perror("Hiba a bemeneti fileban!\n");
        return;
    }
    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        perror("Hiba a kimeneti file letrehozasakot!\n");
        return;
    }

    bool grid[255][255] = {0};

}

void generalisedRuleSet(int rule[][2], int ruleSize, bool grid[MAX_X][MAX_Y, int iteration) {
    for (int i = 0; i < iteration; i++) {
        int offset = i % 2;
        for (int y = offset; y < MAX_Y; y++) {
            for (int x = offset; x < MAX_X; x++) {
                int x0 = x;
                int x1 = x + 1 > MAX_X ? 0 : x + 1;
                int y0 = y;
                int y1 = y + 1 > MAX_Y ? 0 : y + 1;

                int currentState = grid[x0][y0] + (grid[x1][y0] * 2) + (grid[x0][y1] * 4) + (grid[x1][y1] * 8);
                for (int i = 0; i < ruleSize; i++)
                    if (currentState == rule[i][0]) {
                        grid[x0][y0] = 1 & rule[i][1];
                        grid[x1][y0] = 2 & rule[i][1];
                        grid[x0][y1] = 4 & rule[i][1];
                        grid[x1][y1] = 8 & rule[i][1];
                    }
            }
        }
    }
}
*/

int main(int argc, char *argv[]) {
    bool test[MAX_X][MAX_Y] = {0};
    drawRect(test, 20, 20, 25, 23);
    writeGridToFile("original.txt", test);
    for (int i = 0; i < 10; i++)
        rotate(test, i, "right");
    writeGridToFile("rotated.txt", test);
    for (int i = 9; i >= 0; i--)
        rotate(test, i, "left");
    writeGridToFile("recovered.txt", test);
    return 0;
}