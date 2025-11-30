#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include "my_fs.h"

// 1. Get Attributes (ls -l)
static int do_getattr(const char *path, struct stat *st)
{
    memset(st, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
        return 0;
    }

    int32_t offset = fs_find_file_offset(path + 1);
    if (offset != -1) {
        st->st_mode = S_IFREG | 0666;
        st->st_nlink = 1;
        st->st_size = 1024; 
        return 0;
    }

    return -ENOENT;
}

// 2. Read Directory (ls)
static int do_readdir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    if (strcmp(path, "/") != 0) return -ENOENT;

    filler(buffer, ".", NULL, 0);
    filler(buffer, "..", NULL, 0);
    fs_list_files(buffer, filler);

    return 0;
}

// 3. Read File (cat)
static int do_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int32_t file_pos = fs_find_file_offset(path + 1);
    if (file_pos == -1) return -ENOENT;

    fs_read(file_pos, buffer, size);
    return size;
}

// 4. Create File (touch - create part)
static int do_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    int32_t res = fs_open(path + 1, 1);
    if (res == -1) return -EACCES;
    return 0;
}

// 5. Write File
static int do_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int32_t file_pos = fs_find_file_offset(path + 1);
    if (file_pos == -1) return -ENOENT;

    fs_write(file_pos, buffer, size);
    return size;
}

// 6. Delete File (rm)
static int do_unlink(const char *path)
{
    int32_t file_pos = fs_find_file_offset(path + 1);
    if (file_pos == -1) return -ENOENT;
    
    fs_rm(file_pos);
    return 0;
}


// 7. Open File
static int do_open(const char *path, struct fuse_file_info *fi)
{
    int32_t file_pos = fs_find_file_offset(path + 1);
    if (file_pos == -1) return -ENOENT;
    return 0;
}

// 8. Truncate (necessary for echo >)
static int do_truncate(const char *path, off_t size)
{
    int32_t file_pos = fs_find_file_offset(path + 1);
    if (file_pos == -1) return -ENOENT;

    // We already had the fs_shrink function in my_fs.c, we'll use that
    fs_shrink(file_pos, (int)size);
    return 0;
}

// 9. Utimens (to fix touch error)
// This function updates the access time. We are not storing the time on disk at the moment
// So we just return 0 which means "done".
static int do_utimens(const char *path, const struct timespec ts[2])
{
    return 0;
}

// ---------------------------

static struct fuse_operations operations = {
    .getattr  = do_getattr,
    .readdir  = do_readdir,
    .read     = do_read,
    .create   = do_create,
    .write    = do_write,
    .unlink   = do_unlink,
    .open     = do_open,
    .truncate = do_truncate,
    .utimens  = do_utimens,
};

int main(int argc, char *argv[])
{
    fs_init();
    return fuse_main(argc, argv, &operations, NULL);
}