#include <stdio.h>
#include <stdlib.h>

struct Node {
    long long data;
    struct Node* next;
};
typedef struct Node Queue;

void part1(int size, int preamble, long long nums[]);
void enqueue(Queue** head, Queue** tail, long long data);
long long dequeue(Queue** head, Queue** tail);
void releaseNodes(Queue** head, Queue** tail);

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
    long long nums[new_lines];
    int i, j, k;
    for(i=0, j=0, k=0; str[i] != '\0'; i++, j++) {
        if( str[i] == '\n' ) {
            n[j] = '\0';
            nums[k] = atoll(n);
            k++;
            j = -1;
            continue;
        }
        n[j] = str[i];
    }
    n[j] = '\0';
    nums[k] = atoll(n);

    part1(new_lines, preamble_length, nums);
}

void part1(int size, int preamble, long long nums[]) {
    Queue* head;
    Queue* tail;

    head = tail = NULL;

    int i;
    long long sum = 0, target;

    for(i=0; i<preamble; i++) {
        enqueue(&head, &tail, nums[i]);
    }

    for(i; i<size; i++) {
        Queue* cursor = head;
        while(sum != nums[i] && cursor != NULL) {
            Queue* cursori = cursor->next;
            while(sum != nums[i] && cursori != NULL) {
                sum = cursor->data + cursori->data;
                cursori = cursori->next;
            }
            if( sum != nums[i] )
                cursor = cursor->next;
        }
        if( cursor == NULL ) {
            printf("Could not find two preamble numbers that sum to %llu\n", nums[i]);
            target = nums[i];
            break;
        }

        dequeue(&head, &tail);
        enqueue(&head, &tail, nums[i]);
    }

    releaseNodes(&head, &tail);

    for(i=0; i<size; i++) {
        sum = nums[i]; 
        for(int j=i+1; sum<target && j<size; j++) {
            sum += nums[j];
            if( sum == target ) {
                long long min = 922340000000000000;
                long long max = 0;
                for(int k=i; k<=j; k++) {
                    if( min > nums[k] )
                        min = nums[k];
                    if( max < nums[k] ) 
                        max = nums[k];
                }
                printf("Min + Max = %llu\n", (min + max));
            }
        }
        if( sum == target) 
            break;
    }
}

void enqueue(Queue** head, Queue** tail, long long data) {
    Queue* cursor = *tail;

    Queue* newNode = (Queue*) malloc(sizeof(Queue));    

    newNode->data = data;
    newNode->next = NULL;
    
    if( *head == NULL ) {
        *head = *tail = newNode;
        return;
    }

    cursor->next = newNode;
    *tail = newNode;
}

long long dequeue(Queue** head, Queue** tail) {
    Queue* oblivion = *head;

    int data = oblivion->data;

    if( head == tail ) 
        *head = *tail = NULL;
    else 
        *head = oblivion->next;

    free(oblivion);

    return data;
}

void releaseNodes(Queue** head, Queue** tail) {
    Queue* cursor = *head;
    Queue* next;

    while( cursor != NULL ) {
        next = cursor->next;
        free(cursor);
        cursor = next;
    }

    head = tail = NULL;
}