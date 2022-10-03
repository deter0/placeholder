#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>
#include <err.h>

#include "characters.h"

void load_characters(Character *characters_to_load) {
	for (size_t i = 0; 1; i++) {
		if (characters_to_load[i].name == 0)
			break;
		
		Character *character = &characters_to_load[i];
		
		printf("[LOG] Loading bitmap: `%s`\n", character->top_down_bm_path);
		ALLEGRO_BITMAP *top_down_bm = al_load_bitmap(character->top_down_bm_path);
		character->top_down.bm = top_down_bm;

		if (character->top_down_bm_path == NULL) {
			warnx("Failed to load top down bitmap for character: %s\n", character->name);
		} else {
			character->top_down.bm_w = al_get_bitmap_width(top_down_bm);
			character->top_down.bm_h = al_get_bitmap_height(top_down_bm);
			
			character->top_down.bm_loaded = true;
		}
	}
}

void unload_characters(Character *characters_to_unload) {
	for (size_t i = 0; 1; i++) {
		if (characters_to_unload[i].name == 0)
			break;
		
		Character *character = &characters_to_unload[i];
		printf("[LOG] Unloading bitmap: `%s`\n", character->top_down_bm_path);
		al_destroy_bitmap(character->top_down.bm);
	}
}
