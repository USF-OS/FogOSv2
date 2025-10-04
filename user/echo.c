#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
print_help(void)
{
    printf("Usage: echo [OPTION]... [STRING]...\n");
    printf("Echo the STRING(s) to standard output.\n\n");
    printf("Options:\n");
    printf("  -n        do not output the trailing newline\n");
    printf("  --help    display this help and exit\n");
}


int
main(int argc, char *argv[])
{
        int flag_n = 0;
    int i;

    // If no arguments, handle based on flags
    if (argc <= 1) {
        if (!flag_n) {
            write(1, "\n", 1);
        }
        exit(0);
    }

    // Parse command line options
    for (i = 1; i < argc; i++) {
        // Stop at first non-option argument
        if (argv[i][0] != '-') {
            break;
        }

        // Handle --help
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            exit(0);
        }
        // Parse single-character options
        char *option = argv[i] + 1; // Skip the '-'
        while (*option) {
            switch (*option) {
                case 'n':
                    flag_n = 1;
                    break;
               default:
                    printf("echo: unknown option -%c\n", *option);
                    printf("Try 'echo --help' for more information.\n");
                    exit(1);
            }
            option++;
        }
    }

	// Print all non-option arguments
	int printAny = 0;
	for (; i < argc; i++) 
	{ 
	    write(1, argv[i], strlen(argv[i]));
	    printAny = 1;
	    if (i + 1 < argc) {
	        write(1, " ", 1);
	    }
	}

	// Print newline unless -n flag is set
	if (!flag_n && printAny) {  // Check flag_n!
	    write(1, "\n", 1);
	}

	exit(0);
  
}
