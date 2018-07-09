#pragma once
#include "common.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "behave.h"

#define FLAG_ACTOR_ISPLAYER   0x00000001
#define FLAG_ACTOR_DEAD       0x00000002
#define FLAG_ACTOR_CANWALK    0x00000004
#define FLAG_ACTOR_MELEE      0x00000008
#define FLAG_ACTOR_FRIENDLY   0x00000010
#define FLAG_ACTOR_PROJECTILE 0x00000020

actors_vt* create_actor_vector(int num);

void add_vector_elem(actors_vt* vect, actor_t* newActor);

actor_t* actor_get(actors_vt* vect, int num);

void free_actors(actors_vt* vect);

void delete_actor(actors_vt* vect, int num);
void free_actor(actor_t * actor);

actor_t * make_player();
actor_t * make_monster();
actor_t * make_goblin();
unsigned int give_actor_id();
