#include<stdio.h>
#include<stdlib.h>
#include"commands.h"
#include"solitaire.h"

void help(){
	printf("Command Line Solitaire Commands:\n\nm \"card\" \"[col]\": move \"card\" and any cards following it to column \"col\" or to the suit decks if no column is specified or if 0 is specified\ne.g. m xd 4 moves the Ten of Diamonds to column four, m 7h 1 moves the Seven of Hearts to column 1, and m as or m as 0 moves the Ace of Spades to the spade deck\n\nd or draw: draw three cards from the deck\n\nh or help: view this again\n\nq or quit: quit\n\n");

}

//points to one before the current playable card in the deck
struct Node *playable=NULL;

char mv(char *str,size_t dest){
	if(dest<0 || dest>7){
		printf("Error: list must be within [0,7]\n");
		return 0;
	}
	char from;
	struct Node *q=find(str,&from);
	if(!q){
		printf("Error: Unknown card\n");
		return 0;
	}
	if(q->card->face==0){
		printf("Error: Card is unseen\n");
		return 0;
	}
	struct Node **p=table,*r;
	if(dest!=0){
		r=last(*(p+dest));
		if((r->card && r->card->color!=q->card->color && r->card->val==q->card->val+1 && r->card->face==1) || (!r->card && q->card->val==13)){
			if(from==1){
				r=move_list(q,dest);
				if(r->card){r->card->face=1;}
				return 1;
			}else{
				if(playable->next && q==playable->next){
					move_node(q,r);
					return 1;
				}else{
					printf("Error: Illegal move\n");
					return 0;
				}
			}
		}
		printf("Error: Illegal move\n");
		return 0;
	}else{
		size_t suit=get_suit(q);
		r=last(*(p+suit));
		if((r->card && r->card->val==q->card->val-1) || (!r->card && q->card->val==1)){
			if(from==1){
				r=move_node(q,r);
				if(r->card){r->card->face=1;}
				return 1;
			}else{
				if(playable->next && q==playable->next){
					move_node(q,r);
					return 1;
				}else{
					printf("Error: Illegal move\n");
					return 0;
				}
			}
		}
		printf("Error: Illegal move\n");
		return 0;
	}
}

struct Node *draw(char new_game){
	if(new_game){
		playable=NULL;
		return NULL;
	}
	if(!last(*table)->card){
		printf("Error: No cards to draw\n");
		return NULL;
	}
	if(!playable){
		playable=last(*table);
	}
	if(playable->card){
		char i=0;
		do{
			playable->card->face=1;
			playable=playable->prev;
			++i;
		}while(i!=DRAW && playable->card);
	}else{
		while(playable){
			if(playable->card){
				playable->card->face=0;
			}
			playable=playable->next;
		}
	}
	return playable;
}
