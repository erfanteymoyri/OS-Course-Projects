void
top(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [USED]      "used",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  struct proc *p;
  char *state;

  printf("\n--- Process List ---\n");
  printf("PID\tSIZE\tSTATE\tNAME\n");

  for(p = proc; p < &proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    printf("%d\t%s\t%s\t%lu", p->pid, state, p->name, p->sz);
    printf("\n");
  }
}

int
next_process(int before_pid, struct process_data *out)
{
  struct proc *p;
  struct proc *best = 0;
  int best_pid = 1000000000;

  acquire(&pid_lock);

  for(p = proc; p < &proc[NPROC]; p++){
    acquire(&p->lock);

    if(p->state != UNUSED && p->pid > before_pid) {
      if(p->pid < best_pid) {
        best = p;
        best_pid = p->pid;
      }
    }

    release(&p->lock);
  }

  if(best) {
    acquire(&best->lock);

    out->pid = best->pid;
    out->parent_id = best->parent ? best->parent->pid : 0;
    out->head_size = best->sz;
    out->state = best->state;
    safestrcpy(out->name, best->name, sizeof(out->name));

    release(&best->lock);
  }

  release(&pid_lock);

  return best_pid == 1000000000 ? 0 : best_pid;
}