// user/mvtest.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"
#include "user/user.h"

static int 
run_mv(char *argv[]) 
{
  int pid = fork();
  if (pid == 0) {
    exec("mv", argv);
    printf("exec mv failed\n");
    exit(1);
  }
  int status;
  wait(&status);
  return status;
}

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

static void 
create_file(const char *name, const char *content) 
{
  unlink(name);
  int fd = open(name, O_CREATE|O_WRONLY);
  if (fd < 0) {
    printf("create %s failed\n", name);
    exit(1);
  }
  write(fd, content, strlen(content));
  close(fd);
}

static int 
file_exists(const char *name) 
{
  struct stat st;
  return stat(name, &st) == 0;
}

static void 
test_simple_move() 
{
  printf("Test: simple move... ");
  create_file("a.txt", "hello");

  char *argv[] = {"mv", "a.txt", "b.txt", 0};
  run_mv(argv);

  if (!file_exists("a.txt") && file_exists("b.txt")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }
  unlink("b.txt");
}

static void 
test_force_overwrite() 
{
  printf("Test: -f overwrite... ");
  create_file("c.txt", "old");
  create_file("d.txt", "new");

  char *argv[] = {"mv", "-f", "c.txt", "d.txt", 0};
  run_mv(argv);

  int fd = open("d.txt", O_RDONLY);
  if (fd < 0) {
    printf("FAIL (cannot open d.txt)\n");
    unlink("d.txt");
    return;
  }
  char buf[32];
  int n = read(fd, buf, sizeof(buf)-1);
  close(fd);
  if (n < 0) {
    n = 0;
  }
  buf[n] = 0;

  if (contains(buf,"old")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }
  unlink("d.txt");
}

static void 
test_verbose() 
{
  printf("Test: -v verbose output... ");
  create_file("v1.txt", "vvv");

  int out = open("out.txt", O_CREATE|O_WRONLY|O_TRUNC);
  if (out < 0) {
    printf("FAIL (cannot open out.txt)\n");
    return;
  }
  int saved = dup(1);

  close(1);
  dup(out);

  char *argv[] = {"mv", "-v", "v1.txt", "v2.txt", 0};
  run_mv(argv);

  close(out);
  close(1);
  dup(saved);
  close(saved);
  int fd = open("out.txt", O_RDONLY);
  if (fd < 0) {
    printf("FAIL (cannot read out.txt)\n");
    unlink("out.txt");
    unlink("v2.txt");
    return;
  }
  char buf[128];
  int n = read(fd, buf, sizeof(buf)-1);
  close(fd);
  if (n < 0) {
    n = 0;
  }
  buf[n] = 0;

  if (contains(buf,"v1.txt") && contains(buf,"v2.txt")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }
  unlink("out.txt");
  unlink("v2.txt");
}

static void 
test_move_into_dir() 
{
  printf("Test: move into directory... ");
  mkdir("d");

  create_file("x", "data");
  char *argv[] = {"mv", "x", "d", 0};
  run_mv(argv);

  if (!file_exists("x") && file_exists("d/x")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }

  unlink("d/x");
  unlink("d");
}

static void 
test_same_file() 
{
  printf("Test: same file detection... ");
  create_file("s.txt", "same");

  char *argv[] = {"mv", "s.txt", "./s.txt", 0};
  int rc = run_mv(argv);

  if (rc != 0 && file_exists("s.txt")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }
  unlink("s.txt");
}

static void 
test_move_into_dir_with_slash() 
{
  printf("Test: move into dir with trailing slash... ");
  mkdir("ds");
  create_file("x2", "data");
  char *argv[] = {"mv", "x2", "ds/", 0};
  run_mv(argv);
  if (!file_exists("x2") && file_exists("ds/x2")) {
    printf("OK\n");
  } else {
      printf("FAIL\n");
  }
  
  unlink("ds/x2");
  unlink("ds");
}

static void 
test_move_between_dirs() 
{
  printf("Test: move file between directories... ");
  mkdir("dir1");
  mkdir("dir2");
  create_file("dir1/x3", "content");

  char *argv[] = {"mv", "dir1/x3", "dir2", 0};
  run_mv(argv);

  if (!file_exists("dir1/x3") && file_exists("dir2/x3")) {
    printf("OK\n");
  } else {
    printf("FAIL\n");
  }

  unlink("dir2/x3");
  unlink("dir1");
  unlink("dir2");
}

int
main(int argc, char *argv[])
{
  test_simple_move();
  test_force_overwrite();
  test_verbose();
  test_move_into_dir();
  test_same_file();
  test_move_into_dir_with_slash();
  test_move_between_dirs();
  printf("Manual test: run `mv -i src dst` and try typing y/n yourself.\n");
  exit(0);
}
