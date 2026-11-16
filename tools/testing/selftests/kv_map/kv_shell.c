#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define PUT_RB 470
#define GET_RB 471
#define PUT_LINEAR 472
#define GET_LINEAR 473

int prog()
{
    char prompt[256];
    char cmd[32], arg1[128], arg2[128], arg3;
    unsigned long long key;

    fget(prompt, sizeof(prompt));
    int N = sscanf(prompt, "%s %llu %s %s %c", cmd, &key, arg1, arg2, &arg3);

    if(N<1){
        return 1;
    }

    if(strcmp(cmd, "exit") == 0){
        return 0;
    }

    else if(strcmp(cmd, "put") == 0){
        
    }
    else if(strcmp(cmp, "get") == 0){
        
    }
    else{
        printf("bad prompt\n");
        return 1;
    }
}

int main()
{
    while(prog());

    
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