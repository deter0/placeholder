#include <placeholder/ff.h>
#include <placeholder/instances/instances.h>

// #define TIME_DRAWS

#ifdef TIME_DRAWS
#include <time.h>
#endif

#define assert_is(Y, X) assert(Y != NULL);                      \
												assert(Y->class_name != NULL);          \
												assert(strcmp(Y->class_name, X) == 0)   

private function void m_fadeOut(Instance *self, float duration) {
	printf("Method called!");
}

private function void m_setText(Instance *self, const char *fmt, ...) {
	assert_is(self, "TextLabel");
	TextLabel *promoted = (TextLabel*)self;
	
	// TODO(deter): Implement
	// https://stackoverflow.com/questions/3530771/passing-variable-arguments-to-another-function-that-accepts-a-variable-argument
	
	// promoted->text
	assert(false && "Not Implemented");
}

// ++ Draw Functions ++

private function void draw_text_label(TextLabel *text_label) {
#ifdef TIME_DRAWS
	clock_t start, end;
	start = clock();
#endif

	assert_is(text_label, "TextLabel");
	
	al_draw_textf(
		fonts_get_font(get_font(text_label->font), text_label->font_size),
		al_map_rgba_f(
			text_label->text_color.x, // R
			text_label->text_color.y, // G
			text_label->text_color.z, // B
			text_label->text_color.w  // A
		),
		text_label->transform.position.x, text_label->transform.position.y - (text_label->font_size * 0.25f - 2),
		0,
		"%s",
		text_label->text
	);
	
#ifdef TIME_DRAWS
	end = clock();
	
	double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("draw text label: %lf\n", cpu_time_used);
#endif
}

private function void draw_image_sprite(ImageSprite *image_sprite) {
#ifdef TIME_DRAWS
	clock_t start, end;
	start = clock();
#endif

	assert_is(image_sprite, "ImageSprite");

	if (image_sprite->image_is_loaded) {
		assert(image_sprite->bm != NULL && image_sprite->image_w != 0 && image_sprite->image_h != 0);

		al_draw_tinted_scaled_rotated_bitmap_region(
			image_sprite->bm, 													             // The bitmap
			0,                 											                 // Source X
			0,                 											                 // Source Y
			image_sprite->image_w, 											           	 // Source W
			image_sprite->image_h, 											           	 // Source H
			al_map_rgb(255, 255, 255),                               // Tint
			image_sprite->anchor_point.x,							               // Center X
			image_sprite->anchor_point.y,                            // Center Y
			image_sprite->transform.position.x,                      // X
			image_sprite->transform.position.y,                      // Y
			image_sprite->transform.scale.x / image_sprite->image_w, // Scale X
			image_sprite->transform.scale.y / image_sprite->image_h, // Scale Y
			0.0f,                                                    // Rotation
			0                                                        // Flags
		);
	} else {
		al_draw_filled_rectangle(
			image_sprite->transform.position.x,
			image_sprite->transform.position.y,
			image_sprite->transform.position.x
				+ image_sprite->transform.scale.x,
			image_sprite->transform.position.y
				+ image_sprite->transform.scale.y,
			al_map_rgb(255, 0, 220)
		);
	}

#ifdef TIME_DRAWS
	end = clock();
	
	double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("draw image_sprite: %lf\n", cpu_time_used);
#endif
}


private function void debug_draw_text_label(TextLabel *text_label) {
	assert_is(text_label, "TextLabel");
	
	int x = 0, y = 0, width = 0, height = 0;
	al_get_text_dimensions(
		fonts_get_font(get_font(text_label->font), text_label->font_size),
		text_label->text,
		&x,     &y,
		&width, &height
	);
	
	al_draw_rectangle(x, y, x + width, y + height, al_map_rgb(255, 0, 0), 1.0f);
}

// -- Draw Functions --

private function void m_draw(Instance *self) {
	
}

private function int m_loadImage(ImageSprite *img_sprite) {
	if (img_sprite->image_is_loaded == false) {
		if (img_sprite->bm != NULL) {
			al_destroy_bitmap(img_sprite->bm);
		}
		
		if (!file_exists(img_sprite->image_path)) {
			char *debug_info = instance_get_debug_info((Instance*)img_sprite);
			p_error("File for image, `%s`, doesn't exist (%s)\n", img_sprite->image_path, debug_info);
			free(debug_info);
			return 1;
		}
		ALLEGRO_BITMAP *bm = al_load_bitmap(img_sprite->image_path);
		assert(bm);
		
		img_sprite->bm = bm;
		
		img_sprite->image_w = al_get_bitmap_width(bm);
		img_sprite->image_h = al_get_bitmap_height(bm);
		
		img_sprite->image_is_loaded = true;
		
		return 0;
	} else {
		char *info = instance_get_debug_info((Instance*)img_sprite);
		p_warn("Called `instance_image_sprite_load_image` on image_sprite thats already loaded (%s).\n", info);
		
		free(info);

		img_sprite->bm = NULL;
		img_sprite->image_w = 0;
		img_sprite->image_h = 0;
		img_sprite->image_is_loaded = false;

		return -1;
	}
}
