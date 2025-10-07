#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "money.h"

// random generator for dice rolls
unsigned long long seed = 88172645463325252ull;
int myrand() {
    seed = seed * 6364136223846793005ull + 1;
    return (int)(seed >> 33);
}

// clears sapce
void full_clear() {
    for (int i = 0; i < 50; i++)
        printf("\n");
}

// dice face map as strings
const char *die_faces[6][5] = {
    {"+-----+", "|     |", "|  o  |", "|     |", "+-----+"}, // 1
    {"+-----+", "|o    |", "|     |", "|    o|", "+-----+"}, // 2
    {"+-----+", "|o    |", "|  o  |", "|    o|", "+-----+"}, // 3
    {"+-----+", "|o   o|", "|     |", "|o   o|", "+-----+"}, // 4
    {"+-----+", "|o   o|", "|  o  |", "|o   o|", "+-----+"}, // 5
    {"+-----+", "|o   o|", "|o   o|", "|o   o|", "+-----+"}  // 6
};

// draws dices side by side
void draw_die_pair(int d1, int d2) {
    const char **face1 = die_faces[d1 - 1];
    const char **face2 = die_faces[d2 - 1];

    for (int i = 0; i < 5; i++) {
        printf("%s  %s\n", face1[i], face2[i]);
    }
}

// rolls and draw two dice
int roll_dice() {
    int d1 = (myrand() % 6) + 1;
    int d2 = (myrand() % 6) + 1;

    printf("You rolled:\n\n");
    draw_die_pair(d1, d2);
    return d1 + d2;
}

// "main"
void craps_game() {
    char buf[10];
    int bet = 0;

    seed = uptime(); // initialize RNG (based off clock tick)
    full_clear();
    printf("Welcome to  CRAPS!\n");
    display_balance();

    while (1) {
        printf("Enter your bet amount (or 0 to return to menu): ");
        gets(buf, sizeof(buf));
        bet = atoi(buf);

        if (bet == 0) {
            printf("Returning to Casino Menu...\n");
            break;
        }
        if (bet < 0) {
            printf("Invalid bet amount.\n");
            continue;
        }

        subtract_bet(bet);

        printf("\nPress Enter to roll, or type q to quit: ");
        gets(buf, sizeof(buf));
        if (buf[0] == 'q' || buf[0] == 'Q') {
            printf("Returning to menu...\n");
            add_winnings(bet); // refund since they didn't play
            break;
        }

        int sum = roll_dice();

        // gets result of fisrt roll
        if (sum == 7 || sum == 11) {
            int win = bet * 2;
            printf("\nYou rolled %d — NATURAL! You WIN %d!\n", sum, win);
            add_winnings(win);
        } else if (sum == 2 || sum == 3 || sum == 12) {
            printf("\nYou rolled %d — CRAPS! You LOSE %d!\n", sum, bet);
            // loss already handled by subtract_bet
        } else {
            int point = sum;
            printf("\nPoint is set to %d. Roll until %d (win) or 7 (lose).\n", point, point);

			// needs to hit point to win, 7 to lose
            while (1) {
                printf("\nPress Enter to roll again...");
                gets(buf, sizeof(buf));
                sum = roll_dice();

                if (sum == point) {
                    int win = bet * 2;
                    printf("\nYou rolled %d — You WIN %d!\n", sum, win);
                    add_winnings(win);
                    break;
                } else if (sum == 7) {
                    printf("\nYou rolled 7 — You LOSE %d!\n", bet);
                    break;
                } else {
                    printf("\nYou rolled %d — Keep rolling...\n", sum);
                }
            }
        }

        ensure_bankroll();
        display_balance();

			char buf[8];

		while (1) {
   			printf("Play again? (y/n): ");
    		gets(buf, sizeof(buf));

    		if (buf[0] == 'y' || buf[0] == 'Y') {
        		break;
    		}
    		if (buf[0] == 'n' || buf[0] == 'N') {
        		return;
    		}
    		printf("Invalid input. Please enter 'y' or 'n'.\n");
		}
        
    }

    printf("\nFinal cashout: %d\n", cashout_total());
    printf("Returning to Casino Menu...\n");
}
