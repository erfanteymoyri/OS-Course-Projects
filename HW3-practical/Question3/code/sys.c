SYSCALL_DEFINE1(set_my_field, long, new_val)
{
    u64 t2_ns, t3_ns;

    t2_ns = ktime_get_real_ns();
    current->my_custom_field = new_val;
    t3_ns = ktime_get_real_ns();

    current->last_syscall_t2_ns = t2_ns;
    current->last_syscall_t3_ns = t3_ns;
	current->last_syscall_name = "set_my_field";


    return 0;
}

SYSCALL_DEFINE0(get_my_field)
{
	u64 t2_ns, t3_ns;
    long val;

    t2_ns = ktime_get_real_ns();
    val = current->my_custom_field;
    t3_ns = ktime_get_real_ns();

    current->last_syscall_t2_ns = t2_ns;
    current->last_syscall_t3_ns = t3_ns;
	current->last_syscall_name = "get_my_field";

    return val;
}

SYSCALL_DEFINE0(get_time_ns)
{
    return ktime_get_real_ns();
}

SYSCALL_DEFINE0(get_last_syscall_times)
{
    u64 t2 = current->last_syscall_t2_ns;
    u64 t3 = current->last_syscall_t3_ns;
    const char *name = current->last_syscall_name;
    pid_t pid = task_tgid_vnr(current);

    if (!name)
        name = "N/A";

    printk(KERN_INFO "[syscall_report] PID=%d, Syscall=%s, T2=%llu, T3=%llu, Exec_Time=%llu ns\n",
           pid, name, t2, t3, (t3 - t2));

    return 0;
}
