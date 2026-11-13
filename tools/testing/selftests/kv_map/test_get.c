#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

int main()
{ 
    char buffer[128];

    long ret = syscall(471, 1234, buffer, sizeof(buffer));

    if(ret == -1){
        perror("get error\n");
        return 0;
    }

    if(printf("get%ld: %s (len: %ld)\n", 1234, buffer, ret));
    return 0;
}