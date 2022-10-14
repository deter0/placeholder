#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

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
			al_map_rgba_f(promoted->text_color.x, promoted->text_color.y, promoted->text_color.z, promoted->text_color.w),
			promoted->transform.position.x, promoted->transform.position.y,
			0,
			"%s",
			promoted->text
		);
	} else if (strcmp(instance->class_name, "ImageSprite") == 0) {
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
				promoted->anchor_point.x,							        // Center X
				promoted->anchor_point.y,                      // Center Y
				promoted->transform.position.x,                // X
				promoted->transform.position.y,                // Y
				promoted->transform.scale.x / promoted->bm_w,  // Scale X
				promoted->transform.scale.y / promoted->bm_h,  // Scale Y
				0.0f,                                           // Rotation
				0                                               // Flags
			);
		} else {
			al_draw_filled_rectangle(
				promoted->transform.position.x,
				promoted->transform.position.y,
				promoted->transform.position.x
				 + promoted->transform.scale.x,
				promoted->transform.position.y
				 + promoted->transform.scale.y,
				al_map_rgb(255, 0, 220)
			);
		}
	} else if (strcmp(instance->class_name, "Rectangle") == 0) {
		Rectangle *promoted = (Rectangle*)instance;
		al_draw_filled_rectangle(
			promoted->transform.position.x,
			promoted->transform.position.y,
			promoted->transform.position.x + promoted->transform.scale.x,
			promoted->transform.position.y + promoted->transform.scale.y,
			al_map_rgba_f(
				promoted->color.x,
				promoted->color.y,
				promoted->color.z,
				promoted->color.w
			)
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
		other_font->transform.position.x = 500;
		// instance_set_name(other_font, "OtherFont");
		
		TextLabel *grace = instance_new_text_label();
		grace->transform.position.x = 250;
		grace->text = strdup("hi grace!!!!!!!!!!!!!!!!!!!!!heeeeeeeeeeeeey");
		grace->font = Almendra;
		grace->font_size = 64;
		// instance_set_name(grace, "GRACE");
		
		ImageSprite *test = instance_new_image_sprite();
		test->image_path = strdup("../assets/gfx/default_character.png");
		test->transform.position.x = 250;
		test->transform.scale.x = 500;
		test->transform.scale.y = 500;
		instance_image_sprite_load_image(test);
		
		Rectangle *rect = instance_new_rectangle();

		instance_set_parent((Instance*)rect,        (Instance*)main_scene);
		instance_set_parent((Instance*)test,        (Instance*)main_scene);
		instance_set_parent((Instance*)hello_world, (Instance*)main_scene);
		instance_set_parent((Instance*)other_font,  (Instance*)main_scene);
		instance_set_parent((Instance*)grace,       (Instance*)main_scene);
	}
	assert(main_scene != NULL);
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_scene_recursive((Instance*)main_scene);
}

