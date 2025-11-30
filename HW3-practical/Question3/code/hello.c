SYSCALL_DEFINE0(my_hello)
{
    struct file *file;
    char msg[] = "Hello from kernel to stdout!\n";
    size_t msg_len = strlen(msg);
    long ret;
    loff_t pos = 0;

    file = fget(1);

    if (!file) {
        printk(KERN_WARNING "my_hello: Could not get file for stdout (fd 1)\n");
        return -EBADF;
    }

    if (!file->f_op->write && !file->f_op->write_iter) {
        printk(KERN_WARNING "my_hello: stdout does not have a write/write_iter operation\n");
        fput(file);
        return -EACCES;
    }

    ret = kernel_write(file, msg, msg_len, &pos); 

    fput(file);

    if (ret < 0) {
        printk(KERN_WARNING "my_hello: kernel_write to stdout failed with error %ld\n", ret);
        return ret;
    }

    return ret;
}
