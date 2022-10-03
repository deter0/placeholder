#pragma once

#include "characters.h"

static struct Character AllCharacters[] = {
		{
				.name = "default_player",
				.nickname = NULL,
				
				.top_down_bm_path = "../assets/gfx/default_character.png",
				.top_down = {0},
		},
		{.name = 0}
};

Character* get_all_characters(void) {
	return &AllCharacters;
}
