#ifndef COMMANDS_H
#define COMMANDS_H

#define DRAW 3

#include"solitaire.h"

//prints a list of commands for the game
void help();

//moves a list to the given table column (0 for suit deck)
//returns 1 if move is legal and made, 0 otherwise
char mv(char *str,size_t i);

//flips over three cards from the deck
//returns pointer to current playable card from deck
//if new_game is nonzero then it will reset playable
//if new_game is 0, it will behave normally
struct Node *draw(char new_game);
#endif
