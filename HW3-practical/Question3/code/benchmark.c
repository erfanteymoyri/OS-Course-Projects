#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_GETPID 39

#define SYS_SET_MY_FIELD 549
#define SYS_GET_MY_FIELD 550
#define SYS_GET_TIME_NS 551
#define SYS_GET_LAST_SYSCALL_TIMES 552
/* ---------------------------------- */

int main()
{
    unsigned long long t1, t4;
    long ret_val;
    char buf[10];
    pid_t my_pid;

    printf("--- Start Benchmark ---\n\n");

    // --- Test1: GETPID ---
    t1 = syscall(SYS_GET_TIME_NS); 
    ret_val = syscall(SYS_GETPID); 
    t4 = syscall(SYS_GET_TIME_NS); 
    my_pid = ret_val; 
    syscall(SYS_GET_LAST_SYSCALL_TIMES);
    printf("--- getpid Test (PID=%ld) ---\n", my_pid);
    printf("Total User-Perceived Time (T4 - T1): %llu ns\n", (t4 - t1));

    // --- Test2: SET_MY_FIELD ---
    t1 = syscall(SYS_GET_TIME_NS); 
    ret_val = syscall(SYS_SET_MY_FIELD, 999); 
    t4 = syscall(SYS_GET_TIME_NS); 
    syscall(SYS_GET_LAST_SYSCALL_TIMES);
    printf("\n--- set_my_field Test ---\n");
    printf("Total User-Perceived Time (T4 - T1): %llu ns\n", (t4 - t1));

    // --- Test3: GET_MY_FIELD ---
    t1 = syscall(SYS_GET_TIME_NS); 
    ret_val = syscall(SYS_GET_MY_FIELD); 
    t4 = syscall(SYS_GET_TIME_NS); 
    syscall(SYS_GET_LAST_SYSCALL_TIMES);
    printf("\n--- get_my_field Test (Val=%ld) ---\n", ret_val);
    printf("Total User-Perceived Time (T4 - T1): %llu ns\n", (t4 - t1));

    // --- Test4: read (0 bytes) ---
    t1 = syscall(SYS_GET_TIME_NS); 
    ret_val = syscall(SYS_READ, 0, buf, 0); 
    t4 = syscall(SYS_GET_TIME_NS); 
    syscall(SYS_GET_LAST_SYSCALL_TIMES);
    printf("\n--- read (0 bytes) Test ---\n");
    printf("Total User-Perceived Time (T4 - T1): %llu ns\n", (t4 - t1));

    // --- Test5: write (5 bytes) ---
    t1 = syscall(SYS_GET_TIME_NS); 
    ret_val = syscall(SYS_WRITE, 1, "\ntest\n", 5);
    t4 = syscall(SYS_GET_TIME_NS); 
    syscall(SYS_GET_LAST_SYSCALL_TIMES);
    printf("--- write (5 bytes) Test ---\n");
    printf("Total User-Perceived Time (T4 - T1): %llu ns\n", (t4 - t1));

    return 0;
}
