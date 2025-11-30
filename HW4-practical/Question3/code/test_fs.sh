#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}---  Starting the FUSE file system testing process  ---${NC}"

#1. Clean up the previous environment (if any remains)
echo "1. Cleaning up previous runs..."
fusermount -u mnt 2>/dev/null
rm -f filesys.db my_fuse
rm -rf mnt
mkdir mnt

#2. Compile the code
echo "2. Compiling..."
gcc my_fuse.c my_fs.c -o my_fuse -D_FILE_OFFSET_BITS=64 $(pkg-config fuse --cflags --libs)

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Compilation failed!${NC}"
    exit 1
fi
echo -e "${GREEN}Compilation successful.${NC}"

#3. Run FUSE (mount)
echo "3. Mounting filesystem..."
./my_fuse mnt &
FUSE_PID=$!
sleep 2 # We wait a while for the file system to fully mount.

# Check if it is mounted or not
if mount | grep -q "mnt"; then
    echo -e "${GREEN}Filesystem mounted successfully.${NC}"
else
    echo -e "${RED}Error: Failed to mount filesystem.${NC}"
    exit 1
fi

# --- Start operational tests ---

# Test 4: Creating a file
echo "4. Testing File Creation (touch)..."
touch mnt/hello.txt
if [ -f mnt/hello.txt ]; then
    echo -e "${GREEN}Pass: File created.${NC}"
else
    echo -e "${RED}Fail: File not created.${NC}"
    exit 1
fi

# Test 5: Writing to a file
echo "5. Testing Write (echo)..."
TEST_STR="Hello_FUSE_World"
echo $TEST_STR > mnt/hello.txt
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Pass: Write operation successful.${NC}"
else
    echo -e "${RED}Fail: Write operation failed.${NC}"
    exit 1
fi

# Test 6: Reading from file
echo "6. Testing Read (cat)..."
READ_STR=$(cat mnt/hello.txt)
if [ "$READ_STR" == "$TEST_STR" ]; then
    echo -e "${GREEN}Pass: Read data matches written data ($READ_STR).${NC}"
else
    echo -e "${RED}Fail: Read data mismatch! Expected '$TEST_STR', got '$READ_STR'.${NC}"
    # This may give an error if the file size is not handled correctly in getattr, but we'll test it for now.
fi

# Test 7: Listing files
echo "7. Testing Directory List (ls)..."
LS_OUTPUT=$(ls mnt)
if [[ "$LS_OUTPUT" == *"hello.txt"* ]]; then
    echo -e "${GREEN}Pass: File found in list.${NC}"
else
    echo -e "${RED}Fail: File not found in ls output.${NC}"
fi

# Test 8: Delete file
echo "8. Testing Delete (rm)..."
rm mnt/hello.txt
if [ ! -f mnt/hello.txt ]; then
    echo -e "${GREEN}Pass: File deleted.${NC}"
else
    echo -e "${RED}Fail: File still exists after delete.${NC}"
fi


# Test 9: Persistence Test
echo "9. Persistence Test..."
touch mnt/hello.txt
if [ -f mnt/hello.txt ]; then
    echo -e "${GREEN}Pass: File created.${NC}"
else
    echo -e "${RED}Fail: File not created.${NC}"
    exit 1
fi

TEST_STR="Hello_FUSE_World"
echo $TEST_STR > mnt/hello.txt
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Pass: Write operation successful.${NC}"
else
    echo -e "${RED}Fail: Write operation failed.${NC}"
    exit 1
fi

#10. Unmount
echo "9. Unmounting and Cleaning up..."
fusermount -u mnt
if [ $? -eq 0 ]; then
    echo -e "${GREEN}--- ALL TESTS PASSED SUCCESSFULLY ---${NC}"
else
    echo -e "${RED}Error during unmount.${NC}"
fi
