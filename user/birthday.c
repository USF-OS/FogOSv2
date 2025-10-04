#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "time.h"

static const int days_per_month[] = {
    31, 28, 31, 30, 31, 30, 
    31, 31, 30, 31, 30, 31
};

int validate_birthdate(char buf[]) {
    if (strlen(buf) != 5) { // not MM/DD or MM/D
        return 0;
    }
    
    int i = 0, month = 0, day = 0;
    while (buf[i] != '\0') {
        if ((buf[i] == '/') ||(buf[i] >= '0' && buf[i] <= '9')) {
            if (i < 2) {
                month = month * 10 + (buf[i] - '0'); 
            } else if (i > 2) {
                day = day * 10 + (buf[i] - '0'); 
            } 
        } 
        ++i;
    }

    if ((month < 1) || (month > 12) || (day > days_per_month[month-1] && month != 2 && day != 29) || (day < 1)) {
        return 0;        
    }
    
    return 1;
}

void print_usage() {
    printf("Usage:\n"
           "birthday            # if set, receive a message\n"
           "birthday -s MM/DD   # (re)set birthday\n"
           "birthday --help      # help command\n"
    );
}

int main(int argc, char *argv[]) {
    /* 
        birthday command in use
        $birthday - No arguments, if birthday is set, return predefined
        statements. else, ask to set birthday.
        $birthday -s  [MM/DD] - Set birthday
    */
    
    struct stat st; // check if birthday file exists
    int fd; // for open, write, close
    char buf[MAXPATH]; // for write

    int month = 0, day = 0,  i = 0; // for comparisons

    int exists = (stat("birthday.txt", &st) == 0);

    // proper length and checking flag
    if ((argc == 3) && (argv[1][0] == '-' && argv[1][1] == 's') && (strlen(argv[1]) == 2)) {
        if (!validate_birthdate(argv[2])) {
            print_usage();
            return -1;
        }
        
        fd = open("birthday.txt", O_CREATE | O_RDWR | O_TRUNC);
            if (fd < 0) { // error
            return -1;
        }
        write(fd, argv[2], strlen(argv[2]));
        close(fd);
        return 0; 
    } 

    if ((argc == 2) && (argv[1][0] == '-' && argv[1][1] == '-'  && argv[1][2] == 'h'  && argv[1][3] == 'e'  && argv[1][4] == 'l' && argv[1][5] == 'p') && (strlen(argv[1]) == 6)) {
        printf("Usage: birthday\n"
               "Usage: birthday [OPTION] [MM/DD]\n"
               "Receive a birthday message from the OS.\n\n"
               "no arguments             if set, receive a message"
               "-s                       reset birthday\n"
               "   --help    display this help and exit\n\n"
        );
        return 0;
    }
    if (argc == 1 && exists) {
        fd = open("birthday.txt", O_RDWR);
        if (fd < 0) { // error
            return -1;
        }
        int n = read(fd, buf, sizeof(buf) - 1);
        
        if(n < 0){
            close(fd);
            return -1;
        }
        buf[n] = '\0';

        while (buf[i] != '\0') {
            if (i < 2) {
                month = month * 10 + (buf[i] - '0'); 
            } else if (i > 2) {
                day = day * 10 + (buf[i] - '0'); 
            }
            ++i;
        }
            
        uint64 c = clock() / 1000000000ULL;
        uint64 local_date = c - (7 * 3600);
        datetime date = to_datetime(local_date);
        
        if (date.month == month && date.day == day) {
            printf(
            "              (        (\n"
            "             ( )      ( )          (\n"
            "      (       Y        Y          ( )\n"
            "     ( )     | |      | |          Y\n"
            "      Y      | |      | |         | |\n"
            "     | |     | |.-----| |---.___  | |\n"
            "     | |  .--| |,~~~~~| |~~~,,,,'-| |\n"
            "     | |-,,~~'-'___   '-'       ~~| |._\n"
            "    .| |~       // ___            '-',,'.\n"
            "   /,'-'     <_// // _  __             ~,\\\n"
            "  / ;     ,-,     \\_> <<_______________;_)\n"
            "  | ;    {(_)} _,      . |================|\n"
            "  | '-._ ~~,,,           | ,,             |\n"
            "  |     '-.__ ~~~~~~~~~~~|________________|   \n"
            "  |\\         `'----------|\n"
            "  | '=._                 |\n"
            "  :     '=.__            |\n"
            "   \\         `'==========|\n"
            "snd '-._                 |\n"
            "        '-.__            |\n"
            "             ``''--------|\n"
            );
            printf("Happy Birthday!\n");
        } else {
            printf("Uh oh. It's not your birthday yet :(\n");
        }
        return 0;    
    }

    print_usage(); // otherwise, fail. 
    return -1;
}
