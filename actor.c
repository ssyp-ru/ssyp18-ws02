#include "actor.h"
#include "common.h"
#include "behave.h"


avect_t* createNewVector(int num){
	avect_t* newVect = (avect_t*)malloc(sizeof(avect_t));
	newVect->allActors = (actor_t*)calloc(num, sizeof(actor_t));
	newVect->len = 0;
	newVect->capacity = num;
	return newVect;
}


avect_t* initActors(level_t * level, int amountOfEntities)
{
	avect_t* actors = createNewVector(amountOfEntities + 1);
	
	int roomNumber = rand() % level->map->rooms->length;
	actor_t player;
	player.symbol = '@' | COLOR_PAIR(1);
	player.x = vector_get(level->map->rooms, roomNumber).x
	+ vector_get(level->map->rooms, roomNumber).width / 2;
	player.y = vector_get(level->map->rooms, roomNumber).y
	+ vector_get(level->map->rooms, roomNumber).height / 2;
	player.flags |= FLAG_ISPLAYER;
	player.behave = behave_player;
	player.level = level;
	addVectorElem(actors, player);
	return actors;	
}


void drawActors(avect_t* actors)
{
	for(int i = 0; i < actors->len; i++)
	{
		mvaddch(actor_get(actors, i)->y, actor_get(actors, i)->x,
		actor_get(actors, i)->symbol);
	}
}


void resizeVector(avect_t* vect){
	vect->allActors = (actor_t*)realloc(vect->allActors, vect->capacity * 2);
	vect->capacity = vect->capacity * 2;
}


void addVectorElem(avect_t* vect, actor_t newActor){
	if (vect->capacity - vect->len == 0)
		resizeVector(vect);
	vect->allActors[vect->len] = newActor;
	vect->len += 1;
}


actor_t* actor_get(avect_t* vect, int num){
	return &vect->allActors[num];
}


void freeActors(avect_t* vect){
	free(vect->allActors);
	free(vect);
}


bool updateActors(avect_t* vect){
	for (int i = 0; i < vect->len; i++){
	  	actor_t * current_actor = actor_get(vect, i);
		if(current_actor->behave(current_actor) == 0)
		{
			return false;
		}
	}
	return true;
}


