#pragma once

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
	assert_is(text_label, "TextLabel");

#ifdef TIME_DRAWS
	clock_t start, end;
	start = clock();
#endif
	
	al_draw_textf(
		fonts_get_font(get_font(text_label->font), text_label->font_size),
		al_map_rgba_f(
			text_label->text_color.x, // R
			text_label->text_color.y, // G
			text_label->text_color.z, // B
			text_label->text_color.w  // A
		),
		text_label->transform.position.x, text_label->transform.position.y,
		0,
		"%s",
		text_label->text
	);
	
#ifdef TIME_DRAWS
	end = clock();
	
	double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("al_draw_textf: %lf\n", cpu_time_used);
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
