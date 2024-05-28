#include <stdio.h>
#include <stdlib.h>

void part1(int lines, char actions[], int values[]);
void part2(int lines, char actions[], int values[]);

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
    char file_name[] = "day_12.txt";
    long int file_size = getFileSize(file_name);
    if( file_size == -1 ) {
        printf("File not found");
        return 0;
    }
    char str[file_size];
    FILE* file = fopen(file_name, "r");
    int new_lines = 1;
    int i, j, k;

    if( file ) {
        char ch = ' ';
        for(i=0; ch != EOF; i++) {
            ch = fgetc(file);
            str[i] = ch;
            if( ch == '\n' ) 
                new_lines++;
        }
    }

    char actions[new_lines];
    int values[new_lines];
    char ch[5];
    for(i=0, j=-1, k=0; str[i] != '\0'; i++, j++) {
        if( str[i] == '\n' ) {
            ch[j] = '\0';
            values[k] = atoi(ch);
            j = -2;
            k++;
            continue;
        }

        if( j == -1 ) {
            actions[k] = str[i];
            continue;
        }   

        ch[j] = str[i];
    }
    ch[j] = '\0';
    values[k] = atoi(ch);

    part1(new_lines, actions, values);
    part2(new_lines, actions, values);
}

void part1(int lines, char actions[], int values[]) {
    // ew = east, west; where east is positive and west is negative
    // ns = north, south; where north is positive and south is negative
    int ew = 0, ns = 0;
    // ship_direction is such that 0 = north, 1 = east, 2 = south, 3 = west
    int direction = 1; 

    for(int i=0; i<lines; i++) {
        int val = values[i];
        switch(actions[i]) {
            case 'F':
                if( direction > 1 ) 
                    val *= -1;
                if( direction == 0 || direction == 2 ) 
                    ns += val;
                else if( direction == 1 || direction == 3 ) 
                    ew += val;
                else {
                    printf("An error occurred on line %d\n", __LINE__);
                    printf("Invalid direction detected.\n");
                    return;
                }
                break;
            
            case 'L':
                val *= -1;
            case 'R':
                val /= 90;
                direction = (direction + val) & 0x3;
                break;

            case 'S':
                val *= -1;
            case 'N':
                ns += val;
                break;

            case 'W':
                val *= -1;
            case 'E':
                ew += val;
                break;

            default: 
                printf("An error occurred on line %d\n", __LINE__);
                printf("Action command not recognized\n");
                return;
        }
    }

    printf("North/South: %d,  East/West: %d\n", ns, ew);    
    if( ns >> 31 )
        ns = ~ns + 1;
    if( ew >> 31 ) 
        ew = ~ew + 1;

    int sum = ns + ew;
    printf("Sum of Manhattan distances: %d\n\n", sum);
}

void part2(int lines, char actions[], int values[]) {
    // wpNS = north, south direction of waypoint where pos is north and neg is south
    // wpWE = west, east direction of waypoint where pos is east and neg is west
    int wpNS = 1, wpWE = 10;

    // ns is north/south coords of ship where pos is north and neg is south
    // we is west/east coords of ship where pos is east and neg is west
    int ns = 0, we = 0;

    for(int i=0; i<lines; i++) {
        int val = values[i];
        int hold;
        switch(actions[i]) {
            case 'F':
                for(val; val > 0; val--) {
                    ns += wpNS;
                    we += wpWE;
                }
                break;

            case 'L':
                if( val != 180 && val != 360 )
                    val = (val + 180) % 360;
            case 'R':
                val /= 90;
                // north 4, east 10
                for(val; val > 0; val--) {
                    if( wpNS == 0 ) {
                        wpNS = (~wpWE + 1);
                        wpWE = 0;
                    }
                    else if( wpWE == 0 ) {
                        wpWE = wpNS;
                        wpNS = 0;
                    }
                    else if( wpNS > 0 && wpWE > 0 ) {        // north/east
                        hold = wpWE;
                        wpWE = wpNS;
                        wpNS = -hold;
                        // south 10, east 4
                    }
                    else if( wpNS < 0 && wpWE > 0 ) {   // south/east 
                        hold = wpNS;
                        wpNS = -wpWE;
                        wpWE = hold;
                        // west 10, south 4
                    }
                    else if( wpNS < 0 && wpWE < 0 ) {   // south/west
                        hold = wpWE;
                        wpWE = wpNS;
                        wpNS = -hold;
                        // north 10, west 4
                    }
                    else if( wpNS > 0 && wpWE < 0 ) {   // north/west
                        hold = wpNS;
                        wpNS = -wpWE;
                        wpWE = hold;
                        // east 10, north 4
                    }
                }
                break;

            case 'S':
                val *= -1;
            case 'N':
                wpNS += val;
                break;
            
            case 'W':
                val *= -1;
            case 'E':
                wpWE += val;
                break;

            default: 
                printf("An error occurred on line %d\n", __LINE__);
                printf("Action command not recognized\n");
                return;
        }
    }

    printf("North/South: %d,  East/West: %d\n", ns, we);
    if( ns >> 31 )
        ns = ~ns + 1;
    if( we >> 31 ) 
        we = ~we + 1;

    int sum = ns + we;
    printf("Sum of Manhattan distances (part2): %d\n\n", sum);
}