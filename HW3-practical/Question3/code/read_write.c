SYSCALL_DEFINE3(read, unsigned int, fd, char __user *, buf, size_t, count)
{
	u64 t2_ns, t3_ns;
    ssize_t ret;

    t2_ns = ktime_get_real_ns();
    ret = ksys_read(fd, buf, count);
    t3_ns = ktime_get_real_ns();

    current->last_syscall_t2_ns = t2_ns;
    current->last_syscall_t3_ns = t3_ns;
	current->last_syscall_name = "read";

    return ret;
}

SYSCALL_DEFINE3(write, unsigned int, fd, const char __user *, buf,
		size_t, count)
{
    u64 t2_ns, t3_ns;
    ssize_t ret;

    t2_ns = ktime_get_real_ns();
    ret = ksys_write(fd, buf, count);
    t3_ns = ktime_get_real_ns();

    current->last_syscall_t2_ns = t2_ns;
    current->last_syscall_t3_ns = t3_ns;
	current->last_syscall_name = "write";

    return ret;
}
