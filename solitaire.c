#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include"solitaire.h"
#include"commands.h"

struct Card deck[DECK_SIZE];
struct Node nodes[NODES_SIZE];
struct Node *table[TABLE_SIZE]={NULL};

//return corresponding indice for suit
size_t get_suit(struct Node *n){
	if(n->card->color){
		if(n->card->suit){return DIAMOND;}
		else{return HEART;}
	}else{
		if(n->card->suit){return CLUB;}
		else{return SPADE;}
	}
}
//returns a 2 character representation of the card
//must free returned pointer after use
char *to_str(struct Card *card){
	char val,suit,*p=(char *)malloc(sizeof(char)*3),*q=p;
	if(card->val>1 && card->val<10){val='0'+card->val;}
	else{
		switch(card->val){
			case 1:
				val='A';
				break;
			case 10:
				val='X';
				break;
			case 11:
				val='J';
				break;
			case 12:
				val='Q';
				break;
			case 13:
				val='K';
				break;
			default:
				printf("Not a recognized value.\n");
				return q;
		}
	}
	if(card->color){
		if(card->suit){suit='D';}
		else{suit='H';}
	}else{
		if(card->suit){suit='C';}
		else{suit='S';}
	}
	*p++=val;
	*p++=suit;
	*p='\0';
	return q;
}
//prints a 2 character representation of the card
void print_card(struct Card *card){
	if(!card){
		//printf("EM");
		return;
	}
	if(!card->face){
		printf("XX");
		return;
	}
	char *p=to_str(card);
	if(card->color){printf(RED);}
	else{printf(BLACK);}
	printf("%s",p);
	free(p);
	printf(NORMAL);
}
struct Node *find(char *str,char *cp){
	struct Node **p,*q;
	for(char *cp=str;*cp!='\0';++cp){
		*cp=toupper(*cp);
	}
	for(p=table;p!=table+TABLE_SIZE;++p){
		for(q=*p;q;q=q->next){
			char *r;
			if(q->card){
				r=to_str(q->card);
			}else{
				continue;
			}
			if(strcmp(str,r)==0){
				free(r);
				if(p-table==0){*cp=0;}
				else{*cp=1;}
				return q;
			}
			free(r);
		}
	}
	*cp=2;
	return NULL;
}
//initiallizes the deck with all of the standard cards
void make_cards(){
	struct Card *p=deck;
	for(char c=0;c!=2;++c){
		for(char d=0;d!=2;++d){
			for(char e=1;e!=14;++e){
				//change face to 1 for debugging
				p->val=e,p->suit=c,p->color=d,p->face=0;
				++p;
			}
		}
	}
}
/*
//swaps the indices of two cards in the deck
void swap(struct Card* p,struct Card* q){
	struct Card temp=*p;
	*p=*q;
	*q=temp;
}
//shuffle with a seed for srand (will be time(NULL) after testing)
void shuffle(unsigned seed){
	srand(seed);
	for(struct Card* p=deck;p-deck!=DECK_SIZE;++p){
		swap(p,deck+rand()%DECK_SIZE);
	}
}
//shuffle with a seed for srand (will be time(NULL) after testing)
void shuffle_new(unsigned seed){
	srand(seed);
	for(struct Node* p=nodes;p-nodes!=DECK_SIZE;++p){
		swap_new(p,nodes+rand()%DECK_SIZE);
	}
}
*/
//makes a node for every card
void make_nodes(){
	struct Card *p;
	struct Node *q=nodes;
	for(p=deck;p!=deck+DECK_SIZE;++p){
		q->card=p;
		if(p-deck!=0){q->prev=q-1;}
		else{q->prev=NULL;}
		if(p-deck!=DECK_SIZE-1){q->next=q+1;}
		else{q->next=NULL;}
		++q;
	}
	while(q-nodes!=NODES_SIZE){
		q->prev=NULL;
		q->next=NULL;
		q->card=NULL;
		++q;
	}	
}

char is_empty(struct Node *p){
	return(!p->card && !p->next)?1:0;
}

