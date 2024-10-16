#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define MAX_DEPTH 100   //a legkedvezobb tomoritesi arany eleresehez valo max melyseg
#define MAX_X 110       //a lenyeg hogy 2 hatvany legyen
#define MAX_Y 20        //a lenyeg hogy 2 hatvany legyen
#define SET_SIZE 6      //hany rule van
#define RULE_PATH "/home/wittmann/Programing/Algoritmusok ás Adatszerkezetek hatékony implementálása C nyelven/Házi Feladat/cellural-compression/Examples/Demo5/rule.txt"

void generalisedRuleSet(unsigned rules[SET_SIZE][2], bool grid[MAX_X][MAX_Y], int iteration) {
    bool offset = iteration % 2;
    for (int y = offset; y < MAX_Y - offset; y += 2) {
        for (int x = offset; x < MAX_X - offset; x += 2) {
            // Túlindexelési problémák elkerülése
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

void transformFile(char* inputfilename, char* outputFilename, char* ruleFilename, int tries) {
    FILE* input = fopen(inputfilename, "rb");
    if (input == NULL) {
        perror("Hibas a bemenet file!\n");
        return;
    }
    FILE* output = fopen(outputFilename, "ab");
    if (output == NULL) {
        perror("Hiba a kimeneti file letrehozasakor!\n");
        return;
    }

    unsigned rules[SET_SIZE][2];
    readRule(RULE_PATH, rules);
    unsigned char temp = 0;
    bool grid[MAX_X][MAX_Y] = {0};
    unsigned gridIndex = 0;

    while(fread(&temp, sizeof(unsigned char), 1, input) == 1) {
        for (int i = 0; i < 8; i++) {   //betoljuk az olvasott adatot a gridbe
            int y = floor(gridIndex / MAX_X);
            int x = gridIndex % MAX_X;  // Maradékos osztás a sorokhoz            grid[x][y] = temp & 1;
            grid[x][y] = temp & 1;
            temp >>= 1; // Jobbra tolás
            gridIndex++;
        }
        if (gridIndex == (MAX_X * MAX_Y)) { // Helyes méret-ellenőrzés
            for (int m = 0; m < tries; m++)
                generalisedRuleSet(rules, grid, m);
            gridIndex = 0;
            for (int f = 0; f < MAX_X; f++)
                fwrite(grid[f], sizeof(bool), MAX_Y, output);
            memset(grid, 0, sizeof(grid));
        }
        if (gridIndex > 0) {
            for (int m = 0; m < tries; m++)
                generalisedRuleSet(rules, grid, m);
            for (int f = 0; f < MAX_X; f++)
                fwrite(grid[f], sizeof(bool), MAX_Y, output);
        }
    }
    fclose(output);
    fclose(input);
}

int main () {
    transformFile("test.bin", "asd.bin", "rules.txt", 0);
    return 0;
}