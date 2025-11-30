SYSCALL_DEFINE0(get_time_ns)
{
    return ktime_get_real_ns();
}