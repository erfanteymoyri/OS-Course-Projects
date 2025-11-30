#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}--- Starting the Persistence Test ---${NC}"

# 1. Checking for the existence of the previous database
if [ ! -f filesys.db ]; then
    echo -e "${RED}Error: فایل filesys.db پیدا نشد!${NC}"
    echo "لطفاً اول اسکریپت قبلی (test_fs.sh) را اجرا کنید تا دیتابیس ساخته شود."
    exit 1
fi

# 2. Make sure it is not mounted now (we will only unmount it, not delete it)
echo "1. Ensuring filesystem is unmounted..."
fusermount -u mnt 2>/dev/null

# 3. Recompile (to ensure the latest code changes are applied)
echo "2. Re-compiling..."
gcc my_fuse.c my_fs.c -o my_fuse -D_FILE_OFFSET_BITS=64 $(pkg-config fuse --cflags --libs)

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Compilation failed!${NC}"
    exit 1
fi

# 4. Remount (without deleting the database)
echo "3. Mounting existing filesystem..."
./my_fuse mnt &
sleep 2

if mount | grep -q "mnt"; then
    echo -e "${GREEN}Filesystem mounted successfully.${NC}"
else
    echo -e "${RED}Error: Failed to mount.${NC}"
    exit 1
fi

# --- Start checking old information ---

echo "4. Checking for 'hello.txt' from previous run..."

# Does the file exist?
if [ -f mnt/hello.txt ]; then
    echo -e "${GREEN}Pass: Old file 'hello.txt' found!${NC}"
else
    echo -e "${RED}Fail: Old file disappeared! Persistence is broken.${NC}"
    fusermount -u mnt
    exit 1
fi

# Is the content correct?
EXPECTED_STR="Hello_FUSE_World"
READ_STR=$(cat mnt/hello.txt)

if [ "$READ_STR" == "$EXPECTED_STR" ]; then
    echo -e "${GREEN}Pass: Content matches ($READ_STR).${NC}"
else
    echo -e "${RED}Fail: Content mismatch! Got '$READ_STR', expected '$EXPECTED_STR'.${NC}"
fi

# --- Add new information ---

echo "5. Adding a NEW file (append test)..."
echo "This is a new file" > mnt/new_persistent.txt

if [ -f mnt/new_persistent.txt ]; then
    echo -e "${GREEN}Pass: New file created successfully alongside old data.${NC}"
else
    echo -e "${RED}Fail: Could not create new file.${NC}"
fi

# List all files
echo -e "${BLUE}Current files in filesystem:${NC}"
ls -l mnt/

#6. End of work
echo "6. Unmounting..."
fusermount -u mnt

if [ $? -eq 0 ]; then
    echo -e "${GREEN}--- PERSISTENCE TEST PASSED ---${NC}"
    echo "Your filesystem successfully remembered data across restarts!"
else
    echo -e "${RED}Error during unmount.${NC}"
fi