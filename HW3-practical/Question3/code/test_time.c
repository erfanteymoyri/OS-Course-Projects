#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYS_GET_TIME_NS 551 

int main()
{
    unsigned long long time_ns = syscall(SYS_GET_TIME_NS);

    printf("Current kernel time (ns): %llu\n", time_ns);
    return 0;
}