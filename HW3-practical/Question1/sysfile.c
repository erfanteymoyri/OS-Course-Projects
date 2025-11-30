uint64
sys_top(void)
{
  top();
  return 0;
}

uint64
sys_next_process(void)
{
  int before_pid;
  uint64 proc_addr;
  int return_pid;

  argaddr(1, &proc_addr);
  argint(0, &before_pid);

  struct process_data out;

  return_pid = next_process(before_pid, &out);

  if (return_pid > 0) {
    if (copyout(myproc()->pagetable, proc_addr, (char *)&out, sizeof(out)) < 0) {
      return 1;
    }
  }

  return return_pid;
}