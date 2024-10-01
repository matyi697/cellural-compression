#include <stdio.h>
#include <stdbool.h>

void rotateR(bool grid[255][255], int iteration) {
    bool offset = iteration % 2; //lehet currentIteration & 1;
        for (int y = offset; y < 255 - offset; y += 2)
            for (int x = offset; x < 255 - offset; x += 2) {
                //tulindexelesi problemak elkerulese
                int x0 = x;
                int x1 = x + 1 > 254 ? 0 : x + 1;
                int y0 = y;
                int y1 = y + 1 > 254 ? 0 : y + 1;
                //a szabaly (simple rotate)
                if(grid[x0][y0] && !grid[x1][y0] && !grid[x1][y1] && !grid[x0][y1]) {
                    grid[x0][y0] = 0;
                    grid[x1][y0] = 1;
                }
                else
                if(!grid[x0][y0] && grid[x1][y0] && !grid[x1][y1] && !grid[x0][y1]) {
                    grid[x1][y0] = 0;
                    grid[x1][y1] = 1;
                }
                else
                if(!grid[x0][y0] && !grid[x1][y0] && grid[x1][y1] && !grid[x0][y1]) {
                    grid[x1][y1] = 0;
                    grid[x0][y1] = 1;
                }
                else
                if(!grid[x0][y0] && !grid[x1][y0] && !grid[x1][y1] && grid[x0][y1]) {
                    grid[x0][y1] = 0;
                    grid[x0][y0] = 1;
                }
            }
        return;
}

void rotateL(bool grid[255][255], int iteration) {
    bool offset = iteration % 2; //lehet currentIteration & 1;
    for (int y = offset; y < 255 - offset; y += 2)
        for (int x = offset; x < 255 - offset; x += 2) {
            //tulindexelesi problemak elkerulese
            int x0 = x;
            int x1 = x + 1 > 254 ? 0 : x + 1;
            int y0 = y;
            int y1 = y + 1 > 254 ? 0 : y + 1;
            //a szabaly (simple rotate)
            if(grid[x0][y0] && !grid[x1][y0] && !grid[x1][y1] && !grid[x0][y1]) {   //bal felso->bal also
                grid[x0][y0] = 0;
                grid[x0][y1] = 1;
            }
            else
            if(!grid[x0][y0] && !grid[x1][y0] && !grid[x1][y1] && grid[x0][y1]) {   //bal also -> jobb also
                grid[x0][y1] = 0;
                grid[x1][y1] = 1;
            }
            else
            if(!grid[x0][y0] && !grid[x1][y0] && grid[x1][y1] && !grid[x0][y1]) {   //jobb also -> jobb felso
                grid[x1][y1] = 0;
                grid[x1][y0] = 1;
            }
            else
            if(!grid[x0][y0] && grid[x1][y0] && !grid[x1][y1] && !grid[x0][y1]) {   //jobb felso -> bal felso
                grid[x1][y0] = 0;
                grid[x0][y0] = 1;
            }
        }
    return;
}

void drawRect(bool grid[255][255], int x0, int y0, int x1, int y1) {
    for(int x = x0; x <= x1; x++) {
        grid[x][y0] = 1;
        grid[x][y1] = 1;
    }

    for(int y = y0; y <= y1; y++) {
        grid[x0][y] = 1;
        grid[x1][y] = 1;
    }
}


void writeGridToFile(char* filename, bool grid[255][255]) {
    FILE* output = fopen(filename, "w");
    for (int y = 0; y < 255; y++) {
        for (int x = 0; x < 255; x++)
            fprintf(output, " %c", grid[x][y] == 0 ? ' ' : 'O');
        fprintf(output, "\n");
    }
    fclose(output);
}

int main() {
    bool grid[255][255] = {0};

    drawRect(grid, 10, 10, 14, 14);
    writeGridToFile("origin.txt", grid);
    for (int i = 0; i < 10; i++) {
        rotateR(grid, i);
    }
    writeGridToFile("rotated.txt", grid);

    for (int i = 9; i >= 0; i--)
        rotateL(grid, i);
    writeGridToFile("recovered", grid);
    return 0;
}