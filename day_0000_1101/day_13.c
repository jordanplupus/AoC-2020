#include <stdio.h>
#include <stdlib.h>

void part1(int lines, int timestamp, int nums[]);
void part2(int lines, int nums[]);

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
    char file_name[] = "day_13.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");
    int lines = 1;
    int earliest_timestamp;
    int i, j, k;

    if( file ) {
        char ch = ' ';
        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            if( ch == '\n' ) {
                str[i] = '\0';
                earliest_timestamp = atoi(str);
                break;
            }
            str[i] = ch;
        }

        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
            if( ch == ',' ) 
                lines++;
        }
        str[i-1] = '\0';
    }

    int nums[lines];
    char ch[5];
    for(i=0, j=0, k=0; str[i] != '\0'; i++, j++) {
        if( str[i] == ',' ) {
            if( ch[j-1] != 'x' ) {
                ch[j] = '\0';
                nums[k] = atoi(ch);
                k++;
            }
            j = -1;
            continue;
        }

        ch[j] = str[i];
    }
    if( ch[j-1] != 'x' ) {
        ch[j] = '\0';
        nums[k] = atoi(ch);
        nums[k+1] = -1;
    }

    int nums_part2[lines];
    for(i=0, j=0, k=0; str[i] != '\0'; i++, j++) {
        if( str[i] == ',' ) {
            ch[j] = '\0';
            nums_part2[k] = atoi(ch);
            j = -1;
            k++;
            continue;
        }

        ch[j] = str[i];
    }
    ch[j] = '\0';
    nums_part2[k] = atoi(ch);

    part1(lines, earliest_timestamp, nums);
    part2(lines, nums_part2);

    return 0;
}

void part1(int lines, int timestamp, int nums[]) {
    printf("--- Part 1 ---\n\n");
    int earliest_bus = 2147483640;
    int bus_id;

    for(int i=0; nums[i] != -1 && i<lines; i++) {
        int j;
        for(j=0; j<timestamp; j+=nums[i]);
        if( earliest_bus > j ) {
            earliest_bus = j;
            bus_id = nums[i];
        }
    }

    printf("Earliest bus time: %d\n", earliest_bus);
    printf("Bus id: %d\n", bus_id);
    printf("Bus id * time to wait from arrival timestamp: %d\n", ((earliest_bus-timestamp) * bus_id));
}

/**
 * I don't think this uses Chinese Number Theorem.
 * Instead I figured out a way to make brute forcing this very fast. 
 * 
 * This gets the three largest numbers in the nums array, excluding the first number. 
 * Then it gets the first common number that all of these can % by. 
 * Then gets the second number after this first number. 
 * It subtracts the second by the first and stores this value in 'count_up'
 * 
 * After the above, it starts counting from the first common number found before
 * which is stored in 'first_first_num' and puts this value in 'first_num'
 * Then it increments 'first_num' by 'count_up' and checks for when 
 * all values % (this value + i) equal 0.
*/
void part2(int lines, int nums[]) {
    printf("\n--- Part 2 ---\n\n");
    long long first_num = nums[0];
    int largest[lines];
    long long first_first_num;
    int count_up;

    int largest_nums[3];
    largest_nums[0] = 0; largest_nums[1] = 0; largest_nums[2] = 0;
    for(int i=1; i<lines; i++) {
        if( largest_nums[2] < nums[i] && largest_nums[1] > nums[i] ) {
            largest_nums[2] = nums[i];
        }
        else if( largest_nums[1] < nums[i] && largest_nums[0] > nums[i] ) {
            largest_nums[2] = largest_nums[1];
            largest_nums[1] = nums[i];
        }
        else if( largest_nums[0] < nums[i] ) {
            largest_nums[2] = largest_nums[1];
            largest_nums[1] = largest_nums[0];
            largest_nums[0] = nums[i];
        }
    }

    printf("3 largest nums in array: ");
    for(int i=0; i<3; i++) {
        printf("%d, ", largest_nums[i]);
    }
    printf("\n\n");


    largest[0] = nums[0];
    for(int i=1; i<lines; i++) {
        if( nums[i] == largest_nums[0] || 
            nums[i] == largest_nums[1] || 
            nums[i] == largest_nums[2] )
            largest[i] = nums[i];
        else largest[i] = 0;
    }

    printf("Largest: ");
    for(int i=0; i<lines; i++) {
        printf("%d, ", largest[i]);
    }
    printf("\n\n");


    // get first common number between 3 largest values in array
    for(int i=1; i<lines; i++) {
        if( largest[i] == 0 ) continue;
        if( (first_num + i) % largest[i] == 0 ) continue;

        first_num += largest[0];

        if( first_num > 922340000000000000 ) {
            printf("An error occurred on line %d\n", __LINE__);
            printf("Over 63 bit limit\n");
            return;
        }

        i = 0;
    }

    // get second common number between 3 largest values in array
    first_first_num = first_num;
    first_num += largest[0];
    for(int i=1; i<lines; i++) {
        if( largest[i] == 0 ) continue;
        if( (first_num + i) % largest[i] == 0 ) continue;

        first_num += largest[0];

        if( first_num > 922340000000000000 ) {
            printf("An error occurred on line %d\n", __LINE__);
            printf("Over 63 bit limit\n");
            return;
        }

        i = 0;
    }


    count_up = first_num - first_first_num;
    printf("Value to count from:  %d\n", first_first_num);
    printf("Value to count up by: %d\n", count_up);

    // brute force from this common value for the full array
    first_num = first_first_num;
    for(int i=1; i<lines; i++) {
        if( nums[i] == 0 ) continue;
        if( (first_num + i) % nums[i] == 0 ) continue;

        first_num += count_up;

        if( first_num > 922340000000000000 ) {
            printf("An error occurred on line %d\n", __LINE__);
            printf("Over 63 bit limit\n");
            return;
        }

        i = 0;
    }

    printf("Ans: %llu\n", first_num);
}