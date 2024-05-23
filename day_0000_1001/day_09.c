#include <stdio.h>
#include <stdlib.h>

long int getFileSize(char file_name[]) {
    FILE* file = fopen(file_name, "r");

    if( file == NULL ) 
        return -1;

    fseek(file, 0L, SEEK_END);
    long int sz = ftell(file);
    fclose(file);
    return sz;
}

struct Node {
    int x;
    struct Node* next;
};

int main() {
    char file_name[] = "day_09.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");
    int new_lines = 1, digit_length = 16;
    int preamble_length;

    if( file ) {
        char ch = ' ';
        int i;
        for(i=0; ch != EOF; i++) {  // get preamble
            ch = fgetc(file);
            if( ch == '\n' ) {
                str[i] = '\0';
                preamble_length = atoi(str);
                break;
            }
            str[i] = ch;
        }
        for(i=0; ch != EOF; i++) {  // load the rest of the text file
            ch = fgetc(file);
            str[i] = ch;
            if( ch == '\n' )
                new_lines++;
        }
        str[i-1] = '\0';
    }

    char n[digit_length];
    int nums[new_lines];
    int i, j, k;
    for(i=0, j=0, k=0; str[i] != '\0'; i++, j++) {
        if( str[i] == '\n' ) {
            n[j] = '\0';
            nums[k] = atoi(n);
            k++;
            j = -1;
            continue;
        }
        n[j] = str[i];
    }
    n[j] = '\0';
    nums[k] = atoi(n);


}

void part1() {
    
}