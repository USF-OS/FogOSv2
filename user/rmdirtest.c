// user/rmdirtest.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

static int
contains(const char *text, const char *substring)
{
  int text_len = strlen(text);
  int sub_len  = strlen(substring);

  if (sub_len == 0) {
    return 1;
  }

  if (text_len < sub_len) {
    return 0;
  }

  for (int i = 0; i <= text_len - sub_len; i++) {
    int j = 0;
    while (j < sub_len && text[i + j] == substring[j]) {
      j++;
    }
    if (j == sub_len) {
      return 1;
    }
  }

  return 0;
}

static int
run_rmdir(char *argv[])
{
  int pid = fork();
  if (pid == 0) {
    exec("rmdir", argv);
    printf("exec rmdir failed\n");
    exit(1);
  }
  int status;
  wait(&status);
  return status;
}

// Create a directory
static void 
make_dir(const char *name) 
{
  mkdir(name);
}

// Create a file inside a directory
static void 
make_file(const char *path, const char *content) 
{
  int fd = open(path, O_CREATE|O_WRONLY);
  if (fd < 0) {
    printf("create %s failed\n", path);
    exit(1);
  }
  if (content) {
    write(fd, content, strlen(content));
  }
  close(fd);
}

// Check if path exists
static int 
exists(const char *path) 
{
  struct stat st;
  return stat(path, &st) == 0;
}

static void 
test_remove_empty() 
{
  printf("Test: remove one empty directory... ");
  unlink("dir1");
  make_dir("dir1");

  char *argv[] = {"rmdir", "dir1", 0};
  run_rmdir(argv);

  if (!exists("dir1")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }
}

static void 
test_remove_multiple() 
{
  printf("Test: remove multiple directories... ");
  unlink("d1"); 
  unlink("d2");
  make_dir("d1");
  make_dir("d2");

  char *argv[] = {"rmdir", "d1", "d2", 0};
  run_rmdir(argv);

  if (!exists("d1") && !exists("d2")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }
}

static void 
test_non_empty() 
{
  printf("Test: refuse non-empty directory... ");
  unlink("full/file.txt");
  unlink("full");
  make_dir("full");
  make_file("full/file.txt", "data");

  char *argv[] = {"rmdir", "full", 0};
  int rc = run_rmdir(argv);

  if (rc != 0 && exists("full")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }

  unlink("full/file.txt");
  unlink("full");
}

static void 
test_not_dir() 
{
  printf("Test: refuse non-directory... ");
  unlink("afile.txt");
  make_file("afile.txt", "hello");

  char *argv[] = {"rmdir", "afile.txt", 0};
  int rc = run_rmdir(argv);

  if (rc != 0 && exists("afile.txt")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }

  unlink("afile.txt");
}

static void
test_verbose()
{
  printf("Test: -v prints confirmation... ");
  unlink("vdir");
  mkdir("vdir");
  int out = open("out.txt", O_CREATE | O_WRONLY | O_TRUNC);
  if (out < 0) {
    printf("FAIL (cannot open out.txt)\n");
    return;
  }
  int saved = dup(1);
  close(1);
  dup(out); 
  char *argv[] = {"rmdir", "-v", "vdir", 0};
  int rc = run_rmdir(argv);
  close(out);
  close(1);
  dup(saved);
  close(saved);
  int fd = open("out.txt", O_RDONLY);
  
  if (fd < 0) {
    printf("FAIL (cannot read out.txt)\n");
    return;
  }
  
  char buf[256];
  int n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n < 0) {
    n = 0;
  }
  buf[n] = 0;
  if (rc == 0 && contains(buf, "removed directory") && contains(buf, "vdir")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }
  unlink("out.txt");
  unlink("vdir");
}

int
main(int argc, char *argv[])
{
  test_remove_empty();
  test_remove_multiple();
  test_non_empty();
  test_not_dir();
  test_verbose();
  printf("Manual: try `rmdir` with multiple args or flag -v yourself in the shell.\n");
  exit(0);
}
