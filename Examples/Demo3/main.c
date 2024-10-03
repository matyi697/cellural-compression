#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_DEPTH 100   //a legkedvezobb tomoritesi arany eleresehez valo max melyseg
#define MAX_X 50        //a lenyeg hogy 2 hatvany legyen
#define MAX_Y 50        //a lenyeg hogy 2 hatvany legyen
#define SET_SIZE 4      //hany rule van

void rotate(bool grid[MAX_X][MAX_Y], int iteration, char *direction) {
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
            } else if (strcmp(direction, "left")) {
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
    return;
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
    return;
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
    return;
}

void printGrid(bool grid[MAX_X][MAX_Y]) {
    for (int y = 0; y < MAX_Y; y++) {
        for (int x = 0; x < MAX_X; x++)
            printf(" %c", grid[x][y] == 1 ? 'O' : ' ');
        printf("\n");
    }
    return;
}

void delay(int milli_seconds)
{
    milli_seconds *= 1000;

    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds);
    return;
}

void generalisedRuleSet(unsigned rules[SET_SIZE][2], bool grid[MAX_X][MAX_Y], int iteration) {
    bool offset = iteration % 2; // lehet currentIteration & 1
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
    return;
}

void readRule(char* filename, unsigned rules[SET_SIZE][2]) {
    FILE* ruleFile = fopen(filename, "r");
    if (ruleFile == NULL) {
        perror("Hibas bemeneti rule file!\n");
        return;
    }
    unsigned ruleData = 0;
    unsigned resultData = 0;
    for(int i = 0; (fscanf(ruleFile, "%u %u", &ruleData, &resultData) != 0) && i < SET_SIZE; i++) {
        rules[i][0] = ruleData;
        rules[i][1] = resultData;
    }
    fclose(ruleFile);
    return;
}

void TEST1(void) {
    bool test[MAX_X][MAX_Y] = {0};
    drawRect(test, 20, 20, 25, 28);
    writeGridToFile("original.txt", test);

    rotate(test, 1, "left");

    writeGridToFile("rotated_good.txt", test);
    return;
}

void TEST2 (void) {
    bool test[MAX_X][MAX_Y] = {0};
    srand(time(0));
    for (int y = 10; y < 20; y++)
        for (int x = 10; x < 20; x++)
            test[x][y] = rand() % 4 == 1 ? 1 : 0;

    while (1) {
        bool i = 0;
        rotate(test, i, "right");
        system("clear");
        printGrid(test);
        delay(500);
    }
}

void TEST3(void) {
    unsigned rules[4][2] = {0};
    bool test[MAX_X][MAX_Y] = {0};
    readRule("/home/wittmann/Programing/Algoritmusok ás Adatszerkezetek hatékony implementálása C nyelven/Házi Feladat/cellural-compression/Examples/Demo3/rules.txt", rules);
    drawRect(test, 20, 20, 25, 28);

    writeGridToFile("original.txt", test);
    generalisedRuleSet(rules, test, 1);
    writeGridToFile("rotated.txt", test);
}

int main(int argc, char *argv[]) {
    TEST1();
    TEST3();
    return 0;
}