//move a single node to one past another node
struct Node *move_node(struct Node *p,struct Node *q){
	if(!p || !q){
		printf("Invalid pointer.\n");
		return NULL;
	}else if(p==q || q->next==p){
		return p->prev;
	}else{
		struct Node *r=NULL;
		if(p->prev){
			r=p->prev;
			p->prev->next=p->next;
		}
		if(p->next){p->next->prev=p->prev;}
		p->next=q->next;
		p->prev=q;
		q->next=p;
		if(p->next){p->next->prev=p;}
		return r;
	}
}
//swaps the positions of two nodes in the list
void swap_nodes(struct Node *p,struct Node *q){
	if(p==q){return;}
	struct Node *temp=p;
	move_node(p,q);
	move_node(q,temp);
}
//shuffle nodes, returns ptr to first node
struct Node *shuffle(unsigned seed){
	srand(seed);
	for(struct Node *p=nodes;p-nodes!=DECK_SIZE;++p){
		swap_nodes(p,nodes+rand()%DECK_SIZE);
	}
	struct Node *q;
	for(q=nodes;q->prev;++q){}
	return q;
}
//returns last node of list
struct Node *last(struct Node *p){
	if(!p){return NULL;}
	while(p->next){p=p->next;}
	return p;
}
//move a list (or sublist, from q to NULL) to the end of another list
void cat_lists(struct Node *p,struct Node *q){
	if(!q || !p){
		printf("Invalid pointer.\n");
		return;
	}
	if(is_empty(q)){return;}
	p=last(p);
	q->prev->next=NULL;
	p->next=q;
	q->prev=p;
}
//breaks a list, returns ptr to first node of second list
struct Node *break_list(struct Node *p){
	p->prev->next=NULL;
	p->prev=NULL;
	return p;
}
//move a list to the end of a specified list in the table
//returns ptr to node before p (before p was moved)
struct Node *move_list(struct Node *p,size_t pos){
	struct Node *q=p->prev,*r=(last(*(table+pos)));
	r->next=p;
	p->prev=r;
	if(q){q->next=NULL;}
	return q;
}
//pos gives number of elements to remain in the first list
//returns ptr to second list
struct Node *deal_list(struct Node *p,size_t pos){
	if(pos==0){return p;}
	struct Node *q=p;
	size_t cnt=0;
	while(p && cnt!=pos){
		p=p->next;
		++cnt;
	}
	p->prev->card->face=1;
	return break_list(p);
}
//organizes the nodes into the table, must be given the first node of the deck
//(which is returned by shuffle)
void make_table(struct Node *start){
	struct Node **p,*q=nodes+DECK_SIZE;
	for(p=table;p-table!=TABLE_SIZE;++p){
		*p=q;
		++q;
	}
	for(p=table+1;p!=table+8;++p){
		q=deal_list(start,p-table);
		move_list(start,p-table);
		start=q;
	}
	move_list(start,0);
}
//print node information
void print_node(struct Node *n){
	if(!n){
		printf("No node\n\n");
		return;
	}
	if(n->card){print_card(n->card);}
	else{printf("NULL");}
	printf("\nNext: ");
	if(n->next){print_card(n->next->card);}
	else{printf("NULL");}
	printf("\nPrev: ");
	if(n->prev){print_card(n->prev->card);}
	else{printf("NULL");}
	putchar('\n');
	//printf("%ld\n",get_suit(n));
}
//make it look like solitaire
void print_table(){
	struct Node **p,*q;
	putchar('\n');
	for(p=table;p!=table+TABLE_SIZE;++p){
		switch(p-table){
			case 0:printf("Deck:");break;
			case SPADE:printf("Spades:  ");break;
			case HEART:printf("Hearts:  ");break;
			case CLUB:printf("Clubs:   ");break;
			case DIAMOND:printf("Diamonds:");break;
			default:printf("%ld:",p-table);
		}
		for(q=*p;q;q=q->next){
			print_card(q->card);
			putchar(' ');
		}
		putchar('\n');
		if(p-table==0 || p-table==7){putchar('\n');}
	}
	putchar('\n');
}
char *get_line(){
	int c,i=0;
	char *p=(char*)malloc(sizeof(char)),*q=p;
	while((c=getchar())!=EOF && c!='\n'){
		*(p+i)=c;
		++i;
		p=realloc(p,sizeof(char)*(i+1));
	}
	*(p+i)='\0';
	return p;
}
char *word(char *cp,int *ip){
	int i=0,j=0;
	char *p=(char*)malloc(sizeof(char));
	while(isspace(*(cp+i))){++i;}
	while(!isspace(*(cp+i)) && *(cp+i)!='\0'){
		*(p+j)=*(cp+i);
		++i,++j;
		p=realloc(p,sizeof(char)*(j+1));
	}
	*(p+j)='\0';
	(*ip)+=i;
	return p;
}
char won(){
	struct Node **p,*q;
	for(p=table;p-table!=8;++p){
		for(q=*p;q;q=q->next){
			if(q->card){return 0;}
		}
	}
	return 1;
}
//putting everything together
void run(){
	char *game=NULL;
	do{
		if(game){free(game);}
		draw(1);
		help();
		make_cards();
		make_nodes();
		make_table(shuffle(time(NULL)));
		while(!won()){
			print_table();
			int loc=0;
			char *p=get_line(),*q=word(p,&loc);
			if(strcmp(q,"m")==0){
				free(q);
				q=word(p+loc,&loc);
				char *r=word(p+loc,&loc);
				mv(q,atoi(r));
				free(q),free(r);
			}else if(strcmp(q,"d")==0 || strcmp(q,"draw")==0){
				free(q);
				draw(0);
			}else if(strcmp(q,"h")==0 || strcmp(q,"help")==0){
				free(q);
				help();
			}else if(strcmp(q,"q")==0 || strcmp(q,"quit")==0){
				free(q);
				free(p);
				break;
			}else{
				free(q);
				printf("Error: unknown command\n");
			}
			free(p);
		}
		if(won()){
			print_table();
			printf("\nCongratulations! You won!");
		}else{
			printf("\nBetter luck next time!");
		}
		printf(" New game? (enter \"q\" to quit, or enter for a new deal)\n");
		game=get_line();
	}while(strcmp(game,"q")!=0 && strcmp(game,"Q")!=0);
	free(game);
}
