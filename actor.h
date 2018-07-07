#pragma once

#ifndef _ACTOR_05_07_18_11_21
#define _ACTOR_05_07_18_11_21

#define FLAG_ISPLAYER 0x1
#define FLAG_DEAD 0x2

#include <math.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "behave.h"
#include "mapgen.h"

void drawActors(avect_t* vect);

avect_t* initActors(level_t* level, int amountOfEntities);

avect_t* createNewVector(int num);

void resizeVector(avect_t* vect);

void addVectorElem(avect_t* vect, actor_t newActor);

actor_t* actor_get(avect_t* vect, int num);

void freeActors(avect_t* vect);

bool updateActors(avect_t* vect);

actor_t workWithKeyboard(actor_t actp);

actor_t monsterState(actor_t monster, avect_t* vect,
                     int num);

#endif
