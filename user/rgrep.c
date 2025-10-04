// Recursive grep. Uses same pattern matching as regular grep.
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/fs.h"

char buf[1024];
int match(char*, char*);

// Simple strcat implementation (xv6 doesn't have it)
void
my_strcat(char *dest, char *src)
{
  int dest_len = strlen(dest);
  int i = 0;
  while(src[i] != '\0'){
    dest[dest_len + i] = src[i];
    i++;
  }
  dest[dest_len + i] = '\0';
}

void
rgrep(char *pattern, int fd, char *filename, int invert)
{
  int n, m;
  char *p, *q;
  
  m = 0;
  while((n = read(fd, buf+m, sizeof(buf)-m-1)) > 0){
    m += n;
    buf[m] = '\0';
    p = buf;
    while((q = strchr(p, '\n')) != 0){
      *q = 0;
      int matches = match(pattern, p);
      
      // Normal mode: print if matches
      // Invert mode (-v): print if does NOT match
      if((!invert && matches) || (invert && !matches)){
        write(1, filename, strlen(filename));
        write(1, ": ", 2);
        *q = '\n';
        write(1, p, q+1 - p);
      }
      p = q+1;
    }
    if(m > 0){
      m -= p - buf;
      memmove(buf, p, m);
    }
  }
}

void
search_directory(char *pattern, char *dirname, int invert)
{
  int fd;
  struct dirent de;
  struct stat st;
  char path[512];
  
  if((fd = open(dirname, O_RDONLY)) < 0){
    return;
  }
  
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0) continue;
    
    if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){
      continue;
    }
    
    strcpy(path, dirname);
    if(path[strlen(path)-1] != '/'){
      my_strcat(path, "/");
    }
    my_strcat(path, de.name);
    
    if(stat(path, &st) < 0){
      continue;
    }
    
    if(st.type == T_FILE){
      int file_fd;
      if((file_fd = open(path, O_RDONLY)) >= 0){
        rgrep(pattern, file_fd, path, invert);
        close(file_fd);
      }
    } else if(st.type == T_DIR){
      search_directory(pattern, path, invert);
    }
  }
  
  close(fd);
}

int
main(int argc, char *argv[])
{
  char *pattern;
  char *search_path = ".";
  int invert = 0;
  int arg_index = 1;
  
  if(argc > 1 && strcmp(argv[1], "-v") == 0){
    invert = 1;
    arg_index = 2;
  }
  
  if(argc < arg_index + 1){
    fprintf(2, "usage: rgrep [-v] pattern [directory]\n");
    exit(1);
  }
  
  pattern = argv[arg_index];
  
  if(argc > arg_index + 1){
    search_path = argv[arg_index + 1];
  }
  
  struct stat st;
  if(stat(search_path, &st) < 0){
    printf("rgrep: cannot access %s\n", search_path);
    exit(1);
  }
  
  if(st.type == T_FILE){
    int fd;
    if((fd = open(search_path, O_RDONLY)) < 0){
      printf("rgrep: cannot open %s\n", search_path);
      exit(1);
    }
    rgrep(pattern, fd, search_path, invert);
    close(fd);
  } else if(st.type == T_DIR){
    search_directory(pattern, search_path, invert);
  }
  
  exit(0);
}

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9
int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
  if(re[0] == '^')
    return matchhere(re+1, text);
  do{
    if(matchhere(re, text))
      return 1;
  }while(*text++ != '\0');
  return 0;
}

int
matchhere(char *re, char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[1] == '*')
    return matchstar(re[0], re+2, text);
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

int
matchstar(int c, char *re, char *text)
{
  do{
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  return 0;
}
