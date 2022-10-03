#pragma once

#include <allegro5/bitmap.h>

typedef struct CharacterBM {
	bool            bm_loaded;
	ALLEGRO_BITMAP *bm;
	unsigned int    bm_w;
	unsigned int    bm_h;	
} CharacterBM;

typedef struct Character {
	const char *name;
	char       *nickname;
	
	// Bitmaps
	const char *top_down_bm_path;
	CharacterBM top_down;
} Character;

void load_characters(Character *characters);
Character* get_all_characters(void);
