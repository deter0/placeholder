#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

enum Font {
	Alegreya = 0,
	Almendra,
	FONT_LAST_INVALID,
};

int fonts_load_defaults(void);
int fonts_create_font(const char *name, const char *font_file);
ALLEGRO_FONT **fonts_get_fonts(const char *name);
ALLEGRO_FONT *fonts_get_font(const char *name, uint32_t size);
int fonts_delete_font(const char *name);
const char *get_font(enum Font font);
