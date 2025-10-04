#include "kernel/types.h"
#include "user/user.h"

char cwd[MAX_PATH] = "/";

void
getcwd_user(const char *path)
{
  char newpath[MAX_PATH];
  char components[32][64];
  int num_components = 0;
  int i, j, start, len, comp_len;
  int newpath_len = 0;

// Checks between relative path and absolute path
  if (path[0] == '/'){ 
    i = 0; 
    while (path[i] != '\0' && i < MAX_PATH - 1){ 
      newpath[i] = path[i]; 
      i++; 
    } 
    newpath[i] = '\0'; 
    newpath_len = i; 
  } else{ 
    i = 0; 
    while (cwd[i] != '\0' && i < MAX_PATH - 1){ 
      newpath[i] = cwd[i]; 
      i++; 
    } 
    newpath_len = i; 
    if (newpath_len > 0 && newpath[newpath_len - 1] != '/' && newpath_len < MAX_PATH - 1){ 
      newpath[newpath_len] = '/'; 
      newpath_len++; 
    }

    j = 0; 
    while (path[j] != '\0' && newpath_len < MAX_PATH - 1){ 
      newpath[newpath_len] = path[j]; 
      newpath_len++; 
      j++; 
    } 
    newpath[newpath_len] = '\0'; 
  }

// Begin to build components of CWD
  start = 0;
  len = newpath_len;

// This section isolates the component of the cwd so /bin/ would be extracted to put 'bin' in our components[][]
  for (i = 0; i <= len; i++) {
    if (newpath[i] == '/' || newpath[i] == '\0'){
      if (i > start) {
        comp_len = i - start;

        for (j = 0; j < comp_len && j < 63; j++) {
          components[num_components][j] = newpath[start + j];
        }
        components[num_components][j] = '\0';

        if (components[num_components][0] == '.' && components[num_components][1] == '\0'){
            // Nop
        } else if (components[num_components][0] == '.' && components[num_components][1] == '.' && components[num_components][2] == '\0'){
          // '..' is the parent directory so we would get rid of our recent addition
          if (num_components > 0){
            num_components--;
          }
        } else{
          num_components++;
        }
      }
      start = i + 1;
    }
  }

//Slowly build the cwd providing each individual component, putting another '/' then rebuilding until complete
  if (num_components == 0){
    cwd[0] = '/';
    cwd[1] = '\0';
  } else{
    int pos = 0;
    for (i = 0; i < num_components && pos < MAX_PATH - 1; i++){
      cwd[pos] = '/';
      pos++;

      j = 0;
      while (components[i][j] != '\0' && pos < MAX_PATH - 1){
        cwd[pos] = components[i][j];
        pos++;
        j++;
      }
    }
    cwd[pos] = '\0';
  }
}
