#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define PUT_RB 470
#define GET_RB 471
#define PUT_LINEAR 472
#define GET_LINEAR 473

int main()
{
    char cmd[128];
    unsigned long long key = 0;
    char value[128];

    scanf("%s %llu %s", cmd, &key, value);

    if(strcmp(cmd, "linear") == 0){
        long ret = syscall(PUT_LINEAR, key, value);
        if(ret == -1){
            perror("(linear)put error\n");
            return 0;
        }

        printf("(linear)put success: %llu\n", key);
    }
    else{
        long ret = syscall(PUT_RB, key, value);

        if(ret == -1){
            perror("put error\n");
            return 0;
        }

        printf("put success: %llu\n", key);
    }

    return 0;
}