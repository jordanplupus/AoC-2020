#include <stdio.h>
#include <stdlib.h>

void part2(int lines, int instructions[][3]);
int part1(int lines, int instructions[][3]);

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
    char file_name[] = "day_08.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");
    int new_lines = 1, string_length = 100, sub_bags_length = 10;

    if( file ) {
        char ch = ' ';
        int i;
        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
            if( ch == '\n' )
                new_lines++;
        }
        str[i-1] = '\0';
        str[i] = '\0';
    }

    // split on space ' ' 
    int instructions[new_lines][3];

    for(int i=0, j=0; str[i] != '\0'; i++) {
        if( str[i] == 'n' ) {
            instructions[j][0] = 0;
        } else if( str[i] == 'a' ) {
            instructions[j][0] = 1;
        } else if( str[i] == 'j' ) {
            instructions[j][0] = 2;
        } else {
            printf("An error occurred on line %d\n", __LINE__);
            printf("Instruction read error\n");
            return 0;
        }

        i += 4;
        char ch[5];
        int k;
        for(k=0, i; str[i] != '\n' && str[i] != '\0'; k++, i++) {
            ch[k] = str[i];
        }
        ch[k] = '\0';
        instructions[j][1] = atoi(ch);
        instructions[j][2] = 0;
        j++;
    }

    part2(new_lines, instructions);
    //part1(new_lines, instructions);    
}

void part2(int lines, int instructions[][3]) {
    for(int i=0; i<lines; i++) {
        if( instructions[i][0] == 0 || instructions[i][0] == 2 ) {
            instructions[i][0] = instructions[i][0] + 2 & 0x2;
            if( !part1(lines, instructions) ) 
                break;
            // below for-loop needed because part1 can modify instruction array in this function
            for(int j=0; j<lines; j++) { 
                instructions[j][2] = 0;
            }
            instructions[i][0] = instructions[i][0] + 2 & 0x2;
        }
    }
}

int part1(int lines, int instructions[][3]) {
    int acc = 0, looped = 0;

    for(int i=0; i<lines; i++) {
        instructions[i][2]++;

        if( instructions[i][2] > 1 ) {
            looped = 1;
            break;
        }
        
        if( instructions[i][0] == 1 ) {
            acc += instructions[i][1];
        } 
        else if( instructions[i][0] == 2 ) {
            i += (instructions[i][1] - 1);
        } 
    }

    if( !looped ) printf("\n");
    printf("Accumulator = %d\n", acc);
    return looped;
}