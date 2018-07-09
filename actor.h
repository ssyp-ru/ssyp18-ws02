#pragma once
#include <ncurses.h>
#define FLAG_FRIENDLY 0x1
#define FLAG_STACKABLE 0x1
#define FLAG_ISPLAYER 0x1
#include "common.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "behave.h"


actors_vt* init_actors(level_t* level,
                     int amount_of_entities);

actors_vt* create_new_vector(int num);

void add_vector_elem(actors_vt* vect, actor_t* newActor);

actor_t* actor_get(actors_vt* vect, int num);

void free_actors(actors_vt* vect, bool isFull);

