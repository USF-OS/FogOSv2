#include "kernel/types.h"
#include "user/user.h"

/*
clear: writes an ANSI escape sequence to stdout to clear the screen
and move the cursor to (1,1).

Sequence:
 \x1b[2J] -> clear entire screen
 \x1b[H   -> move cursor to top-left

*/

int
main(void) {
	static const char seq[] = "\x1b[2J\x1b[H";
	int to_write = sizeof(seq) - 1;
	int written = 0;

	// loop until all bytes are written in order to avoid partial writes
	while (written < to_write) {
		int n = write(1, seq + written, to_write - written);
		if (n < 0) {
			write(2, "clear: write failed\n", 21);
			exit(1);
		}
		written += n;
	} 
	exit(0);                    
}                                  
