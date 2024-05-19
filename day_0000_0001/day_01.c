#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1(int nums[], int size);
void part2(int nums[], int size);

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
    char file_name[] = "day_01.txt";
    long int file_size = getFileSize(file_name);
    FILE* file = fopen(file_name, "r");
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    int new_lines = 1;  // start at 1 as this is for array declaration
    int i, j, k;

    if( file ) {
        char ch = ' ';
        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
            if( ch == '\n' )
                new_lines++;
        }
        str[i-1] = '\0';
    }
    fclose(file);

    // split string array by '\n'
    int nums[new_lines];
    char ch[5]; // no num is more than 4 digits
    for(i=0, j=0, k=0; i<file_size; i++, j++) {
        if( str[i] == '\0' ) break;
        if( str[i] != '\n' ) {
            ch[j] = str[i];
        } else {
            ch[j] = '\0';
            nums[k] = atoi(ch);
            j = -1; // will count up to zero on next for-loop iteration
            k++;
        }
    }
    ch[j] = '\0';
    nums[k] = atoi(ch);

    part1(nums, k+1);
    part2(nums, k+1);
    
    return 0;
}

void part1(int nums[], int size) {
    int sum = 2020;

    for(int i=0; i<size; i++) {
        for(int j=i+1; j<size; j++) {
            if( nums[i] + nums[j] == sum ) {
                printf("%d\n", (nums[i] * nums[j]));
                return;
            }
        }
    }
}

void part2(int nums[], int size) {
    int sum = 2020;

    for(int i=0; i<size; i++) {
        for(int j=i+1; j<size; j++) {
            for(int k=j+1; k<size; k++) {
                if( nums[i] + nums[j] + nums[k] == sum ) {
                    printf("%d\n", (nums[i] * nums[j] * nums[k]));
                    return;
                }
            }
        }
    }
}