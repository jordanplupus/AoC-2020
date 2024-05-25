#include <stdio.h>
#include <stdlib.h>

void part1(int lines, int nums[]);
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
    char file_name[] = "day_10.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");
    int new_lines = 2;

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

    char c[5];
    int nums[new_lines];
    nums[0] = 0;
    int i, j, k;
    for(i=0, j=0, k=1; str[i] != '\0'; i++, j++) {
        if( str[i] == '\n' ) {
            c[j] = '\0';
            nums[k] = atoi(c);
            j = -1;
            k++;
            continue;
        }
        c[j] = str[i];
    }
    c[j] = '\0';
    nums[k] = atoi(c);

    for(i=0, j=0; i<new_lines; i++) {
        if( i == new_lines-1 ) {
            if( j == 1 ) {
                i = -1;
                j = 0;
                continue;
            }
            else break;
        }
        if( nums[i] > nums[i+1] ) {
            int s = nums[i];
            nums[i] = nums[i+1];
            nums[i+1] = s;
            j = 1;
        }
    }

    part1(new_lines, nums);
    part2(new_lines, nums);
}


void part1(int lines, int nums[]) {
    int prevNum = 0;
    int diff1 = 0, diff3 = 1;

    for(int i=1; i<lines; i++) {
        if     ( nums[i] - prevNum == 1 ) diff1++;
        else if( nums[i] - prevNum == 3 ) diff3++;
        prevNum = nums[i]; 
    }

    printf("Differences quantities multiplied together: %d\n", (diff1 * diff3));
}


// Ok so I couldn't solve this one after extensive headache math in permutations.
// I looked up this solution and it worked: 
// https://fasterthanli.me/series/advent-of-code-2020/part-10
// I don't fully understand why this works unfortunately. 
void part2(int lines, int nums[]) {

    // The possible paths each node can take
    int possibilities[lines];

    for(int i=0; i<lines; i++) {
        possibilities[i] = 1;
    }

    for(int i=0; i<lines-2; i++) {
        if( nums[i+1] - nums[i] == 1 && nums[i+2] - nums[i+1] == 1 && nums[i+3] - nums[i+2] == 1 ) {
            possibilities[i]+=2;
        }
        else if( (nums[i+1] - nums[i] == 1 && nums[i+2] - nums[i+1] == 2) ||
                 (nums[i+1] - nums[i] == 2 && nums[i+2] - nums[i+1] == 1) ||
                 (nums[i+1] - nums[i] == 1 && nums[i+2] - nums[i+1] == 1) ) {
            possibilities[i]++;
        }
    }

    long long node[lines];
    node[lines-1] = 1;
    for(int i=lines-2; i>=0; i--) {
        if( possibilities[i] >= 1 ) {
            node[i] = node[i+1];
        }
        if( possibilities[i] >= 2 ) {
            node[i] += node[i+2];
        }
        if( possibilities[i] >= 3 ) {
            node[i] += node[i+3];
        }
        printf("Val at %d:  %llu\n", i, node[i]);
    }
}

/* // old attempt
void part2(int lines, int nums[]) {
    int possibilities[lines];

    for(int i=0; i<lines; i++) {
        possibilities[i] = 1;
    }

    int adjacents = 0;
    int total = 0;
    for(int i=0; i<lines-2; i++) {
        if( nums[i+1] - nums[i] == 1 && nums[i+2] - nums[i+1] == 1 && nums[i+3] - nums[i+2] == 1 ) {
            possibilities[i]+=2;
            total++;
        }
        else if( (nums[i+1] - nums[i] == 1 && nums[i+2] - nums[i+1] == 2) ||
                 (nums[i+1] - nums[i] == 2 && nums[i+2] - nums[i+1] == 1) ||
                 (nums[i+1] - nums[i] == 1 && nums[i+2] - nums[i+1] == 1) ) {
            possibilities[i]++;
            total++;
        }
    }

    int mult = 1;
    for(int i=0; i<lines; i++) {
        mult *= possibilities[i];
        if( possibilities[i] > 1 ) {
            if( possibilities[i+1] > 1 ) {
                adjacents++;
            }
            if( possibilities[i+2] > 1 ) {
                adjacents++;
            }
        }
    }

    int repeat = mult / total;
    for(int i=0; i<adjacents; i++) {
        mult -= repeat;
    }

    printf("Total possibilities: %d\n", mult);
}*/