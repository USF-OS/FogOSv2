#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "money.h"

void craps_game();
void blackjack_game();
void slots_game();

int main() {
    printf("    _        ______   _______    _____        ____    ____  \n");
    printf("   | |      |  ____| |__   __|  / ____|      / ___|  / __ \\ \n");
    printf("   | |      | |__       | |    | (___       | |  _  | |  | |\n");
    printf("   | |      |  __|      | |     \\___ \\      | | | | | |  | |\n");
    printf("   | |____  | |____     | |     ____) |     | |_| | | |__| |\n");
    printf("   |______| |______|    |_|    |_____/       \\____|  \\____/ \n");
    printf("\n");

    printf(" █████   ███    █   █   █████   █      █████   █     █   █████   ██  \n");
    printf("█       █   █   ██ ██   █    █  █        █     ██    █  █        ██  \n");
    printf("█  ███  █████   █ █ █   █████   █        █     █ █   █  █  ███   ██  \n");
    printf("█    █  █   █   █   █   █    █  █        █     █  █  █  █    █   ██  \n");
    printf("█    █  █   █   █   █   █    █  █        █     █   █ █  █    █      \n");
    printf(" █████  █   █   █   █   █████   █████  █████   █    ██   █████   ██  \n");
    printf("\n");

    int choice;

    while (1) {
        printf("\n=====================\n");
        printf(" Casino Menu\n");
        printf("=====================\n");
        printf("1. Craps\n");
        printf("2. Blackjack\n");
        printf("3. Slots\n");
        printf("4. Cashout (quit)\n");
        printf("Enter your choice: ");

        char buf[4];
        gets(buf, sizeof(buf));
        choice = atoi(buf);

        switch (choice) {
            case 1:
                craps_game();
                break;
            case 2:
                blackjack_game();
                break;
            case 3:
                slots_game();
                break;
            case 4:
                printf("Cashing out: %d\n", cashout_total());
                printf("Goodbye!\n");
                
                exit(0);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
