#ifndef SOLITAIRE_H
#define SOLITAIRE_H

#include<stdio.h>

#define DECK_SIZE 52
#define TABLE_SIZE 12
#define NODES_SIZE 64

//macros for Linux terminal text colors
#define NORMAL "\x1B[0m"
#define BLACK "\x1B[30m"
#define RED "\x1B[31m"

//macros for indices of the suit lists in the table
#define SPADE 8
#define HEART 9
#define CLUB 10
#define DIAMOND 11

//nodes for doubly linked lists
struct Node{
	struct Card *card;
	struct Node *next;
	struct Node *prev;
};

//representation of a playing card
struct Card{
	//value of the card (1 for ace, 11 for jack, 12 for queen, and 13 for king)
	char val;

	//these two bits determine the suit of a card
	//0 is black, 1 is red
	unsigned color:1;
	//0 is spades or hearts, 1 is clubs or diamonds
	unsigned suit:1;

	//determines if card is face up or face down
	unsigned face:1;
};
//storage for the cards
extern struct Card deck[DECK_SIZE];

//storage for the nodes
extern struct Node nodes[NODES_SIZE];

//array of lists for game management
extern struct Node *table[TABLE_SIZE];

//return corresponding indice for suit
size_t get_suit(struct Node *n);

//returns a 2 character representation of the card
//returns ptr to dynamic array, remember to free
char *to_str(struct Card *card);

//prints a 2 character representation of the card
void print_card(struct Card *card);

//returns a pointer to the specified card in table, or NULL if not found
//cp is 0 if card was from the deck,
//1 if card was from a different column, or
//2 if card was not found
struct Node *find(char *str,char *cp);

//initiallizes the deck with all of the standard cards
void make_cards();

//makes a node for every card
void make_nodes();

//check if a list is empty
char is_empty(struct Node *p);

//move a single node to one past another node
//returns ptr to node that was before p (like move_list)
struct Node *move_node(struct Node *p,struct Node *q);

//swaps the positions of two nodes in the list
void swap_nodes(struct Node *p,struct Node *q);

//shuffle nodes, returns ptr to first node
struct Node *shuffle(unsigned seed);

//move a list (or sublist, from q to NULL) to the end of another list
void cat_lists(struct Node *p,struct Node *q);

//breaks a list, returns ptr to first node of second list
struct Node *break_list(struct Node* p);

//move a list to the end of a specified list in the table
//returns ptr to node before p (before p was moved)
struct Node *move_list(struct Node *p,size_t pos);

//pos gives number of elements to remain in the first list
//returns ptr to second list
struct Node *deal_list(struct Node *p,size_t pos);

//returns last node of list
struct Node *last(struct Node *p);

//organizes the nodes into the table, must be given the first node of the deck
//(which is returned by shuffle)
void make_table(struct Node *start);

//print node information
void print_node(struct Node *n);

//make it look like solitaire
void print_table();

//returns ptr to dynamic string from input (must free it after use)
char *get_line();

//returns a dynamic string of just the first word in cp,
//ip gives index of the beginning of the next word in cp
char *word(char *cp,int *ip);

//returns 0 if game is still going, 1 if won
char won();

//putting everything together
void run();

#endif
