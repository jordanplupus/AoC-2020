#include <stdio.h>
#include <stdlib.h>

void part1(int line_length, int line_count, char groups[][line_length]);

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
    char file_name[] = "day_06.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");
    int blankLines = 1;
    int line_length = 200;

    if( file ) {
        char ch = ' ';
        int i;
        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
            if( str[i] == '\n' && str[i-1] == '\n' ) {
                blankLines++;
            }
        }
        str[i-1] = '\0';
    }

    // split string by blank lines
    char groups[blankLines][line_length];

    int i, j, k;
    for(i=0, j=0, k=0; str[i]!='\0'; i++, j++) {
        if( j >= line_length ) {
            printf("An error occurred on line %d\n", __LINE__);
            return 0;
        }

        if( str[i] == '\n' && str[i+1] == '\n' ) {
            groups[k][j] = '\0';
            j = -1;
            k++;
            i++;
        }
        else {
            groups[k][j] = str[i];
        }
    }

    groups[k][j] = '\0';

    part1(line_length, blankLines, groups);
}

void part1(int line_length, int line_count, char groups[][line_length]) {
    int chars[26];
    int sum = 0, sum2 = 0;
    int people = 1;

    for(int i=0; i<26; i++)
        chars[i] = 0;

    for(int i=0; i<line_length; i++) {
        if( groups[0][i] == '\0' )
            break;
        if( groups[0][i] == '\n' ) 
            people++;
    }

    for(int i=0, j=0; i<line_count; j++) {
        if( groups[i][j] == '\0' ) {
            for(int k=0; k<26; k++) {
                if( chars[k] > 0 )
                    sum++;
                if( chars[k] == people )
                    sum2++;
                chars[k] = 0;
            }

            // reset and increment values for next group
            i++;
            j = -1;
            people = 1;
            for(int k=0; k<line_length; k++) {
                if( groups[i][k] == '\0' )
                    break;
                if( groups[i][k] == '\n' ) 
                    people++;
            }
        }

        else if( groups[i][j] != '\n' ) {
            if( groups[i][j] < 'a' || groups[i][j] > 'z' ) {
                printf("An error occurred on line %d\n", __LINE__);
                printf("Character was: %c\n", groups[i][j]);
                exit(0);
            }
            chars[groups[i][j] - 'a']++;
        }    
    }

    printf("Sum: %d\n", sum);
    printf("Sum part 2: %d\n", sum2);
}