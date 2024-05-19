#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int part1(int x, int y, char str[][y], int xCount, int yCount);

// rows
int getXSize(char file_name[]) {
    FILE* file = fopen(file_name, "r");

    if( file == NULL ) 
        return -1;

    char ch = ' ';
    int sz = 1;
    
    while(ch != EOF) {
        ch = fgetc(file);
        if( ch == '\n' )
            sz++;
    }
    fclose(file);
    
    return sz;
}

// columns
int getYSize(char file_name[]) {
    FILE* file = fopen(file_name, "r");

    if( file == NULL )
        return -1;

    char ch = ' ';
    int sz;

    for(sz = -1; ch != '\n'; sz++) {
        ch = fgetc(file);
    }
    fclose(file);

    return sz;
}

int main() {
    char file_name[] = "day_03.txt";
    int x = getXSize(file_name);
    int y = getYSize(file_name);
    char str[x][y];
    FILE* file = fopen(file_name, "r");

    //printf("%d, %d\n", x, y);

    char ch = ' ';
    for(int i=0; i<x; i++) {
        for(int j=0; j<y; j++) {
            ch = fgetc(file);
            if( ch != '\n' )
                str[i][j] = ch;
            else j--;   // ignore '\n'
        }
    }
    fclose(file);
    
    long mult = 1;
    mult *= part1(x, y, str, 1, 1);
    mult *= part1(x, y, str, 1, 3);
    mult *= part1(x, y, str, 1, 5);
    mult *= part1(x, y, str, 1, 7);
    mult *= part1(x, y, str, 2, 1);
    printf("All trees multiplied: %lu\n", mult);
}

/**
 * xCount = amount to count down each iteration
 * yCount = amount to count right each iteration
*/
int part1(int x, int y, char str[][y], int xCount, int yCount) {
    int i, j, trees = 0;
    
    for(i=0, j=0; i<x; i+=xCount, j=((j+yCount) % y)) {
        if( str[i][j] == '#' )
            trees++;
    }

    printf("Tree count for right %d, down %d:  %d\n", yCount, xCount, trees);

    return trees;
}