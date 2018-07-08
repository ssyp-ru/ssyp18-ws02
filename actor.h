#pragma once
#include <ncurses.h>
#define FLAG_FRIENDLY 0x1
#define FLAG_STACKABLE 0x1
#include "common.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "behave.h"


void draw_actors(avect_t* vect);

avect_t* init_actors(level_t* level,
                     int amount_of_entities);

avect_t* create_new_vector(int num);

void add_vector_elem(avect_t* vect, actor_t* newActor);

actor_t* actor_get(avect_t* vect, int num);

void free_actors(avect_t* vect, bool isFull);



