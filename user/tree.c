#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

// Helper to concatenate strings
char* strcat_helper(char *dest, const char *src) {
    char *rdest = dest;
    while (*dest) dest++;
    while ((*dest++ = *src++));
    return rdest;
}

// The main recursive tree function
void tree(char *path, char *prefix, int *dir_count, int *file_count) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "tree: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "tree: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (st.type != T_DIR) {
        fprintf(2, "tree: %s is not a directory\n", path);
        close(fd);
        return;
    }
    
    // Check if the initial path is already too long
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
      fprintf(2, "tree: path too long\n");
      close(fd);
      return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        // Safely copy the entry name into the buffer and null-terminate it.
        // This is the pointer `p` that you correctly identified as critical.
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0; 

        if (stat(buf, &st) < 0) {
            fprintf(2, "tree: cannot stat %s\n", buf);
            continue;
        }
        
        printf("%s|-- %s\n", prefix, p);

        if (st.type == T_DIR) {
            (*dir_count)++;
            int prefix_len = strlen(prefix);
            // Check for prefix buffer overflow before recursing
            if (prefix_len + 5 < 512) {
                // Append to the prefix for the recursive call
                strcat_helper(prefix, "|   ");
                tree(buf, prefix, dir_count, file_count);
                // Backtrack here: After the recursive call returns, restore the prefix
                // for the next entry in the current directory.
                prefix[prefix_len] = '\0';
            } else {
                // Recurse without extending prefix if the buffer is full
                tree(buf, prefix, dir_count, file_count);
            }
        } else {
            (*file_count)++;
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    int dir_count = 0;
    int file_count = 0;
    char *start_path = ".";

    if (argc > 1) {
        start_path = argv[1];
    }
    
    printf("%s\n", start_path);
    
    // Count the starting directory itself
    if(strcmp(start_path, ".") != 0 && strcmp(start_path, "/") != 0) {
        struct stat st;
        if(stat(start_path, &st) == 0 && st.type == T_DIR) {
            dir_count++;
        }
    }

    // Create a single, modifiable buffer for the prefix string on the stack here.
    // This buffer is reused by all recursive calls.
    char prefix_buf[512];
    prefix_buf[0] = '\0';

    tree(start_path, prefix_buf, &dir_count, &file_count);

    printf("\n%d directories, %d files\n", dir_count, file_count);
    exit(0);
}
