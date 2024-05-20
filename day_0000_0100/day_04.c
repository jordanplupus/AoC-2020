#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void part2(int charLimit, int checkLimit, int passportCount, char passports[][charLimit], char checks[][5]);
int isValid(char passport[], char check[], int j);

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
    char file_name[] = "day_04.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");
    int blankLines = 0, charLimit = 500;

    if( file ) {
        char ch = ' ';
        int i;
        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
            if( str[i] == '\n' && str[i-1] == '\n' ) 
                blankLines++;
        }
        str[i-1] = '\0';
    }

    // split string
    char passports[blankLines + 1][charLimit];
    for(int i=0, j=0, k=0; i<file_size; i++, k++) {
        if( j >= charLimit ) {
            printf("An error occurred on line %d\n", __LINE__);
            return 0;
        }

        if( str[i] == '\n' && str[i-1] == '\n' ) {
            passports[j][k] = '\0';
            j++;
            k = -1;
        }
        else passports[j][k] = str[i];
    }

    char checks[8][5] = {{"byr\0"}, {"iyr\0"}, {"eyr\0"}, {"hgt\0"}, 
                         {"hcl\0"}, {"ecl\0"}, {"pid\0"}, {"cid\0"}};

    // part 2 added into part 1 so no part 1
    part2(charLimit, 7, blankLines + 1, passports, checks);

    return 0;
}

void part2(int charLimit, int checkLimit, int passportCount, char passports[][charLimit], char checks[][5]) {
    int valid_passports = 0;
    int valid = 1;
    
    for(int i=0; i<passportCount; i++) {
        for(int j=0; j<checkLimit; j++) {
            if( !isValid(passports[i], checks[j], j) ) {
                valid = 0;
                break;
            }
        }

        if( valid ) {
            valid_passports++;
        }
        valid = 1;
    }

    printf("Valid passports: %d\n", valid_passports);
}

/**
 * Check if a passport is valid or not based on the part 2 criteria. 
*/
int isValid(char passport[], char check[], int j) {
    int k;
    for(k=2; passport[k] != '\0'; k++) {
        if( check[0] == passport[k-2] && 
            check[1] == passport[k-1] && 
            check[2] == passport[k] )
            break;
    }

    if( passport[k] == '\0' ) 
        return 0;

    // regex check portion
    int lower, upper;
    char ch[5];
    int i;
    switch(j) {
        case 0: // byr: valid 1920 - 2002
            lower = 1920;
            upper = 2002;
            break;

        case 1: // iyr: valid 2010 - 2020
            lower = 2010;
            upper = 2020;
            break;

        case 2: // eyr: valid 2020 - 2030
            lower = 2020;
            upper = 2030;
            break;

        case 3: // hgt: valid 150-193cm or 59-76in
            k+=2;
            int i;
            for(k, i=0; passport[k] != '\0'; k++, i++) {
                if( passport[k] >= '0' && passport[k] <= '9' )
                    ch[i] = passport[k];
                else {
                    ch[i] = '\0';
                    break;
                }
            }
            int hgt = atoi(ch);
            if( passport[k] == 'c' && hgt >= 150 && hgt <= 193 ) 
                return 1;
            if( passport[k] == 'i' && hgt >= 59 && hgt <= 76 ) 
                return 1;
            return 0; 

        case 4: // hcl: valid # followed by 0-9 and a-f
            k+=2;
            if( passport[k] != '#' ) 
                return 0;
            k++;
            for(k; passport[k] != '\0'; k++) {
                if( !(passport[k] >= '0' && passport[k] <= '9') &&
                    !(passport[k] >= 'a' && passport[k] <= 'f') ) {
                        if( passport[k] != '\n' && passport[k] != ' ' )
                            return 0;
                        return 1;
                    } 
            }
            return 1;

        case 5: // ecl: valid amb, blu, brn, gry, grn, hzl, oth
            k+=2;
            ch[0] = passport[k];
            ch[1] = passport[k+1];
            ch[2] = passport[k+2];
            if     ( ch[0] == 'a' && ch[1] == 'm' && ch[2] == 'b' ) return 1;
            else if( ch[0] == 'b' && ch[1] == 'l' && ch[2] == 'u' ) return 1;
            else if( ch[0] == 'b' && ch[1] == 'r' && ch[2] == 'n' ) return 1;
            else if( ch[0] == 'g' && ch[1] == 'r' && ch[2] == 'y' ) return 1;
            else if( ch[0] == 'g' && ch[1] == 'r' && ch[2] == 'n' ) return 1;
            else if( ch[0] == 'h' && ch[1] == 'z' && ch[2] == 'l' ) return 1;
            else if( ch[0] == 'o' && ch[1] == 't' && ch[2] == 'h' ) return 1;
            return 0;

        case 6: // pid: valid 9 digit number
            k+=2;
            for(k, i=0; i<9; k++, i++) {
                if( !(passport[k] >= '0' && passport[k] <= '9') )
                    return 0;
            }
            if( passport[k] >= '0' && passport[k] <= '9' )
                return 0;
            return 1;

        default: 
            printf("An error occurred on line %d\n", __LINE__);
            exit(0);
    }

    // reaching here means that it is case 0-2
    k+=2;
    for(k, i=0; passport[k] != '\0'; k++, i++) {
        if( passport[k] >= '0' && passport[k] <= '9' )
            ch[i] = passport[k];
        else {
            ch[i] = '\0';
            break;
        }
    }
    int chk = atoi(ch);
    if( chk >= lower && chk <= upper )
        return 1;
    return 0;
}