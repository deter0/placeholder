
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro.h>

#include <hashtable/hashtable.h>
#include <assert.h>
#include <stdio.h>
#include <err.h>

#include "fonts.h"

#define DEBUG_FONTS

#define MAX_FONTS 1024
#define DEFAULT_FONT_SIZE 96
static hashtable_t Fonts;
static bool        FontsHTInit = false; // Fonts hashtable has been initted

static const uint32_t font_sizes[] = {
	8, 10, 11, 12, 13, 14, 16, 18,
	20, 24, 28, 30, 32, 36, 48, 52, 54,
	62, 74, 86, 92, 104, 112, 126
};
static const uint32_t num_font_sizes = sizeof(font_sizes)/sizeof(font_sizes[0]);

static const char *font_paths[] = {
	"../assets/fonts/Alegreya-Regular.ttf",
	"../assets/fonts/AlmendraDisplay-Regular.ttf"
};
static const char *font_names[] = {
	"Alegreya",
	"Almendra",
};
static ALLEGRO_FONT *fonts_fi[FONT_LAST_INVALID] = {0};

static_assert((sizeof(font_paths)/sizeof(font_paths[0])) == FONT_LAST_INVALID);
static_assert((sizeof(font_names)/sizeof(font_names[0])) == FONT_LAST_INVALID);

int fonts_load_defaults(void) {
	for (size_t i = 0; i < FONT_LAST_INVALID; i++) {
		const char *path = font_paths[i];
		const char *name = font_names[i];
		printf("[DEBUG] Loaded font `%s` (at `%s`)\n", name, path);
		fonts_create_font(name, path);
	}
}
const char *get_font(enum Font font) {
	assert(font < FONT_LAST_INVALID);
	return font_names[font];
}

int fonts_create_font(const char *name, const char *font_file) {
	if (FontsHTInit == false) {
		hashtable_init(&Fonts, sizeof(const char*), sizeof(ALLEGRO_FONT*), 128, 0);
		FontsHTInit = true;
	}

	ALLEGRO_FONT **loaded_fonts = (ALLEGRO_FONT**)calloc(sizeof(ALLEGRO_FONT*), num_font_sizes);
	uint32_t loaded_fonts_index = 0;
	
	for (size_t i = 0; i < num_font_sizes; i++) {
		ALLEGRO_FONT *font = al_load_ttf_font(font_file, font_sizes[i], 0);
		if (font == NULL) {
			warnx("Error loading font '%s' at `%s` of size %d\n", name, font_file, font_sizes[i]);
			return -1;
		}
		loaded_fonts[loaded_fonts_index++] = font;
	}
#ifdef DEBUG_FONTS
	printf("[DEBUG] Put Font: %s\n", name);
#endif
	hashtable_insert(&Fonts, hash_str(name), (const void*)name, (const void*)&loaded_fonts);
	return 0;
}

ALLEGRO_FONT **fonts_get_fonts(const char *name) { // TODO: Return a default font
	assert(FontsHTInit != false);

	ALLEGRO_FONT ***retrieved_fonts = (ALLEGRO_FONT**)hashtable_find(
		&Fonts,
		hash_str(name),
		name
	);
	if (retrieved_fonts == NULL) {
		warnx("Font '%s' not found.", name);
		return NULL;
	}
	
	if (*retrieved_fonts != NULL)
		return *retrieved_fonts;

	warnx("Something went wrong retrieving font: %s\n", name);
	return NULL;
}

ALLEGRO_FONT *fonts_get_font(const char *name, uint32_t size) {
	ALLEGRO_FONT **fonts = fonts_get_fonts(name);
	assert(fonts != NULL);
	
	uint32_t selected_size = UINT32_MAX;
	int32_t selected_size_index = -1;
	for (size_t i = 0; i < num_font_sizes; i++) {
		if (abs(font_sizes[i] - size) < abs(selected_size - size)) {
			selected_size = font_sizes[i];
			selected_size_index = i;
		}
	}
	if (selected_size_index != -1 && fonts[selected_size_index]) {
		return fonts[selected_size_index];
	} else {
		warnx("Something went wrong querying font size for font: %s size %d!\n", name, size);
		return NULL;
	}
}

int fonts_delete_font(const char *name) {
	assert(FontsHTInit != false);

	ALLEGRO_FONT ***fonts_ptr = fonts_get_fonts(name);
	if (fonts_ptr == NULL || *fonts_ptr == NULL) {
		warnx("Error deleting font most likely more information above.\n");
		return -1;
	}
	ALLEGRO_FONT **fonts = *fonts_ptr;
	
	for (size_t i = 0; i < num_font_sizes; i++) {
		al_destroy_font(fonts[i]);
	}
	free(fonts);
	hashtable_remove(&Fonts, hash_str(name), name);
	
	return 0;
}
