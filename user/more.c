#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"

#define LINES_PER_PAGE 24

char line_buffer[256];

// Struct to hold navigation flags
struct {
    int space;    // Next page
    int enter;    // Next line  
    int quit;        // Quit program
} more_flags;

void 
print_help(void)
{
    printf(" more - Displays the contents of a file one screen at a time\n");
    printf(" -----------------------More--prompt-----------------------\n\n");
    printf(" <Space> + <Return/Enter>       - Display next page of file\n");
    printf(" <Return/Enter>                 - Display next line of file \n");
    printf(" anykey + <Return/Enter>        - Display help \n");
    printf(" q + <Return/Enter>             - Exit/quit from more\n");
}

/**
 * Read one line from file descriptor
 * file - file description, 
 * buffer - pointer to character array where is line store
 * max_len - size of buffer - in this case is 256
 */
int read_line(int file, char *buffer, int max_len) {
    int pos = 0;
    char c;
    
    while (pos < max_len - 1) {
        // read one char from line, if 0 mean file end
        if (read(file, &c, 1) <= 0) {
            if (pos > 0) {
                buffer[pos] = '\0';
                return 1;
            }
            return 0; 
        }

        // If there is newline character or not 
        if (c == '\n') {
            buffer[pos] = '\0';
            return 1;
        }

        buffer[pos++] = c;
    }
    
    buffer[pos] = '\0';
    return 1;
}

/**
 * Wait for user input and set flags
 */
void User_command(void) {
    char key;
    // Start read from keyboard, variable's address, 1 byte for char
    read(0, &key, 1); 
    
    // Reset all flags
    more_flags.space = 0;
    more_flags.enter = 0; 
    more_flags.quit = 0;

    // read(0, &key, 1); 
    // Set the appropriate flag
    switch (key) {
        case ' ':       // Spacebar - next page
            more_flags.space = 1;
            // User typed Space+Enter, throw away the Enter
            read(0, &key, 1);
            break;
        case '\n':      // Enter - next line
            more_flags.enter = 1;
            break;
        case 'q':       // Q - quit
            more_flags.quit = 1;
            /// User typed q+Enter, throw away the Enter
            read(0, &key, 1);
            break;
        default:        // Any other key - help
		    // Read the rest of the line to consume all input at once
		    char temp;
		    while (read(0, &temp, 1) > 0 && temp != '\n') {
		        // Skip to the end of the current line
		    }
            printf("\n");
            printf("Unknown command. Available commands:\n\n");
            print_help();
            printf("--More--\n");
            // Recursively wait for next command after help
             User_command();
            break;
	}
}
/**
 *  more function
 */
void more(int file) {
    int lines_displayed = 0;
    
    while (1) {
        int result = read_line(file, line_buffer, sizeof(line_buffer));
        
        if (result == 0) {
            // End of file - exit silently
            break;
        }
        
        // Print the line
        printf("%s\n", line_buffer);
        lines_displayed++;
        
        // Pause after each screenful
        if (lines_displayed >= LINES_PER_PAGE) {
            printf("--More--\n");
            User_command();
            printf("\n");
            
            if (more_flags.quit) {
                break;
            } else if (more_flags.space) {
                lines_displayed = 0; // Reset for next page
            } else if (more_flags.enter) {
                lines_displayed = LINES_PER_PAGE - 1; // Show one more line
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: more <filename>\n");
         printf("Try 'more --help' for more information.\n");
        exit(1);
    }

    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
    	printf("Usage: more <filename>\n\n");
    	print_help();
    	exit(0);
    }
    
    int file = open(argv[1], O_RDONLY);
    
    if (file < 0) {
        printf("more: cannot open %s\n", argv[1]);
        exit(1);
    }
    
    more(file);
    close(file);
    
    exit(0);
}
