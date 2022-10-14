#pragma once

#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>

#include <la/la.h>

typedef int CharacterI; // CharacterIndex
typedef struct World {
	CharacterI player_character;
	V2f        player_pos;
} World;

void world_draw(World *world, ALLEGRO_KEYBOARD_STATE *state, int width, int height);
