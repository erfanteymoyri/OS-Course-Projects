#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main()
{
    long ret = syscall(548); 

    printf("System call returned %ld\n", ret);
    return 0;
}