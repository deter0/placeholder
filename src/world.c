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
			fonts_get_font(get_font(promoted->font), promoted->font_size),
			al_map_rgb(255, 255, 255),
			promoted->transform.position[0], promoted->transform.position[1],
			0,
			"%s",
			promoted->text
		);
	} else if (strcmp(instance->class_name, "ImageSprite") == 0) {
		printf("[DEBUG] Draw Bitmap.\n");
		ImageSprite *promoted = (ImageSprite*)instance;
		if (promoted->image_is_loaded) {
			assert(promoted->bm != NULL && promoted->bm_h != 0 && promoted->bm_w != 0);

			al_draw_tinted_scaled_rotated_bitmap_region(
				promoted->bm, 													        // The bitmap
				0,                 											        // Source X
				0,                 											        // Source Y
				promoted->bm_w, 												        // Source W
				promoted->bm_h, 												        // Source H
				al_map_rgb(255, 255, 255),                      // Tint
				promoted->anchor_point[0],							        // Center X
				promoted->anchor_point[1],                      // Center Y
				promoted->transform.position[0],                // X
				promoted->transform.position[1],                // Y
				promoted->transform.scale[0] / promoted->bm_w,  // Scale X
				promoted->transform.scale[1] / promoted->bm_h,  // Scale Y
				0.0f,                                           // Rotation
				0                                               // Flags
			);
		} else {
			// Draw purple rect
		}
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
		// instance_set_name(other_font, "OtherFont");
		
		TextLabel *grace = instance_new_text_label();
		grace->transform.position[1] = 250;
		grace->text = strdup("hi grace!!!!!!!!!!!!!!!!!!!!!heeeeeeeeeeeeey");
		grace->font = Almendra;
		grace->font_size = 64;
		// instance_set_name(grace, "GRACE");
		
		ImageSprite *test = instance_new_image_sprite();
		test->image_path = strdup("../assets/gfx/cool.jpg");
		test->transform.position[0] = 250;
		test->transform.scale[0] = 500;
		test->transform.scale[1] = 500;
		instance_image_sprite_load_image(test);

		instance_set_parent(test, main_scene);
		instance_set_parent(hello_world, main_scene);
		instance_set_parent(other_font, main_scene);
		instance_set_parent(grace, main_scene);
	}
	assert(main_scene != NULL);
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_scene_recursive((Instance*)main_scene);
}

