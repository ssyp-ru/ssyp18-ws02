#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include "graphics.h"
#include "generate.h"
#include "actor.h"

void drawActors(avect_t* vect){
	for (int i = 0; i < vect->len; i++){
		move(vect->allActors[i].x, vect->allActors[i].y);
		addch(vect->allActors[i].view | A_NORMAL);
	}
	refresh();
}

avect_t* createNewVector(int num){
	avect_t* newVect = (avect_t*)malloc(sizeof(avect_t));
	newVect->allActors = (actor_t*)malloc(sizeof(actor_t) * num);
	newVect->len = 0;
	newVect->capacity = num;
	return newVect;
}

avect_t* resizeVector(avect_t* vect){
	vect->allActors = (actor_t*)realloc(vect->allActors, vect->capacity * 2);
	vect->capacity = vect->capacity * 2;
	return vect;
}

avect_t* addVectorElem(avect_t* vect, actor_t newActor){
	if (vect->capacity - vect->len == 0)
		vect = resizeVector(vect);
	vect->allActors[vect->len] = newActor;
	vect->len += 1;
	return vect;
}

actor_t insertVectorElem(avect_t* vect, int num){
	return vect->allActors[num];
}

void deleteVector(avect_t* vect){
	free(vect->allActors);
	free(vect);
}

avect_t* actorsAction(avect_t* vect){
	for (int i = 0; i < vect->len; i++){
		vect->allActors[i] = monsterState(vect->allActors[i], vect, i);
	}
	return vect;
}

actor_t workWithKeyboard(actor_t actp){
	chtype data = getch();
	switch(data){
		case 'h':
			actp.x -= 1;
		break;
		case 'l':
			actp.x += 1;
		break;
		case 'k':
			actp.y -= 1;
		break;
		case 'j':
			actp.y += 1;
		break;
	}
	return actp;	
}

actor_t monsterState(actor_t monster, avect_t* vect, int num){
	int actpoint = 0;
	switch(monster.state){
		case 'a'://monster search target
			for (int i = 0; i < vect->len; i++){ //search new target
				if (i != num){
					if (abs(vect->allActors[i].x - monster.x) <= 3 &&
							abs(vect->allActors[i].x - monster.x) <= 3){
						monster.targx = vect->allActors[i].x;
						monster.targy = vect->allActors[i].y;
						monster.state = 'b';	//new state(chase)
						break;		
					}
				}
			}
//---------------------------------------------------------
			if (monster.state != 'b'){ //if target not found
				int i = rand() % 8;	//actor walks and search target
				switch(i){
					case 0:
						monster.x += monster.speed;
					break;
					case 1:
						monster.x += monster.speed / 2;
						monster.y += monster.speed / 2;
					break;
					case 2:
						monster.y += monster.speed;
					break;
					case 3:
						monster.x -= monster.speed / 2;
						monster.y += monster.speed / 2;
					break;
					case 4:
						monster.x -= monster.speed;
					break;
					case 5:
						monster.x -= 1;
						monster.y -= 1;
					break;
					case 6:
						monster.y -= 1;
					break;
					case 7:
						monster.y -= 1;
						monster.x += 1;
					break;
				}
			}
		break;
//---------------------------------------------------------
		case 'b':
			if (monster.x == monster.targx && monster.y != monster.targy){
				for (actpoint = monster.speed; actpoint > 0
						&& monster.y != monster.targy; actpoint--){
					if (monster.y > monster.targy){
						monster.y--;
					} else {
						monster.y++;
					}		
				}
			}
//---------------------------------------------------------
			if (monster.y == monster.targy && monster.x != monster.targx){
				for (actpoint = monster.speed; actpoint > 0
					 	&& monster.x != monster.targx; actpoint--){
					if (monster.x > monster.targx){
						monster.x--;
					} else {
						monster.x++;
					}		
				}
			}
//---------------------------------------------------------
			if (monster.x != monster.targx && monster.y != monster.targy){
				for (actpoint = monster.speed; actpoint > 0 && monster.y !=
					 	monster.targy && monster.x != monster.targx; actpoint--){
					if (monster.y > monster.targy){
						monster.y--;
					} else {
						monster.y++;
					}	
					if (monster.x > monster.targx){
						monster.x--;
					} else {
						monster.x++;
					}		
				}
				if (actpoint != 0){
					if (monster.x != monster.targx){
						for (actpoint = monster.speed; actpoint > 0
							 	&& monster.x != monster.targx; actpoint--){
							if (monster.x > monster.targx){
								monster.x--;
							} else {
								monster.x++;
							}		
						}
					}
					if (monster.y != monster.targy){
						for (actpoint = monster.speed;	actpoint > 0
							 	&& monster.y != monster.targy; actpoint--){
							if (monster.y > monster.targy){
								monster.y--;
							} else {
								monster.y++;
							}	
						}
					}
				}
			}
//---------------------------------------------------------
			char flag = 'b';
			for (int i = 0; i < vect->len; i++){ //search new position of target
				if (i != num){
					if (abs(vect->allActors[i].x - monster.x) <= 3 &&
						abs(vect->allActors[i].x - monster.x) <= 3){
						monster.targx = vect->allActors[i].x;
						monster.targy = vect->allActors[i].y;
						flag = 's';
						break;		
					}
				}
			}									//                                            |
//---------------------------------------------------------           |
			if (flag == 's'){ //if actor find new position of target and    V
				if (monster.x == monster.targx && monster.y == monster.targy){ //if target
					monster.state = 'c';	//near with actor new state attack;
				}
			} else {
				monster.state = 'a';	//target loss. Search new target
			}
		break;
	}
	return monster;
}
