#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <stdio.h>

#include <placeholder/instances/instances.h>
#include <placeholder/fonts.h>
#include <placeholder/world.h>
#include <placeholder/ff.h>
#include <placeholder/util.h>
#include <placeholder/plog.h>

#define LA_IMPLEMENTATION
#include <la/la.h>

// #include "plui/plui.h"

#define SCALE 4

Scene *main_scene = NULL;

static void try_draw_instance(Instance *instance) {
	/*
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
				promoted->bm, 													       // The bitmap
				0,                 											       // Source X
				0,                 											       // Source Y
				promoted->bm_w, 												       // Source W
				promoted->bm_h, 												       // Source H
				al_map_rgb(255, 255, 255),                     // Tint
				promoted->anchor_point.x,							         // Center X
				promoted->anchor_point.y,                      // Center Y
				promoted->transform.position.x,                // X
				promoted->transform.position.y,                // Y
				promoted->transform.scale.x / promoted->bm_w,  // Scale X
				promoted->transform.scale.y / promoted->bm_h,  // Scale Y
				0.0f,                                          // Rotation
				0                                              // Flags
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
	} else if (strcmp(instance->class_name, "UIRectangle") == 0) {
		UIRectangle *promoted = (UIRectangle*)instance;
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
	*/
	if (instance->m_draw != NULL) {
		instance->m_draw(instance);
	}

	if (instance->enable_debugging && instance->m_debugDraw != NULL) {
		instance->m_debugDraw(instance);
	}
	
	// plui_begin_topbar();
}

static void draw_scene_recursive(Instance *scene) {
	try_draw_instance(scene);
	for (size_t i = 0; i < scene->children_count; i++) {
		draw_scene_recursive(scene->children[i]);
	}
}

ImageSprite *image;
void world_draw(World *world, ALLEGRO_KEYBOARD_STATE *state, int dsp_width, int dsp_height) {
	if (main_scene == NULL) {
		main_scene = instance_new_scene();

		TextLabel *hello_world = instance_new_text_label();
		hello_world->text = strdup("Hello, World!");
		hello_world->font = Almendra;
		hello_world->font_size = 54;
		
		image = instance_new_image_sprite();
		assert(image);
		image->image_path = "../assets/gfx/default_character.png";
		//image->m_loadImage(image);
		
		instance_set_parent((Instance*)image,       (Instance*)main_scene);
		instance_set_parent((Instance*)hello_world, (Instance*)main_scene);
	}
	assert(main_scene != NULL);
	main_scene->s_inputService->m_frameBegun(main_scene->s_inputService);

	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_scene_recursive((Instance*)main_scene);
	
	e_keyState w_state = main_scene->s_inputService->m_getKeyState(main_scene->s_inputService, ALLEGRO_KEY_W);
	e_keyState a_state = main_scene->s_inputService->m_getKeyState(main_scene->s_inputService, ALLEGRO_KEY_A);
	e_keyState s_state = main_scene->s_inputService->m_getKeyState(main_scene->s_inputService, ALLEGRO_KEY_S);
	e_keyState d_state = main_scene->s_inputService->m_getKeyState(main_scene->s_inputService, ALLEGRO_KEY_D);
	
	V2f direction = {0}; 
	
	if ((d_state & (KS_HELD | KS_JUST_PRESSED)) != 0) {
		direction.x += 1;
	}
	if ((a_state & (KS_HELD | KS_JUST_PRESSED)) != 0) {
		direction.x -= 1;
	}
	if ((w_state & (KS_HELD | KS_JUST_PRESSED)) != 0) {
		direction.y += 1;
	}
	if ((s_state & (KS_HELD | KS_JUST_PRESSED)) != 0) {
		direction.y -= 1;
	}
	
	v2f_div(*(&direction), v2ff(v2f_sqrlen(direction)));
	p_debugLog("Direction, %f, %f :: %f\n", direction.x, direction.y, v2f_sqrlen(direction));
	
	image->transform.position.x += direction.x;
	image->transform.position.y += direction.y;
	
	p_debugLog("%f, %f\n", image->transform.position.x, image->transform.position.y);
	
	// image->transform.position = new_pos;
}

