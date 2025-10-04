#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"


// Set number of lines to display on the users screen
// (seen in more.c)
#define LINES_PER_PAGE 24 

//Max number of lines from file
#define MAX_LINES 100

// Max chars per line in file
#define LINE_BUF 128	

// Loads given file, takes the liines and puts them into memory as an array of str
char *lines[MAX_LINES];
int total_lines = 0;

// Reads each line from FD and stops when there's a newline or when there's no more space
// in buf. 
// returns 1 = line has been read 
// returns 0 = end of file
int read_line(int fd, char *buf, int size) {
    int i = 0;
    char c;
    while (i < size-1) {
        if (read(fd, &c, 1) <= 0) {
            if (i == 0) return 0;
            break;
        }
        if (c == '\n') break;
        buf[i++] = c;
    }
    buf[i] = 0;
    return 1;
}


// Takes each line from file, line by line, stores it in an array, and loads
// it into memory
void load_file(int fd) {
    char buf[LINE_BUF];
    while (read_line(fd, buf, LINE_BUF)) {
    
        if (total_lines >= MAX_LINES) break;
        lines[total_lines] = malloc(strlen(buf)+1);

        strcpy(lines[total_lines], buf);
        total_lines++;
    }
}


// Reads and handles the user input for a command
char get_command() {
    char c;

    if (read(0, &c, 1) <= 0) return 0;

    // Handles command for error using the escape key, and translates
    // them into smaller chars for the user to input.
    // "\033[" is the given escape sequence code in ANSI
    if (c == '\033') {
        char seq[2];

        // Should red a '[' char
        read(0, &seq[0], 1);

        // After determining that the command is an escape sequence,
        // Read the char (should be A or B) that follows, to determine 
        // if the user wants to scroll up or down. The functionalities of the escape
        // sequence are used to allow an 'escape' from the current line
        read(0, &seq[1], 1);
        
        if (seq[0] == '[') {

        	// Handles escape sequence
        	// A = up arrow on keyboard
        	// B = down arrow on keyboard 
            if (seq[1] == 'A') return 'U'; 
            if (seq[1] == 'B') return 'D'; 
        }
        return 0;
    }

    return c;
}

// Displays the text from file onto the users screen,
// and waits for the users input for a command. 
void less(int fd) {
    load_file(fd);
    int current = 0;

    while (1) {
        // Prints the set 24 lines of text(maximum) onto the screen 
        for (int i = 0; i < LINES_PER_PAGE && current+i < total_lines; i++) {
            printf("%s\n", lines[current+i]);
        }
        printf("--Less--\n");

        char cmd = get_command();

		// Handles less commands 

		// q = quit
        if (cmd == 'q') break;

        // space = go to the next page
        else if (cmd == ' ') {
            current += LINES_PER_PAGE;
            char dummy;
            read(0, &dummy, 1);
        }

		// enter = next line         
        else if (cmd == '\n') current++;

        // b = go back to the previous page 
        else if (cmd == 'b') {
            current -= LINES_PER_PAGE;
            char dummy;
            read(0, &dummy, 1);
        }

		// Go to the beginning/top of the file 
        else if (cmd == 'g') {
            current = 0;
            char dummy;
            read(0, &dummy, 1);
        }

        // Go to the bottom of the file
        else if (cmd == 'G') {
            current = total_lines - LINES_PER_PAGE;
            char dummy;
            read(0, &dummy, 1);
        }

        // up arrow = Go up on the screen
        else if (cmd == 'U') current--;        
        else if (cmd == 'D') current++;        

		 else if (cmd != 0) {
		 printf("Unknown command. Available commands: Space, Enter, b, g, G, Up, Down, q\n");
		 }
		// Prevents program from going outside of the given file, or from the
		// user scrolling past the end/before the start of the file
        if (current < 0) current = 0;
        if (current > total_lines-1) current = total_lines-1;
    }

    // Free memory
    for (int i = 0; i < total_lines; i++) {
        free(lines[i]);
    }
}

// Handles command line/output arguments and interactions 
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: less <file>\n");
        printf("Try 'less --help' for more information.\n");

        exit(1);
    }

        if (strcmp(argv[1], "--help") == 0) {
        printf("Usage: less <filename>\n\n");
        printf("Interactive Navigation:\n");
        printf("  <Space> : Next page\n");
        printf("  <Enter> : Next line\n");
        printf("  b       : Previous page\n");
        printf("  g       : Go to beginning\n");
        printf("  G       : Go to end\n");
        printf("  Up/Down : Scroll up or down one line\n");
        printf("  q       : Quit\n\n");
        printf("Shows --Less-- prompt when waiting for user input.\n");
        exit(0);
    }


    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("less: cannot open %s\n", argv[1]);
        printf("Try 'less --help' for more information.\n");
        exit(1);
    }

    less(fd);
    close(fd);
    exit(0);
}
