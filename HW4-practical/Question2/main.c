#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <time.h>
#include <errno.h>

enum COMMAND {
    NONE = 0,
    RUN = 10,
    EXEC = 11,
};

struct config {
    enum COMMAND subcommand;
    char name[64];
    char command[256];
};

int validate_config(struct config cfg) {
    if (cfg.subcommand == NONE) {
        fprintf(stderr, "[ERR] Mssing subcommand (run|exec)\n");
        return 1;
    }

    if (strcmp(cfg.name, "") == 0) {
        strncpy(cfg.name, "bib", sizeof(cfg.name)-1);
    }

    if (strcmp(cfg.command, "") == 0) {
        fprintf(stderr, "[ERR] Mssing command (e.g. 'sleep 1000')\n");
        return 1;
    }
    return 0;
}

void setup_time_offsets() {
    struct timespec now;
    
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
        perror("clock_gettime failed");
        return;
    }

    // Correctly calculate the negative offset for the kernel
    long sec = -(now.tv_sec + 1);
    long nsec = 1000000000L - now.tv_nsec;

    FILE *f = fopen("/proc/self/timens_offsets", "w");
    if (f == NULL) {
        // This is expected if not running as root or with CAP_SYS_ADMIN
        perror("[ERROR] Failed to open /proc/self/timens_offsets"); 
        return;
    }

    // Write the corrected offset for monotonic and boottime clocks
    fprintf(f, "monotonic %ld %ld\n", sec, nsec);
    fprintf(f, "boottime %ld %ld\n", sec, nsec);
    
    fflush(f); 
    fclose(f);
}


int run_container(struct config cfg) {
    pid_t pid;

    if (unshare(CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWUTS | CLONE_NEWTIME) != 0) {
    	fprintf(stderr, "[ERR] Failed to unshare(2).");
   		return 1;
    }
    setup_time_offsets();

    pid = fork();
    if (pid < 0) {
        return 1;
    }
    if (pid == 0) {
	if (sethostname(cfg.name, strlen(cfg.name)) != 0) {
            perror("Failed to set hostname");
            exit(1);
        }

        if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) != 0) {
            perror("Failed to make root mount private");
            exit(1);
        }

        if (mount("proc", "/proc", "proc", 0, NULL) != 0) {
            perror("Failed to mount /proc");
            exit(1);
        }

	setsid();

        printf("Running child with pid: %d (in container namespace)\n", getpid());
        
        // Execute the command
       	char *args[] = {"/bin/sh", "-c", cfg.command, NULL};
        execvp(args[0], args);
        
        // If execl returns, it failed
        perror("Execvp failed");
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
        printf("[Parent] Stoping...\n");
    }
    return 0;
}

int main(int argc, char **argv) {
    struct config cfg = {
        .subcommand = NONE,
        .name = "",
        .command = "",
    };

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "run") == 0) {
            cfg.subcommand = RUN;
            i++;
        } else if (strcmp(argv[i], "exec") == 0) {
            cfg.subcommand = EXEC;
            i++;
        } else if (strcmp(argv[i], "--name") == 0) {
            if (i+1 >= argc) {
                fprintf(stderr, "[ERR] Missing --name value (e.g. [--name bib]).\n");
                return 1;
            }
            strncpy(cfg.name, argv[++i], sizeof(cfg.name) - 1);
            i++;
        } else {
            strncpy(cfg.command, argv[i], sizeof(cfg.command) - 1);
            i++;
        }
    }

    if (validate_config(cfg) != 0) {
        return 1;
    }

    switch (cfg.subcommand) {
        case RUN:
            if (run_container(cfg) != 0) {
                fprintf(stderr, "[ERR] Running container failed due to some internal errors.\n");
                return 1;
            }
            break;
        case EXEC:
            printf("EXEC subcommand have not implemented yet...\n");
            break;
        case NONE:
        default:
            break;
    }
    return 0;
}
