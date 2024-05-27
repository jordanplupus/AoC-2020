#include <stdio.h>
#include <stdlib.h>

void part1(int lines, int line_length, char seats[][line_length]);
void part2(int lines, int line_length, char seats[][line_length]);

long int getFileSize(char file_name[]) {
    FILE* file = fopen(file_name, "r");

    if( file == NULL ) 
        return -1;

    fseek(file, 0L, SEEK_END);
    long int sz = ftell(file);
    fclose(file);
    return sz;
}

int main() {
    char file_name[] = "day_11.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");
    int new_lines = 1, line_length;
    int i, j, k;

    if( file ) {
        char ch = ' ';
        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
            if( ch == '\n' ) {
                new_lines++;
                break;
            }
        }
        line_length = i+1;
        for(i++; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
            if( ch == '\n' )
                new_lines++;
        }
        str[i-1] = '\0';
        str[i] = '\0';
    }

    char seats[new_lines][line_length]; 
    for(i=0, j=0, k=0; str[i] != '\0'; i++, j++) {
        if( str[i] == '\n' ) {
            seats[k][j] = '\0';
            j = -1;
            k++;
            continue;
        }
        seats[k][j] = str[i];
    }
    seats[k][j] = '\0';

    // only un-comment one at a time as both below functions modify the pointer 
    // values in the seats array

    //part1(new_lines, line_length, seats);
    part2(new_lines, line_length, seats);
}

void part1(int lines, int line_length, char seats[][line_length]) {
    int takeSeat[lines][line_length-1];

    for(int i=0; i<lines; i++) {
        for(int j=0; seats[i][j] != '\0'; j++) {
            if( seats[i][j] == 'L' ) 
                takeSeat[i][j] = 1;
            else takeSeat[i][j] = 0;
        }
    }

    int repeat = 1;
    while( repeat ) {
        repeat = 0;
        for(int i=0; i<lines; i++) {
            for(int j=0; seats[i][j] != '\0'; j++) {
                if( seats[i][j] == '.' ) 
                    continue;

                int taken_seats = 0;
                for(int x=i-1; x<=i+1; x++) {
                    for(int y=j-1; y<=j+1; y++) {
                        if( (x==i && y==j) || x < 0 || x >= lines || y < 0 || y >= line_length-1 ) 
                            continue;

                        if( seats[x][y] == '#' ) 
                            taken_seats++;
                        
                        if( seats[i][j] == '#' && taken_seats >= 4 ) {
                            takeSeat[i][j] = 1;
                            repeat = 1;
                            goto BREAK;
                        }
                    }
                }
                if( seats[i][j] == 'L' && taken_seats == 0 ) {
                    takeSeat[i][j] = 2;
                    repeat = 1;
                }
                BREAK:{}
            }
        }

        for(int i=0; i<lines; i++) {
            for(int j=0; seats[i][j] != '\0'; j++) {
                if( seats[i][j] == '.' ) 
                    continue;

                if( takeSeat[i][j] == 1 ) 
                    seats[i][j] = 'L';
                else if( takeSeat[i][j] == 2 ) 
                    seats[i][j] = '#';
            }
        }
    }// end while

    int occupied_seats = 0;
    for(int i=0; i<lines; i++) {
        for(int j=0; seats[i][j] != '\0'; j++) {
            //printf("%c", seats[i][j]);
            if( seats[i][j] == '#' ) 
                occupied_seats++;
        }
        //printf("\n");
    }

    printf("\nThere are %d occupied seats.\n", occupied_seats);
}

void part2(int lines, int line_length, char seats[][line_length]) {
    int takeSeat[lines][line_length-1];

    for(int i=0; i<lines; i++) {
        for(int j=0; seats[i][j] != '\0'; j++) {
            if( seats[i][j] == 'L' ) 
                takeSeat[i][j] = 1;
            else takeSeat[i][j] = 0;
        }
    }

    int repeat = 1;
    while( repeat ) {
        repeat = 0;
        for(int i=0; i<lines; i++) {
            for(int j=0; seats[i][j] != '\0'; j++) {
                if( seats[i][j] == '.' ) 
                    continue;
            
                int taken_seats = 0;
                for(int iterX=-1; iterX<=1; iterX++) {
                    for(int iterY=-1; iterY<=1; iterY++) {
                        if( iterX == 0 && iterY == 0 ) continue;
                        for(int x=i+iterX, y=j+iterY;  x>=0 && x<lines && y>=0 && y<line_length-1;  x+=iterX, y+=iterY) {   
                            if( seats[x][y] == 'L' ) 
                                break;
                            if( seats[x][y] == '#' ) { 
                                taken_seats++;
                                break;
                            }
                        }
                        if( seats[i][j] == '#' && taken_seats >= 5 ) {
                            takeSeat[i][j] = 1;
                            repeat = 1;
                            goto BREAK;
                        }
                    }
                }
                if( seats[i][j] == 'L' && taken_seats == 0 ) {
                    takeSeat[i][j] = 2;
                    repeat = 1;
                }
                BREAK:{}
            }
        }

        for(int i=0; i<lines; i++) {
            for(int j=0; seats[i][j] != '\0'; j++) {
                if( seats[i][j] == '.' ) 
                    continue;

                if( takeSeat[i][j] == 1 ) 
                    seats[i][j] = 'L';
                else if( takeSeat[i][j] == 2 ) 
                    seats[i][j] = '#';
            }
        }
    }// end while

    int occupied_seats = 0;
    for(int i=0; i<lines; i++) {
        for(int j=0; seats[i][j] != '\0'; j++) {
            //printf("%c", seats[i][j]);
            if( seats[i][j] == '#' ) 
                occupied_seats++;
        }
        //printf("\n");
    }

    printf("\nThere are %d occupied seats in part 2.\n", occupied_seats);
}