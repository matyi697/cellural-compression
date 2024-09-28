#include <stdio.h>
#include <stdbool.h>

void stringThingF(bool grid[255][255], int iteration) {
    bool offset = iteration % 2; //lehet currentIteration & 1;
    for (int i = 0; i < 2; i++) {
        offset = i % 2;
        for (int y = offset; y < 255 - offset; y += 2)
            for (int x = offset; x < 255 - offset; x += 2) {
                //tulindexelesi problemak elkerules
                int x0 = x; /*bal felso sarok*/
                int x1 = x + 1 > 254 ? 0 : x + 1;
                int y0 = y;
                int y1 = y + 1 > 254 ? 0 : y + 1;
                /*
                if(grid[x0][y0] && !grid[x1][y0] && !grid[x1][y1] && !grid[x0][y1]) {}
                else
                if(grid[x0][y1] && !grid [][] && !grid[][] && !grid[][]) {}
                else
                if(grid[x1][y0] && !grid [][] && !grid[][] && !grid[][]) {}
                else
                if(grid[x1][y1] && !grid [][] && !grid[][] && !grid[][]) {}
                */
                if (grid[x0][y0] && grid[x1][y0] && grid[x1][y1] && grid[x0][y1]) {
                    printf("asdasd");
                }

            }
    }
}

/*
A grid offset egy iteracio modulo 
ha a gridbol kiindexelunk akkor osszefuzzik az elejevel

*/

int main() {
    bool grid[255][255] = {0};
    grid[0][0] = 1;
    grid[0][1] = 1;
    grid[0][2] = 1;
    grid[244][0] = 1;
    grid[244][1] = 1;
    grid[244][2] = 1;

    stringThingF(grid, 2);
    return 0;
}