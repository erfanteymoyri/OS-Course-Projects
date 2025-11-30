#ifndef MY_FS_H
#define MY_FS_H

#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>

// Basic file system functions
void fs_init();
int32_t fs_open(const char *filename, int create_flag);
void fs_write(int32_t file_pos, const char *data, int size);
void fs_read(int32_t file_pos, char *buffer, int size);
void fs_close(int32_t file_pos);
void fs_rm(int32_t file_pos);
void fs_shrink(int32_t file_pos, int new_size);

// Helper functions
int32_t fs_find_file_offset(const char *filename);
void fs_list_files(void *buffer, fuse_fill_dir_t filler);

#endif