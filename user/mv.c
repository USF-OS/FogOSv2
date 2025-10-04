// user/mv.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MV_MAXPATH 256

// Get basename of path, strip dir and trailing slashes
static const char *
base_of(const char *path)
{
  // Start from the end of the string
  const char *end = path + strlen(path);
  // Drop any trailing slashes
  while (end > path && end[-1] == '/') {
    end--;
  }
  // Move backwards until the last slash or the beginning
  while (end > path && end[-1] != '/') {
    end--;
  }
  // Now points to the basename
  return end;
}

// Check if a path is a directory
static int
is_dir(const char *path)
{
  struct stat st;

  if (stat(path, &st) < 0) {
    return 0;
  }
  
  return st.type == T_DIR;
}

static void 
usage(void) 
{
  printf("usage: mv [-f] [-i] [-v] SRC DST\n");
}

// Join directory path + filename into one full path string
static int 
path_join(char *out, int out_size, const char *dir, const char *name) 
{
  int dir_len = strlen(dir);
  int name_len = strlen(name);
  int need_slash = (dir_len == 0 || dir[dir_len-1] != '/');
  int total_len = dir_len; 
  if (need_slash) {
    total_len += 1;
  }
  total_len += name_len;
  total_len += 1;
  if (total_len > out_size) {
    return -1;
  }
  memmove(out, dir, dir_len);
  int pos = dir_len;
  if (need_slash) {
    out[pos++] = '/';
  }
  memmove(out + pos, name, name_len);
  pos += name_len;
  out[pos] = '\0';
  return 0;
}

int
main(int argc, char *argv[])
{
  int argi = 1; 
  int force = 0;
  int interactive = 0;
  int verbose = 0;
  int was_interactive_ok = 0;
  
  if (argc < 3) {
    usage();
    exit(1);
  }
  // Parse flags
  while (argi < argc && argv[argi][0] == '-') {
    if (strcmp(argv[argi], "-f") == 0) {
        force = 1;
    } else if (strcmp(argv[argi], "-i") == 0) {
        interactive = 1;
    } else if (strcmp(argv[argi], "-v") == 0) {
        verbose = 1;
    } else {
        usage();
        exit(1);
    }
    argi++;
  }
  if (argc - argi < 2) { 
    usage(); 
    exit(1); 
  }

  char *src = argv[argi++];
  char *dst = argv[argi++];

  if (is_dir(src)) {
    printf("mv: %s: is a directory (unsupported)\n", src);
    exit(1);
  }

  char finaldst[MV_MAXPATH];
  if (is_dir(dst)) {
    const char *base = base_of(src);
    if (path_join(finaldst, sizeof(finaldst), dst, base) < 0) {
      printf("mv: destination path too long\n");
      exit(1);
    }
  } else {
    if ((int)strlen(dst) + 1 > (int)sizeof(finaldst)) {
      printf("mv: destination path too long\n");
      exit(1);
    }
    strcpy(finaldst, dst);
  }

  struct stat sst, dstst;
  if (stat(src, &sst) == 0 && stat(finaldst, &dstst) == 0) {
    if (sst.dev == dstst.dev && sst.ino == dstst.ino) {
        printf("mv: '%s' and '%s' are the same file\n", src, finaldst);
        exit(1);
    }
  }
  
  if (stat(finaldst, &dstst) == 0 && dstst.type != T_DIR) {
    if (interactive && !force) {
      printf("mv: overwrite '%s'? (y/Y to confirm)\n", finaldst);
      char ch;
      if (read(0, &ch, 1) != 1 || (ch != 'y' && ch != 'Y')) {
        printf("not overwritten\n");
        exit(0);
      }
      char dummy;
      while (read(0, &dummy, 1) == 1 && dummy != '\n');
      was_interactive_ok = 1;
    } else if (!force) {
      printf("mv: %s exists (use -f to overwrite)\n", finaldst);
      exit(1);
    }
    
    if (unlink(finaldst) < 0) {
      printf("mv: cannot remove %s\n", finaldst);
      exit(1);
    }
  }

  if (link(src, finaldst) < 0) {
    printf("mv: cannot link %s -> %s\n", src, finaldst);
    exit(1);
  }
  
  if (unlink(src) < 0) {
    printf("mv: warning: linked but failed to remove %s\n", src);
    exit(1);
  }

  if (was_interactive_ok) {
    printf("mv: '%s' overwritten with '%s'\n", finaldst, src);
  }
  
  if (verbose) {
    printf("'%s' -> '%s'\n", src, finaldst);
  }
  
  exit(0);
}
