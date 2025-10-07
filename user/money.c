#include "money.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int total_money = 0;
int bankroll = 1000;

// used when user goes below 0, so can keep track of total loss
void ensure_bankroll() {
    if (bankroll <= 0) {
        bankroll = 1000;
        total_money -= 1000;
        printf("[Bankroll replenished. -1000 from total]\n");
    }
}

void add_winnings(int amount) {
    bankroll += amount;
    printf("[+%d added to bankroll]\n", amount);
}

int subtract_bet(int amount) {
    if (bankroll < amount) {
        printf("[Not enough bankroll for bet of %d. Auto-refilling.]\n", amount);
        ensure_bankroll();
    }
    bankroll -= amount;
    return amount;
}

void display_balance() {
    printf("\n========== BALANCE ==========\n");
    printf("Total profit/loss: %d\n", total_money);
    printf("Current bankroll:  %d\n", bankroll);
    printf("Projected cashout: %d\n", total_money + bankroll);
    printf("=============================\n\n");
}

int cashout_total() {
    return total_money + bankroll;
}
