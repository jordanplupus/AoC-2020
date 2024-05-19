#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part1(char str[]);

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
    char file_name[] = "day_02.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");

    if( file ) {
        char ch = ' ';
        int i;
        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
        }
        str[i-1] = '\0';
    }
    fclose(file);

    part1(str);
}

void part1(char str[]) {
    int i = 0;
    int nums[2];
    int valid_pass = 0, valid_pass_2 = 0;
    
    while(str[i] != '\0') {
        char ch[3];
        int j, k;
        for(i, j=0, k=0; str[i] != '\0'; i++, j++) {
            if( str[i] != '-' && str[i] != ' ' ) {
                ch[j] = str[i];
            }
            else if( str[i] == '-' ) {
                ch[j] = '\0';
                nums[k] = atoi(ch);
                j = -1;
                k++;
            }
            else if( str[i] == ' ' ) {
                ch[j] = '\0';
                nums[k] = atoi(ch);
                break;
            }
            else {  // this may never be able to happen
                printf("An error occurred on line %d\n", __LINE__);
                return;
            }
        }

        i++;
        char check = str[i];
       
        i+=2;
        if( (str[nums[0] + i] == check && str[nums[1] + i] != check) ||
            (str[nums[0] + i] != check && str[nums[1] + i] == check) ) {
                valid_pass_2++;
        }

        i++;
        int counter = 0;
        for(i; str[i] != '\n' && str[i] != '\0'; i++) {
            if( str[i] == check )
                counter++;
        }

        if( counter >= nums[0] && counter <= nums[1] ) {
            valid_pass++;
        }

        if( str[i] == '\0' ) break;
        i++;    // skip over new line to next line to parse
    }

    printf("Valid Pass: %d\n", valid_pass);
    printf("Valid Pass 2: %d\n", valid_pass_2);
}