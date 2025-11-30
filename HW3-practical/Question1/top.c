#include "kernel/types.h"
#include "kernel/stat.h"
#include <time.h>
#include "user/user.h"

#define INITIAL_CAP 32
#define MAX_CAP 100   

char*
get_state_string(enum procstate state)
{
    switch (state) {
        case SLEEPING:  return "SLEEPING";
        case RUNNABLE:  return "RUNNABLE";
        case RUNNING:   return "RUNNING";
        case ZOMBIE:    return "ZOMBIE";
        case USED:      return "USED";
        case UNUSED:    return "UNUSED";
        default:        return "UNKNOWN";
    }
}

int fibo(int n)
{
  if (n == 1 || n == 0) {
    return 1;
  }
  return fibo(n-1) + fibo(n-2);
}



void
print_table_simple(struct process_data *procs, int count)
{
    for (int i = 0; i < count; i++) {
        printf("%d\t%d\t%s\t%d\t%s\n",
               procs[i].pid,
               procs[i].parent_id,
               get_state_string(procs[i].state),
               procs[i].head_size,
               procs[i].name);
    }
}

void
print_tree_recursive(struct process_data *procs, int count, int parent_id, int depth)
{
    for (int i = 0; i < count; i++) {
        if (procs[i].parent_id == parent_id) {
            for (int j = 0; j < depth; j++) printf("    ");
            printf("%d | %d | %s | %d | %s\n",
                   procs[i].pid,
                   procs[i].parent_id,
                   get_state_string(procs[i].state),
                   procs[i].head_size,
                   procs[i].name);
            
            print_tree_recursive(procs, count, procs[i].pid, depth + 1);
            // printf("depth: %d", depth);
        }
    }
}

int
main(int argc, char *argv[])
{
    int tree_mode = (argc > 1 && (strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--tree") == 0));

    int cap = MAX_CAP;
    struct process_data *all_procs = malloc((uint)(sizeof(struct process_data) * cap));
    int count = 0;

    int last_pid = 0;
    struct process_data current_proc_data;
    int ret;

    int pid_root = getpid();

    fork();
    fork();
    fork();

    if (getpid() != pid_root) {
      fibo(35);
      exit(0);
    }

    if (getpid() == pid_root) {
      while (1) {
        ret = next_process(last_pid, &current_proc_data);
        if (ret == 0) {
            break;
        } else if (ret < 0) {
            break;
        }

        if (count >= cap) {
            break;
        }
        all_procs[count++] = current_proc_data;

        last_pid = ret;
      }

      printf("----------------------------------------------------------\n");
      printf("PID\tParent\tState\t\tSize\tName\n");
      printf("----------------------------------------------------------\n");
      if (tree_mode) {
        print_tree_recursive(all_procs, count, 0, 0);
      } else {
        print_table_simple(all_procs, count);
      }
      printf("----------------------------------------------------------\n");

      free(all_procs);
      exit(0);
    }

}