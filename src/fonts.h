#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

int fonts_create_font(const char *name, const char *font_file);
ALLEGRO_FONT **fonts_get_fonts(const char *name);
ALLEGRO_FONT *fonts_get_font(const char *name, uint32_t size);
int fonts_delete_font(const char *name);
