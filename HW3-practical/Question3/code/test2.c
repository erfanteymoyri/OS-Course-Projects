SYSCALL_DEFINE1(set_my_field, long, new_val)
{
    current->my_custom_field = new_val;

    printk(KERN_INFO "PID %d set my_custom_field to %ld\n", current->pid, new_val);
    return 0;
}

SYSCALL_DEFINE0(get_my_field)
{
    long val = current->my_custom_field;

    printk(KERN_INFO "PID %d read my_custom_field: %ld\n", current->pid, val);
    return val;
}