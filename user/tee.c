#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

/*
 * fd 0 = standard input (keyboard, pipe, etc)
 * fd 1 = standard output (terminal)
 */

int main(int argc, char *argv[]){

	char data[512];
	int bytesToRead;
	struct stat st;
	int offset;
	int append = 0;
	int fds[10];
	int fd_count = 0;
	int flags = 0;
	int file_start;

	if(strcmp(argv[1], "-a") == 0){
		append = 1;
		flags = O_CREATE | O_RDWR;
		
		// For starting at the correct file when opening them
		file_start = 2;

	} else {
		flags = O_CREATE | O_WRONLY | O_TRUNC;
		
		// For starting at correct file
		file_start = 1;
	}

	for(int i = file_start; i < argc; i++){
		
		// [i - file_start]: open the files at fds[0] instead; empty slot(s) at fds[0] since i would start at 1 or 2
		fds[i - file_start] = open(argv[i], flags);
		fd_count++;
		
		if(fds[i - file_start] < 0){
			printf("Error in opening file: %s\n", argv[i]);
		}
	}
		
		

	while((bytesToRead = read(0, data, sizeof(data))) > 0){
				
		// Writing to the terminal
		write(1, data, bytesToRead);

		// Writing to files
		for(int i = 0; i < fd_count; i++){

			if(append == 1) {
				
				// Appending logic
				fstat(fds[i], &st);	// Get the file's information
				offset = st.size;
				
				seek(fds[i], offset);	// Move the write pointer to the end of the file
			}

			// Writing to the file
			write(fds[i], data, bytesToRead);
			
			// Writing a newline at the end of the file
			if(bytesToRead > 0 && data[bytesToRead - 1] != '\n'){
				write(fds[i], "\n", 1);
				write(1, "\n", 1);
			}
		}
	}

	// Closing files
	for(int i = 0; i < fd_count; i++){
		close(fds[i]);
	}

	return 0;
}
