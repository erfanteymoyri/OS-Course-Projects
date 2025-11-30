#define FUSE_USE_VERSION 26 
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include "my_fs.h"

#define DISK_NAME "filesys.db"
#define MAGIC_NUMBER 0xDEADBEEF

// CHANGE: Define fixed disk size (2 MB)
#define DISK_SIZE (2 * 1024 * 1024)

typedef struct {
    int32_t magic;
    int32_t version;
    int32_t last_allocated_pos;
    int32_t root_file_offset;
} Superblock;

typedef struct {
    char name[255];
    int32_t size;
    int32_t capacity;
    int32_t type;
    int32_t is_active;
    int32_t next_file_pos;
    int32_t data_pos;
} FileEntry;

FILE *disk_file = NULL;

// --- Helper Functions ---

void save_superblock(Superblock *sb) {
    fseek(disk_file, 0, SEEK_SET);
    fwrite(sb, sizeof(Superblock), 1, disk_file);
    fflush(disk_file);
}

void load_superblock(Superblock *sb) {
    fseek(disk_file, 0, SEEK_SET);
    fread(sb, sizeof(Superblock), 1, disk_file);
}

// --- Main FS Functions ---

void fs_init() {
    disk_file = fopen(DISK_NAME, "rb+");

    if (!disk_file) {
        printf("Disk not found. Creating new disk (Fixed 2MB)...\n");
        disk_file = fopen(DISK_NAME, "wb+");
        
        // CHANGE: Create a file with exact 2MB size
        // Seek to the last byte and write zero so OS allocates full space
        fseek(disk_file, DISK_SIZE - 1, SEEK_SET);
        fputc(0, disk_file);
        fseek(disk_file, 0, SEEK_SET);

        Superblock sb;
        sb.magic = MAGIC_NUMBER;
        sb.version = 1;
        sb.last_allocated_pos = sizeof(Superblock);
        sb.root_file_offset = -1;
        
        save_superblock(&sb);
    } else {
        Superblock sb;
        load_superblock(&sb);
        if (sb.magic != MAGIC_NUMBER) {
            printf("Error: Invalid magic number!\n");
            exit(1);
        }
    }
}

void fs_shutdown() {
    if (disk_file) {
        fclose(disk_file);
        disk_file = NULL;
    }
}

int32_t fs_find_file_offset(const char *filename) {
    Superblock sb;
    load_superblock(&sb);
    int32_t current_pos = sb.root_file_offset;

    while (current_pos != -1) {
        FileEntry entry;
        fseek(disk_file, current_pos, SEEK_SET);
        fread(&entry, sizeof(FileEntry), 1, disk_file);

        if (entry.is_active && strcmp(entry.name, filename) == 0) {
            return current_pos;
        }
        current_pos = entry.next_file_pos;
    }
    return -1;
}

void fs_list_files(void *buffer, fuse_fill_dir_t filler) {
    Superblock sb;
    load_superblock(&sb);
    int32_t current_pos = sb.root_file_offset;

    while (current_pos != -1) {
        FileEntry entry;
        fseek(disk_file, current_pos, SEEK_SET);
        fread(&entry, sizeof(FileEntry), 1, disk_file);

        if (entry.is_active) {
            filler(buffer, entry.name, NULL, 0);
        }
        current_pos = entry.next_file_pos;
    }
}

int32_t fs_open(const char *filename, int create_flag) {
    int32_t existing_offset = fs_find_file_offset(filename);
    if (existing_offset != -1) {
        return existing_offset;
    }

    if (!create_flag) {
        return -1;
    }

    Superblock sb;
    load_superblock(&sb);

    FileEntry new_entry;
    strncpy(new_entry.name, filename, 255);
    new_entry.size = 0;
    new_entry.is_active = 1;
    new_entry.next_file_pos = -1;
    new_entry.capacity = 1024;

    // Calculate required space
    int32_t needed_space = sizeof(FileEntry) + new_entry.capacity;
    
    // Check disk space before allocation
    if (sb.last_allocated_pos + needed_space > DISK_SIZE) {
        printf("Error: Disk Full! Cannot create file '%s'.\n", filename);
        return -1;
    }

    int32_t entry_write_pos = sb.last_allocated_pos;
    new_entry.data_pos = entry_write_pos + sizeof(FileEntry);

    if (sb.root_file_offset == -1) {
        sb.root_file_offset = entry_write_pos;
    } else {
        int32_t current = sb.root_file_offset;
        while (true) {
            FileEntry temp;
            fseek(disk_file, current, SEEK_SET);
            fread(&temp, sizeof(FileEntry), 1, disk_file);
            
            if (temp.next_file_pos == -1) {
                temp.next_file_pos = entry_write_pos;
                fseek(disk_file, current, SEEK_SET);
                fwrite(&temp, sizeof(FileEntry), 1, disk_file);
                break;
            }
            current = temp.next_file_pos;
        }
    }

    fseek(disk_file, entry_write_pos, SEEK_SET);
    fwrite(&new_entry, sizeof(FileEntry), 1, disk_file);
    
    sb.last_allocated_pos += needed_space;
    save_superblock(&sb);

    return entry_write_pos;
}

void fs_write(int32_t file_pos, const char *data, int size) {
    FileEntry entry;
    fseek(disk_file, file_pos, SEEK_SET);
    fread(&entry, sizeof(FileEntry), 1, disk_file);

    if (size > entry.capacity) {
        size = entry.capacity;
    }

    fseek(disk_file, entry.data_pos, SEEK_SET);
    fwrite(data, 1, size, disk_file);

    entry.size = size;
    fseek(disk_file, file_pos, SEEK_SET);
    fwrite(&entry, sizeof(FileEntry), 1, disk_file);
    fflush(disk_file);
}

void fs_read(int32_t file_pos, char *buffer, int size) {
    FileEntry entry;
    fseek(disk_file, file_pos, SEEK_SET);
    fread(&entry, sizeof(FileEntry), 1, disk_file);

    if (size > entry.size) size = entry.size;

    fseek(disk_file, entry.data_pos, SEEK_SET);
    fread(buffer, 1, size, disk_file);
}

void fs_close(int32_t file_pos) {
    // No open-file state, just flush
    fflush(disk_file);
}

void fs_rm(int32_t file_pos) {
    FileEntry entry;
    fseek(disk_file, file_pos, SEEK_SET);
    fread(&entry, sizeof(FileEntry), 1, disk_file);
    
    entry.is_active = 0; 
    
    fseek(disk_file, file_pos, SEEK_SET);
    fwrite(&entry, sizeof(FileEntry), 1, disk_file);
}

void fs_shrink(int32_t file_pos, int new_size) {
    FileEntry entry;
    fseek(disk_file, file_pos, SEEK_SET);
    fread(&entry, sizeof(FileEntry), 1, disk_file);

    if (new_size < entry.size) {
        entry.size = new_size;
        fseek(disk_file, file_pos, SEEK_SET);
        fwrite(&entry, sizeof(FileEntry), 1, disk_file);
    }
}
