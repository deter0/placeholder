#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <mathc/mathc.h>

#include <stdio.h>

#include "characters.h"
#include "instances/instances.h"
#include "fonts.h"
#include "world.h"

#define SCALE 4

Scene *main_scene = NULL;

static void try_draw_instance(Instance *instance) {
	if (strcmp(instance->class_name, "TextLabel") == 0) {
		TextLabel *promoted = (TextLabel*)instance;
		al_draw_textf(
			fonts_get_font(get_font(promoted->font), 32),
			al_map_rgb(255, 255, 255),
			promoted->transform.position[0], promoted->transform.position[1],
			0,
			"%s",
			promoted->text
		);
	}
}

static void draw_scene_recursive(Instance *scene) {
	try_draw_instance(scene);
	for (size_t i = 0; i < scene->children_count; i++) {
		draw_scene_recursive(scene->children[i]);
	}
}

void world_draw(World *world, ALLEGRO_KEYBOARD_STATE *state, int dsp_width, int dsp_height) {
	if (main_scene == NULL) {
		main_scene = instance_new_scene();

		TextLabel *hello_world = instance_new_text_label();
		hello_world->text = strdup("Hello, World!");
		
		TextLabel *other_font = instance_new_text_label();
		other_font->text = strdup("This is another cool font");
		other_font->font = Almendra;
		other_font->transform.position[0] = 500;
		instance_set_name(other_font, "OtherFont");

		instance_set_parent(hello_world, main_scene);
		instance_set_parent(other_font, main_scene);
	}
	assert(main_scene != NULL);
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_scene_recursive((Instance*)main_scene);
}

