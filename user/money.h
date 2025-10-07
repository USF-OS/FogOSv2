#ifndef MONEY_H
#define MONEY_H

extern int total_money;
extern int bankroll;

void ensure_bankroll();
void add_winnings(int amount);
int subtract_bet(int amount);
void display_balance();
int cashout_total();

#endif
