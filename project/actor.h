#ifndef _ACTOR_05_07_18_11_21
#define _ACTOR_05_07_18_11_21

typedef struct inventory{
	char* name;
	unsigned int item_count;	
}inv_t;

typedef struct actor{
	unsigned char health;
	int speed;
	unsigned char painchance; //if the bullet hits. Actor with any probability runs away (painchance < 256)
	chtype view; 
	unsigned int x;
	unsigned int y;
	inv_t* actor_inventory;
	unsigned char state;	//current state.For example monster search monster
	int flags;						//features of actor(maybe check actor is player)
	unsigned int targx;		//-|____position of target of this actor(player, item, other actor)
	unsigned int targy;		//-|
}actor_t;

typedef struct actors_vect{
	actor_t* allActors;
	unsigned int len;
	unsigned int capacity;	
} avect_t;

void drawActors(avect_t* vect);

avect_t* createNewVector(int num);

avect_t* resizeVector(avect_t* vect);

avect_t* addVectorElem(avect_t* vect, actor_t newActor);

actor_t insertVectorElem(avect_t* vect, int num);

void deleteVector(avect_t* vect);

avect_t* actorsAction(avect_t* vect);

actor_t workWithKeyboard(actor_t actp);

actor_t monsterState(actor_t monster, avect_t* vect, int num);

#endif
