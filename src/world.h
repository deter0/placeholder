#pragma once

#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>
#include <mathc/mathc.h>

typedef int CharacterI; // CharacterIndex
typedef struct World {
	CharacterI player_character;
	mfloat_t player_pos[VEC2_SIZE];
} World;

void world_draw(World *world, ALLEGRO_KEYBOARD_STATE *state, int width, int height);
