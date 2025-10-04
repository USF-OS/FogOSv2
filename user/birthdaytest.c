#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int run(char *args[]) {
    int pid = fork();
    if (pid == 0) {
        // childs job
        exec("birthday", args);
        // if fail
        printf("exec failed");
        exit(-1);  
    } else if (pid > 0) {
        // parents job
        int status;
        wait(&status);
        return status;  // status = childs exit numbe
    } else {
        printf("fork failed\n");
        return -1;
    }
}

int main() {
    int tests[5];
    char* test_desc[5];

    // should return -1
    char *args1[] = {"birthday", 0};
    tests[0] = run(args1);
    test_desc[0] = "Run birthday command without setting it";

    // should return 0
    char *args2[] = {"birthday", "--help", 0};
    tests[1] = run(args2);
    test_desc[1] = "Run birthday command with help option";

    // should return 0
    char *args3[] = {"birthday", "-s", "12/25", 0};
    tests[2] = run(args3);
    test_desc[2] = "Run birthday command with set option and valid birthday";

    // should return 0
    char *args4[] = {"birthday", 0};
    tests[3] = run(args4);
    test_desc[3] = "Run birthday command with a set birthday";

    // should return -1
    char *args5[] = {"birthday", "-s", "13/99", 0};
    tests[4] = run(args5);
    test_desc[4] = "Run birthday command with set option and invalid birthday";

    for (int i = 0; i < 5; i++) {
        if ((i == 0 && tests[i] != 0) || (i == 4 && tests[i] != 0) || (tests[i] == 0)) {
            printf("[PASS] Test %d: %s\n", i, test_desc[i]);
        } else {
            printf("[FAIL] Test %d: %s\n", i, test_desc[i]);
        }
    }
    
    return 0;
}
