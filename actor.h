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

void draw_actors(avect_t* vect);

avect_t* init_actors(level_t* level,
                     int amount_of_entities);

avect_t* create_new_vector(int num);

void add_vector_elem(avect_t* vect, actor_t* newActor);

actor_t* actor_get(avect_t* vect, int num);

void free_actors(avect_t* vect, bool isFull);

bool update_actors(avect_t* vect);

#endif
