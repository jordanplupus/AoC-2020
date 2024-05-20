#include <stdio.h>
#include <stdlib.h>

void part1(int lines, char bp[][11]);

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
    char file_name[] = "day_05.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");
    int newLines = 1, line_length = 10;

    if( file ) {
        char ch = ' ';
        int i;
        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
            if( str[i] == '\n' ) 
                newLines++;
        }
        str[i-1] = '\0';
    }

    // split string
    char bp[newLines][line_length + 1];
    for(int i=0, j=0, k=0; i<file_size; i++, j++) {
        if( str[i] == '\n' ) {
            bp[k][j] = '\0';
            k++;
            j = -1;
        }
        else {
            bp[k][j] = str[i];
        }
    }

    // both parts done in part1
    part1(newLines, bp);
}

void part1(int lines, char bp[][11]) {
    int lower_row, upper_row;
    int lower_col, upper_col;
    int d, seat_id, highest_seat_id = 0, lowest_seat_id = 100000;
    int seat_ids[1000];

    // analyze first 7 chars
    for(int i=0; i<lines; i++) {
        lower_row = 0; upper_row = 127;
        lower_col = 0; upper_col = 7;
        for(int j=0; j<7; j++) {
            d = (upper_row - lower_row) / 2;
            if( bp[i][j] == 'F' ) {
                upper_row = lower_row + d;
            }
            else if( bp[i][j] == 'B' ) {
                lower_row = upper_row - d;
            }
            else {
                printf("An error occurred on line %d\n", __LINE__);
                printf("bp was %c\n", bp[i][j]);
                exit(0);
            }
        }
        for(int j=7; j<10; j++) {
            d = (upper_col - lower_col) / 2;
            if( bp[i][j] == 'L' ) {
                upper_col = lower_col + d;
            }
            else if( bp[i][j] == 'R' ) {
                lower_col = upper_col - d;
            }
            else {
                printf("An error occurred on line %d\n", __LINE__);
                printf("bp was %c\n", bp[i][j]);
                exit(0);
            }
        }
        
        seat_id = lower_row * 8 + lower_col;
        seat_ids[seat_id] = seat_id;
        //printf("Seat id of %s:  %d\n", bp[i], seat_id);
        if( seat_id > highest_seat_id )
            highest_seat_id = seat_id;
        if( seat_id < lowest_seat_id )
            lowest_seat_id = seat_id;
    }

    printf("Highest seat id: %d\n", highest_seat_id);
    //printf("Lowest seat id: %d\n", lowest_seat_id);
    printf("\n");

    for(int i=lowest_seat_id; i<=highest_seat_id; i++) {
        if( seat_ids[i] == 0 ) {
            printf("Space between seat id's: %d, %d\n", seat_ids[i-1], seat_ids[i+1]);
            printf("So your seat id is %d\n", seat_ids[i-1] + 1);
            break;
        }
    }
}