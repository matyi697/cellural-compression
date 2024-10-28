#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_X 110       //a lenyeg hogy 2 hatvany legyen
#define MAX_Y 20        //a lenyeg hogy 2 hatvany legyen
#define SET_SIZE 6      //hany rule van

void delay(int milli_seconds) {
    milli_seconds *= 1000;

    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds);
    return;
}

void printGrid(bool grid[MAX_X][MAX_Y]) {
    system("clear");
    for (int y = 0; y < MAX_Y; y++) {
        for (int x = 0; x < MAX_X; x++)
            printf(" %c", grid[x][y] == 1 ? 'O' : ' ');
        printf("\n");
    }
    return;
}

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
    return;
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
    return;
}

void readMessage(char* filename, bool grid[MAX_X][MAX_Y]) {
    FILE *message = fopen(filename, "r");
    if (message == NULL) {
        perror("File megnyitasi hiba!\n");
        return;
    }

    char temp;
    int x = 0;
    int y = 0;

    while((temp = fgetc(message)) != EOF) {
        if (temp == ' ') {
            grid[x][y] = 0;
            x++;
            continue;
        }
        if (temp == 'O') {
            grid[x][y] = 1;
            x++;
            continue;
        }
        if (temp == '\n') {
            x = 0;
            y++;
        }
    }
    fclose(message);
}

void messageTest() {
    unsigned rules[SET_SIZE][2] = {};
    bool grid[MAX_X][MAX_Y] = {0};
    readRule("/home/wittmann/Programing/Algoritmusok ás Adatszerkezetek hatékony implementálása C nyelven/Házi Feladat/cellural-compression/Examples/Demo3/stringThing.txt", rules);
    readMessage("/home/wittmann/Programing/Algoritmusok ás Adatszerkezetek hatékony implementálása C nyelven/Házi Feladat/cellural-compression/Examples/Demo4/message.txt", grid);
    int i = 0;
    printGrid(grid);
    delay(3000);
    while (i < 100) {
        generalisedRuleSet(rules, grid, i);
        printGrid(grid);
        delay(100);
        i++;
    }
    i--;
    while(i >= 0) {
        generalisedRuleSet(rules, grid, i);
        printGrid(grid);
        delay(100);
        i--;
    }
    printGrid(grid);
    return;
}

int main(int argc, char *argv[]) {
    messageTest();
    return 0;
}