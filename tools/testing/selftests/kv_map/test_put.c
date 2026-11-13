#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

int main()
{
    long ret = syscall(470, 1234, "Hello");

    if(ret == -1){
        perror("put error\n");
        return 0;
    }

    printf("put success: %ld\n", 1234);
    return 0;
}