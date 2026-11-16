#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define PUT_RB 470
#define GET_RB 471
#define PUT_LINEAR 472
#define GET_LINEAR 473

#define MAX_N 100000
#define VALUE_LEN 16

void set_random_str(char *str, int len)
{
    for(int i = 0; i<len; i++){
        str[i] = 'a'+ rand()%26;
    }
    str[len] = '\0';
}

int main()
{
    srand((unsigned int)time(NULL));

    int N;
    char cmd[16];
    printf("type, N: ");
    scanf("%s %d", cmd, &N);
    
    if(N>MAX_N){
        printf("bad input\n");
        return 0;
    }

    int keys[MAX_N];
    char values[MAX_N][VALUE_LEN+1];
    char buffer[VALUE_LEN+1];
    
    struct timespec start, end;
    long long elapsed = 0LL;

    for(int i = 0; i<N; i++){
        keys[i] = rand();
        set_random_str(values[i], VALUE_LEN);
    }

    long put_nr = (strcmp(cmd, "rb") == 0) ? PUT_RB : PUT_LINEAR;
    long get_nr = (strcmp(cmd, "rb") == 0) ? GET_RB : GET_LINEAR;

    //put
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0; i<N; i++){
        if(syscall(put_nr, keys[i], values[i]) == -1){
            printf("%s_put error\n", (strcmp(cmd, "rb") == 0)?"rb":"linear");
            return 0;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed = (end.tv_sec - start.tv_sec)*1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("%s_put: %.6f ms\n", (strcmp(cmd, "rb") == 0)?"rb":"linear",elapsed/1e6);

    for(int i = N-1; i>0; i--){
        int j = rand()%(i+1);

        int temp = keys[i];
        keys[i] = keys[j];
        keys[j] = temp;
    }

    //get
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(int i = 0; i<N; i++){
        if(syscall(get_nr, keys[i], buffer) == -1){
            printf("%s_get error\n", (strcmp(cmd, "rb") == 0)?"rb":"linear");
            return 0;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    elapsed = (end.tv_sec - start.tv_sec)*1000000000LL + (end.tv_nsec - start.tv_nsec);
    printf("%s_get: %.6f ms\n", (strcmp(cmd, "rb") == 0)?"rb":"linear",elapsed/1e6);
}