#pragma once

#define FLAG_ISPLAYER 0x1
#define FLAG_DEAD 0x2
#define FLAG_CANWALK 0x4
#define FLAG_MELEE 0x8
#define FLAG_FRIENDLY 0x10
#define FLAG_STACKABLE 0x1

#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include "mapgen.h"
#include "behave.h"

void draw_actors(avect_t* vect);

avect_t* init_actors(level_t* level, int amountOfEntities);

avect_t* create_new_vector(int num);

void resize_vector(avect_t* vect);

void add_vector_elem(avect_t* vect, actor_t newActor);

void delete_elem(avect_t* vect, int num);

actor_t* actor_get(avect_t* vect, int num);

void free_actors(avect_t* vect);

bool update_actors(avect_t* vect);

actor_t work_with_keyboard(actor_t actp);

actor_t monster_state(actor_t monster, avect_t* vect, int num);
