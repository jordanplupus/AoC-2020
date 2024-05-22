#include <stdio.h>
#include <stdlib.h>

void part1(int find_bag, int lines, int length, int sub_bags[][length]);
int part2(int bag_index, int bag_count, int length, int sub_bags[][length], int sub_bag_count[][length]);
int getStringIndex(int lines, int strLen, char bags[][strLen], char str[]);
void printBags(int lines, int char_length, int int_length, char bags[][char_length], int sub_bags[][int_length], int sub_bag_count[][int_length]);

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
    char file_name[] = "day_07.txt";
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
    }

    // --- get character array of bag strings ---
    char bags[new_lines][string_length];

    for(int i=0, j=0, k=0, l=0; str[i] != '\0'; i++, l++) {
        if( str[i] == '\n' ) {
            j = 0;
            k++;
            l = -1;
            continue;
        }

        if( j < 2 ) {
            if( str[i] == ' ' ) {
                j++;
                if( j == 2 ) {
                    bags[k][l] = '\0';
                    continue;
                }
            }
            bags[k][l] = str[i];
        }
        else continue;
    }

    // --- get all bag sub-contents and put the indexes of those bags in the bags
    //     character array into a 2d int array where x is the index of the main
    //     bag from the char array, and y contains the index of the sub-bags that
    //     the main bag can contain ---
    int sub_bags[new_lines][sub_bags_length];
    int sub_bag_count[new_lines][sub_bags_length];
    char ch[string_length];

    for(int i=0, j=0, k=0, sl = 0, l=0; str[i] != '\0'; i++, l++) {
        if( str[i] == '\n' ) {
            j = 0;
            k++;
            sl = 0;
            sub_bags[k][sl] = -1;
            sub_bag_count[k][sl] = -1;
        }

        if( j % 3 == 0 ) {
            if( str[i] < '0' || str[i] > '9' ) 
                continue;
            else {
                sub_bag_count[k][sl] = (int)str[i] - '0';
                sub_bag_count[k][sl+1] = -1;
                sub_bags[k][sl+1] = -1;
                j++;
                i+=2;
                l = 0;
            }
        }

        if( str[i] == ' ' ) {
            j++;
            if( j % 3 == 0 ) {
                ch[l] = '\0';
                int index = getStringIndex(new_lines, string_length, bags, ch);

                if( index == -1 ) {
                    printf("An error occurred on line %d\n", __LINE__);
                    printf("The index of %s should be contained in the ", ch);
                    printf("bags character array but it is not\n");
                    printf("Line %d in text file\n", k);
                    return 0;
                }

                sub_bags[k][sl] = index;
                sl++;
                continue;
            }
        }
        ch[l] = str[i];
    }

    // uncomment below to print all bags and the indexes of the 
    // sub-bags that they can contain
    //printBags(new_lines, string_length, sub_bags_length, bags, sub_bags, sub_bag_count);

    int find_bag = getStringIndex(new_lines, string_length, bags, "shiny gold");
    if( find_bag == -1 ) {
        printf("An error occurred on line %d\n", __LINE__);
        printf("Specified bag could not be found\n");
        return 0;
    }

    part1(find_bag, new_lines, sub_bags_length, sub_bags);
    int bags_in_bag = part2(find_bag, 0, sub_bags_length, sub_bags, sub_bag_count);
    printf("The specified bag can hold %d bags\n", bags_in_bag);

    return 0;
}

/**
 * Gets all bags that can hold the shiny gold bag. 
*/
void part1(int find_bag, int lines, int length, int sub_bags[][length]) {
    int can_hold[lines];
    int can_holds = 0;
    for(int i=0; i<lines; i++) {
        can_hold[i] = 0;
    }

    can_hold[find_bag] = 1;

    int iterate = 1;
    while(iterate) {
        iterate = 0;
        for(int i=0; i<lines; i++) {
            if( can_hold[i] == 1 || can_hold[i] == 2 ) 
                continue;

            iterate = 1;
            int mark_2 = 1;
            for(int j=0; sub_bags[i][j] != -1; j++) {
                if( can_hold[sub_bags[i][j]] == 1 ) {
                    can_hold[i] = 1;
                    mark_2 = 0;
                    can_holds++;
                    break;
                }
                if( can_hold[sub_bags[i][j]] == 0 )
                    mark_2 = 0;
            }
            if( mark_2 ) 
                can_hold[i] = 2;
        }
    }

    printf("\n%d bags can contain the specified bag\n", can_holds);
}

/**
 * Uses recursion to get all the bags that the shiny gold bag can contain.
*/
int part2(int bag_index, int bag_count, int length, int sub_bags[][length], int sub_bag_count[][length]) {
    for(int i=0; sub_bags[bag_index][i] != -1; i++) {
        bag_count += sub_bag_count[bag_index][i];
        bag_count += sub_bag_count[bag_index][i] * part2(sub_bags[bag_index][i], 0, length, sub_bags, sub_bag_count);
    }
    return bag_count;
}

int getStringIndex(int lines, int strLen, char bags[][strLen], char str[]) {
    for(int i=0; i<lines; i++) {
        int j;
        for(j=0; bags[i][j] != '\0'; j++) {
            if( bags[i][j] != str[j] )
                break;
            if( str[j] == '\0' )
                break;
        }
        if( bags[i][j] == '\0' && str[j] == '\0' ) 
            return i;
    }
    return -1;
}

void printBags(int lines, int char_length, int int_length, char bags[][char_length], int sub_bags[][int_length], int sub_bag_count[][int_length]) {
    for(int i=0; i<lines; i++) {
        printf("%d: %s bags contain bag indexes:  ", i, bags[i]);
        for(int j=0; sub_bags[i][j] != -1; j++) {
            printf("(%d) bags of ", sub_bag_count[i][j]);
            printf("%d, ", sub_bags[i][j]);
        }
        printf("\n");
    }
}