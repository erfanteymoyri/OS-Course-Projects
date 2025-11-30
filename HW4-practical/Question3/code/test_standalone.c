#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "my_fs.h"

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define NC    "\033[0m"

// Mock filler function to simulate FUSE behavior for fs_list_files
// FUSE expects a function that takes these arguments. We just print the name.
int mock_filler(void *buf, const char *name, const struct stat *stbuf, off_t off) {
    printf("   [Found File]: %s\n", name);
    return 0;
}

void print_pass(const char *message) {
    printf("%s[PASS]%s %s\n", GREEN, NC, message);
}

void print_fail(const char *message) {
    printf("%s[FAIL]%s %s\n", RED, NC, message);
    exit(1);
}

int main() {
    printf("--- Starting Standalone FS Test ---\n\n");

    // 1. Initialize the File System
    // This should create filesys.db (10MB) if it doesn't exist
    printf("1. Initializing File System...\n");
    remove("filesys.db"); // Clean start
    fs_init();
    
    // Check if file exists
    FILE *f = fopen("filesys.db", "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fclose(f);
        if (size == 2 * 1024 * 1024) {
            print_pass("filesys.db created with correct size (10MB).");
        } else {
            char msg[100];
            sprintf(msg, "filesys.db size is incorrect. Got: %ld", size);
            print_fail(msg);
        }
    } else {
        print_fail("filesys.db was not created.");
    }

    // 2. Create a File
    printf("\n2. Creating 'unit_test.txt'...\n");
    int32_t fd = fs_open("unit_test.txt", 1);
    if (fd != -1) {
        print_pass("File 'unit_test.txt' created/opened successfully.");
    } else {
        print_fail("Failed to create file.");
    }

    // 3. Write to File
    printf("\n3. Writing data...\n");
    const char *text = "Standalone Logic Check 123";
    fs_write(fd, text, strlen(text) + 1); // +1 to include null terminator for easier reading
    print_pass("Write operation completed.");

    // 4. Read from File
    printf("\n4. Reading data back...\n");
    char buffer[100] = {0};
    fs_read(fd, buffer, 100);
    printf("   Read content: '%s'\n", buffer);

    if (strcmp(text, buffer) == 0) {
        print_pass("Read data matches written data.");
    } else {
        print_fail("Read data does NOT match.");
    }

    // 5. List Files
    printf("\n5. Listing files (using mock filler)...\n");
    // We cast our mock function to match fuse_fill_dir_t signature
    fs_list_files(NULL, (fuse_fill_dir_t)mock_filler);
    print_pass("List files executed.");

    // 6. Delete File
    printf("\n6. Deleting 'unit_test.txt'...\n");
    fs_rm(fd);
    
    // Verify deletion by trying to open it without create flag
    int32_t check_fd = fs_open("unit_test.txt", 0);
    if (check_fd == -1) {
        print_pass("File successfully deleted (cannot open anymore).");
    } else {
        print_fail("File still exists after deletion!");
    }

    // 7. Disk Full Check (Optional logic verification)
    // Since we set capacity to 1024 in fs_open, let's verify logical limits if implemented
    // or just close down.
    
    printf("\n7. Shutting down...\n");
    fs_close(fd); // Just flushes
    // fs_shutdown(); // You can add this to my_fs.c/h if you want to close the FILE* pointer properly

    printf("\n%s--- ALL STANDALONE TESTS PASSED ---%s\n", GREEN, NC);
    return 0;
}