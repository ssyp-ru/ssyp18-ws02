#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include "graphics.h"
#include "generate.h"
#include "actor.h"

int main(){
	map_t* mapp = createMap(100, 40);
	srand(time(NULL));
	initCurs(mapp);
	avect_t* vect = createNewVector(1);
	actor_t newActor;
	newActor.health = 100;
	newActor.speed = 2;
	newActor.painchance = 100;
	newActor.view = '@' | A_NORMAL;
	newActor.x = 20;
	newActor.y = 30;
	newActor.actor_inventory = NULL;
	newActor.state = 'a';
	newActor.flags = 0;
	newActor.targx = 5;
	newActor.targy = 6;
 	vect = addVectorElem(vect, newActor);
	do{
		actorsAction(vect);
		drawActors(vect);
		clear();
		usleep(200000);
	}while(getch() != 'q');
	deleteVector(vect);
	exitWindow(mapp);
}
