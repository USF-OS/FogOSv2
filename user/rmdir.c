// user/rmdir.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

static void
usage(void)
{
  printf("usage: rmdir [-v] DIR [DIR ...]\n");
}

// Check if path is a directory
static int
is_dir(const char *path)
{
  struct stat st;
  if (stat(path, &st) < 0) {
    return 0;
  }
  return st.type == T_DIR;
}

// Check if entry is "." or ".."
static int
is_dot_or_dotdot(const char *s)
{
  return (strcmp(s, ".") == 0) || (strcmp(s, "..") == 0);
}

// Remove one directory
static int
rmdir_one(const char *path, int verbose)
{
  if (!is_dir(path)) {
    printf("rmdir: %s: not a directory\n", path);
    return -1;
  }

  int fd = open(path, O_RDONLY);
  if (fd < 0) {
    printf("rmdir: cannot open %s\n", path);
    return -1;
  }

  struct dirent entry;
  int empty = 1;
  while (read(fd, &entry, sizeof(entry)) == sizeof(entry)) {
    if (entry.inum == 0) {
      continue;
    }

    char name[DIRSIZ+1];
    memmove(name, entry.name, DIRSIZ);
    name[DIRSIZ] = 0;

    if (is_dot_or_dotdot(name)) {
      continue;
    }
    empty = 0; 
    break;
  }
  close(fd);

  if (!empty) {
    printf("rmdir: %s: directory not empty\n", path);
    return -1;
  }

  if (unlink(path) < 0) {
    printf("rmdir: failed to remove %s\n", path);
    return -1;
  }

  if (verbose) {
    printf("rmdir: removed directory '%s'\n", path);
  }
  
  return 0;
}

int
main(int argc, char *argv[])
{
  if (argc < 2) { 
    usage(); 
    exit(1); 
  }
  int verbose = 0;
  int argi = 1;
  if (strcmp(argv[argi], "-v") == 0) {
    verbose = 1;
    argi++;
    if (argi >= argc) {
        usage();
        exit(1);
    }
  }
  
  int rc = 0;
  for (int i = argi; i < argc; i++) {
    if (rmdir_one(argv[i], verbose) < 0)
      rc = 1;
  }

  exit(rc);
}
