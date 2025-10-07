#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "money.h"

#define NUM_DECKS 3
#define NUM_SUITS 4
#define NUM_RANKS 13
#define DECK_SIZE (52 * NUM_DECKS)
#define MAX_HAND 12
#define MAX_SPLITS 4

typedef struct {
    char rank[3];
    char suit[4];
    int value;
} Card;

// global vars
char *ranks[NUM_RANKS]  = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
int   values[NUM_RANKS] = {11,2,3,4,5,6,7,8,9,10,10,10,10};
char *suits[NUM_SUITS]  = {"♠","♥","♦","♣"};

// creates a random num based of seed
int pseudo_random(int seed) {
    return (1103515245 * seed + 12345) & 0x7fffffff;
}

void shuffle_deck(Card *deck, int size) {
    int t = uptime();
    for (int i = size-1; i > 0; i--) {
        t = pseudo_random(t);
        int j = t % (i+1);
        Card tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
}

// prints hand side by side
void print_hand(Card hand[], int count, int hide_first) {
    for (int row = 0; row < 5; row++) {
        for (int i = 0; i < count; i++) {
            if (i == 0 && hide_first) {
                switch (row) {
                    case 0: printf("+-----+ "); break;
                    case 1: printf("|/////| "); break;
                    case 2: printf("|/////| "); break;
                    case 3: printf("|/////| "); break;
                    case 4: printf("+-----+ "); break;
                }
            } else {
                char *r = hand[i].rank;
                switch (row) {
                    case 0: printf("+-----+ "); break;
                    case 1:
                        if (strlen(r) == 1) printf("|%s    | ", r); 	// for single chars
                        else printf("|%s   | ", r);						// double chars
                        break;
                    case 2: printf("|  %s  | ", hand[i].suit); break;
                    case 3:
                        if (strlen(r) == 1) printf("|    %s| ", r);		// for single chars
                        else printf("|   %s| ", r);						// double chars
                        break;
                    case 4: printf("+-----+ "); break;
                }
            }
        }
        printf("\n");
    }
}

// gets hand value
int calculate_hand(Card hand[], int count) {
    int total = 0;
    int aces = 0;
    for (int i = 0; i < count; i++) {
        total += hand[i].value;
        if (hand[i].value == 11) {
        	aces++;
        }
    }
    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }
    return total;
}


int split_hand(Card player_hands[MAX_SPLITS][MAX_HAND], int hand_counts[MAX_SPLITS],
               int current_hand, int *active_hands, Card deck[], int *deck_pos) {

    if (*active_hands >= MAX_SPLITS) {
        printf("Maximum splits reached!\n");
        return 0;
    }
    
    if (hand_counts[current_hand] != 2) {
        printf("Can only split with 2 cards.\n");
        return 0;
    }
    if (strcmp(player_hands[current_hand][0].rank, player_hands[current_hand][1].rank) != 0) {
        printf("Cards don't match.\n");
        return 0;
    }
    
   // create new hand at the next available index, moves card wanting to split to new hand
    int new_hand = *active_hands;
    player_hands[new_hand][0] = player_hands[current_hand][1];
    hand_counts[new_hand] = 1;
    hand_counts[current_hand] = 1;

	//deals new card to original hand and new hand
    player_hands[current_hand][hand_counts[current_hand]++] = deck[(*deck_pos)++];
    player_hands[new_hand][hand_counts[new_hand]++] = deck[(*deck_pos)++];

    (*active_hands)++;
    printf("Hand split! Now %d hands.\n", *active_hands);
    return 1;
}

