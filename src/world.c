#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_primitives.h>
#include <mathc/mathc.h>

#include <stdio.h>

#include "characters.h"
#include "instances/instances.h"
#include "world.h"

#define SCALE 4

Scene *main_scene = NULL;

void world_draw(World *world, ALLEGRO_KEYBOARD_STATE *state, int dsp_width, int dsp_height) {
	if (main_scene == NULL) {
		main_scene = instance_new_scene();
	}
	assert(main_scene != NULL);

	al_clear_to_color(al_map_rgb(195, 204, 184));

}

