#include <stdio.h>
#include <stdlib.h>

void part1(int find_bag, int lines, int x, int y, int bag_indexes[], int bags[][y]);
void printBagsAsIntegerVals(int index_max, int y, int bag_type[], int bags_contained[][y]);

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
    int new_lines = 1;
    int bags_size_x = 20000, bags_size_y = 10;

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

    // split strings (convert to integers) 

    int bag_indexes[new_lines];
    
    /**
     * stores each bag type integer and their subbags at the index of the integer. 
     * Ex: if bag is int 10, and contains bags 15 and 20, 
     *     stores into bags as such:
     *     bags[10][0] = 10; bags[10][1] = 15; bags[10][2] = 20; bags[10][3] = -1 
    */
    int bags[bags_size_x][bags_size_y];
    
    int char_sum = 0, index = 0;

    for(int i=0; i<bags_size_x; i++) {
        bags[i][0] = -1;
    }

    for(int i=0, j=0, k=0, n=0; str[i] != '\0'; i++) {
        if( str[i] == '\n' ) {
            j = 0;
            n = 0;
            k++;
            index = 0;
            continue;
        }

        // below portion adds the bag type 
        if( j < 2 ) {
            index += str[i]; // adding character values
            if( str[i] == ' ' ) {
                j++;
                if( j == 2 ) {  // stop adding once 2 spaces encountered
                    if( index > bags_size_x ) {
                        printf("An error occurred on line %d\n", __LINE__);
                        printf("Bag index %d is too large a value.\n", index);
                        return 0;
                    }
                    if( bags[index][n] != -1 ) {
                        printf("An error occurred on line %d\n", __LINE__);
                        printf("Index %d already contains the data %d\n", index, bags[index][n]);
                        printf("line in text file is %d\n", k);
                        return 0;
                    }
                    bag_indexes[k] = index;
                    bags[index][n] = index;
                    bags[index][n+1] = -1;
                    i += 13;
                    j++;
                    n++;
                    if( str[i+1] == 'n' ) {   // skip on 'contains no other bags'
                        i += 13;
                    }
                }
            }
            continue;
        }

        // iterate until a number is reached
        if( j % 3 == 0 ) {
            if( str[i] >= '0' && str[i] <= '9' ) {
                j++;
                i+=2;
            }
            else continue;
        }

        // below portion adds the bags contained in the above bag type
        char_sum += str[i];
        if( str[i] == ' ' ) j++;
        if( j % 3 == 0 ) {
            bags[index][n] = char_sum;
            bags[index][n+1] = -1;
            char_sum = 0;
            n++;
        }
    }// end giant above for-loop

    // uncomment below to see all bags converted to integers
    //printBagsAsIntegerVals(new_lines, bags_size_y, bag_indexes, bags);

    char_sum = 0;
    char find[] = "shiny gold \0";
    for(int i=0; find[i] != '\0'; i++) {
        char_sum += find[i];
    }

    printf("\nFind bag %d\n\n", char_sum);

    part1(char_sum, new_lines, bags_size_x, bags_size_y, bag_indexes, bags);
}

void part1(int find_bag, int lines, int x, int y, int bag_indexes[], int bags[][y]) {
    int can_hold[x];
    int can_holds = 0;
    for(int i=0; i<lines; i++) 
        can_hold[bag_indexes[i]] = 0;
    
    can_hold[find_bag] = 1;
    
    int iterate = 2;
    while(iterate != 0) {
        iterate = 0;
        for(int i=0; i<lines; i++) {
            //printf("%d, ", can_hold[bag_indexes[i]]);
            if( can_hold[bag_indexes[i]] == 1 || can_hold[bag_indexes[i]] == 2 ) 
                continue;
            
            iterate = 1;
            int mark_2 = 1;
            for(int j=1; bags[bag_indexes[i]][j] != -1; j++) {
                if( can_hold[bags[bag_indexes[i]][j]] == 1 ) {
                    can_hold[bags[bag_indexes[i]][0]] = 1;
                    mark_2 = 0;
                    can_holds++;
                    break;
                }
                if( can_hold[bags[bag_indexes[i]][j]] == 0 )
                    mark_2 = 0;
            }
            if( mark_2 ) // mark the bag as doesn't contain 
                can_hold[bags[bag_indexes[i]][0]] = 2;
        }
        
        //printf("\n");
    }

    printf("Bag can be held in %d other bags.\n", can_holds);
}

/**
 * Prints the contents of all bags and their sub-bags
 * int x = new lines
 * int y = y length of the second array
*/
void printBagsAsIntegerVals(int index_max, int y, int bag_indexes[], int bags[][y]) {
    printf("All bag types converted to integers:\n\n");
    for(int i=0; i<index_max; i++) {
        printf("Bag %d contains:  ", bag_indexes[i]);
        for(int j=1; bags[bag_indexes[i]][j] != -1; j++) {
            printf("%d, ", bags[bag_indexes[i]][j]);
        }
        printf("\n");
    }
}