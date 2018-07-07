#pragma once


#include "common.h"

int behave_monster(actor_t* self);

int behave_player(actor_t* self);

int behave_fire(actor_t* self);

int behave_projectiles(actor_t* self);

int behave_searchtarget(actor_t* self);

int behave_chasetarget(actor_t* self);

int behave_meleeattack(actor_t* self);

int behave_pain(actor_t* self);