// "main"
int blackjack_game() {
	printf("\nWelecome to BLACKJACK!!!\n");
    while (1) {
        display_balance();

        char buf[16];
        printf("Enter bet amount (0 to quit): ");
        gets(buf, sizeof(buf));
        int bet = atoi(buf);
        if (bet <= 0) {
            printf("\nLeaving the blackjack table.\n");
            break;
        }

        subtract_bet(bet);

        Card deck[DECK_SIZE];
        int deck_pos = 0;
        int idx = 0;
        for (int d = 0; d < NUM_DECKS; d++) {
            for (int s = 0; s < NUM_SUITS; s++) {
                for (int r = 0; r < NUM_RANKS; r++) {
                    strcpy(deck[idx].rank, ranks[r]);
                    strcpy(deck[idx].suit, suits[s]);
                    deck[idx].value = values[r];
                    idx++;
                }
            }
        }
        
        shuffle_deck(deck, DECK_SIZE);

        Card player_hands[MAX_SPLITS][MAX_HAND];
        int hand_counts[MAX_SPLITS] = {0};
        int active_hands = 1;
        Card dealer[MAX_HAND];
        int dealer_count = 0;

        player_hands[0][hand_counts[0]++] = deck[deck_pos++];
        dealer[dealer_count++] = deck[deck_pos++];
        player_hands[0][hand_counts[0]++] = deck[deck_pos++];
        dealer[dealer_count++] = deck[deck_pos++];

        char input[16];

        for (int h = 0; h < active_hands; h++) {
            int turn = 1;
            while (turn) {
                printf("\nDealer's hand:\n");
                print_hand(dealer, dealer_count, 1);

                printf("\nYour hand %d:\n", h + 1);
                print_hand(player_hands[h], hand_counts[h], 0);
                int total = calculate_hand(player_hands[h], hand_counts[h]);
                printf("Total: %d\n", total);

                if (total >= 21) {
                    if (total == 21) printf("Blackjack!\n");
                    else printf("Bust!\n");
                    break;
                }

                int can_split = hand_counts[h] == 2 && active_hands < MAX_SPLITS &&
                    strcmp(player_hands[h][0].rank, player_hands[h][1].rank) == 0;

				// needs h,p,s to continue
				int valid_input = 0;
				while (!valid_input) {
				    if (can_split) {
				        printf("Hit, Split or Stand? (h/p/s): ");
				    } else {
				        printf("Hit or Stand? (h/s): ");
				    }
				
				    int n = read(0, input, sizeof(input));
				    if (n <= 0) {
				        printf("Error reading input.\n");
				        continue;
				    }
				
				    if (input[n - 1] == '\n') {
				        input[n - 1] = '\0';
				    } else {
				        input[n] = '\0'; 
				    }
				
				    if (input[0] == 'h') {
				        player_hands[h][hand_counts[h]++] = deck[deck_pos++];
				        valid_input = 1;
				    } else if (can_split && input[0] == 'p') {
				        split_hand(player_hands, hand_counts, h, &active_hands, deck, &deck_pos);
				        valid_input = 1;
				    } else if (input[0] == 's') {
				        turn = 0;
				        valid_input = 1;
				    } else {
				        if (can_split) {
				            printf("Invalid input. Please enter h, p, or s.\n");
				        } else {
				            printf("Invalid input. Please enter h or s.\n");
				        }
				    }
				}
            }
        }

        printf("\nDealer's turn:\n");
        print_hand(dealer, dealer_count, 0);

        // dealer draws until greater than 17
        while (calculate_hand(dealer, dealer_count) < 17) {
            dealer[dealer_count++] = deck[deck_pos++];
            printf("Dealer hits:\n");
            print_hand(dealer, dealer_count, 0);
        }
        
        int dealer_total = calculate_hand(dealer, dealer_count);
        printf("Dealer total: %d\n", dealer_total);

		// calculates all hands and gives wins, loses, pushes
        for (int h = 0; h < active_hands; h++) {
            int player_total = calculate_hand(player_hands[h], hand_counts[h]);
            printf("\nHand %d total: %d\n", h + 1, player_total);

            if (player_total > 21) printf("Bust. Dealer wins.\n");
            else if (dealer_total > 21 || player_total > dealer_total) {
                printf("You win!\n");
                add_winnings(bet * 2);
            } else if (player_total == dealer_total) {
                printf("Push.\n");
                add_winnings(bet); // returns original bet
            } else {
                printf("Dealer wins.\n");
            }
        }

        display_balance();
		
		while (1) {
		    printf("Play again? (y/n): ");
		    gets(buf, sizeof(buf));
		
		    if (buf[0] == 'y' || buf[0] == 'Y') {
		        break;
		    }
		    if (buf[0] == 'n' || buf[0] == 'N') {
		        return 0;
		    }
		    printf("Invalid input. Please enter 'y' or 'n'.\n");
		}

        
        printf("\n=============================\n\n");
    }

    return 0;
}
