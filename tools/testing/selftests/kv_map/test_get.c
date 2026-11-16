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
    char buffer[128];
    
    scanf("%s %llu", cmd, &key);

    if(strcmp(cmd, "linear") == 0){
        long ret = syscall(GET_LINEAR, key, buffer, sizeof(buffer));

        if(ret == -1){
            perror("(linear)get error\n");
            return 0;
        }

        if(printf("(linear)get%llu: %s (len: %ld)\n", key, buffer, ret));
    }
    else{
        long ret = syscall(GET_RB, key, buffer, sizeof(buffer));

        if(ret == -1){
            perror("get error\n");
            return 0;
        }

        if(printf("get%llu: %s (len: %ld)\n", key, buffer, ret));
    }

    return 0;
}