#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "money.h"

#define NUM_REELS 3
#define NUM_SYMBOLS 5
#define SPIN_STEPS 15

// symbols for slots
char symbols[NUM_SYMBOLS] = {'$', '@', '#', '*', '7'};

int pseudo_rand(int seed) {
    return (1103515245 * seed + 12345) & 0x7fffffff;
}

// clears terminal screen
void clear_screen() {
    for (int i = 0; i < 30; i++)
        printf("\n");
}

void print_slots(char reels[]) {
    printf("╔═════╦═════╦═════╗\n");
    printf("║     ║     ║     ║\n");
    printf("║  %c  ║  %c  ║  %c  ║\n", reels[0], reels[1], reels[2]);
    printf("║     ║     ║     ║\n");
    printf("╚═════╩═════╩═════╝\n");
}

// simulates the slots spinning
void animate_spin(char reels[]) {
    int t = uptime();
    int final[NUM_REELS];

    // gets the true result
    for (int i = 0; i < NUM_REELS; i++) {
        t = pseudo_rand(t);
        final[i] = t % NUM_SYMBOLS;
    }

    // animates reels individually
    for (int step = 0; step < SPIN_STEPS; step++) {
        for (int i = 0; i < NUM_REELS; i++) {
            // reel stops ealier on reel order 2,1,0
            if (step < SPIN_STEPS - i * 3) {
                t = pseudo_rand(t);
                reels[i] = symbols[t % NUM_SYMBOLS];
            } else {
                reels[i] = symbols[final[i]]; // locks final symbol
            }
        }
        clear_screen();
        print_slots(reels);

        // slows down animation
        for (volatile int j = 0; j < 800000; j++);
    }
}

//"main"
void slots_game() {
    char reels[NUM_REELS];
    char input[10];
    int bet = 0;

    printf("\nWelcome to the Slot Machine!\n");
    display_balance();

    while (1) {
        printf("Enter your bet amount (or 0 to return to menu): ");
        gets(input, 10);
        bet = atoi(input);

        if (bet == 0) {
            printf("Returning to Casino Menu...\n");
            break;
        }

        if (bet < 0) {
            printf("Invalid bet.\n");
            continue;
        }

        subtract_bet(bet);

        printf("Press Enter to spin...");
        gets(input, 10);

        animate_spin(reels);

        if (reels[0] == reels[1] && reels[1] == reels[2]) {
            int win = bet * 5; // 5x jackpot
            printf("🎉 JACKPOT! You won %d!\n", win);
            add_winnings(win);
        } else if (reels[0] == reels[1] || reels[1] == reels[2] || reels[0] == reels[2]) {
            int win = bet * 2; // 2x two-match
            printf("⭐ Two symbols matched! You won %d!\n", win);
            add_winnings(win);
        } else {
            printf("No match. You lost %d.\n", bet);
        }

        ensure_bankroll();
        display_balance();
    }
}


