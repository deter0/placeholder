// Placeholder UI - Plui

#include "../instances/instances.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define LA_IMPLEMENTATION
#include <la/la.h>

#include "plui.h"

static Scene    *plui_output = NULL;
static PluiRoot *root = NULL;

static void init_root() {
	PluiRoot *n_root = (PluiRoot*)instance_new_zeroed_any(sizeof(PluiRoot));
	assert(n_root);
	
	n_root->class_name = strdup("PluiRoot");
	n_root->name       = strdup("PluiRoot");

	root = n_root;
}

void plui_set_scene(Scene *scene) {
	if (!root)
		init_root();
	
	instance_set_parent(root, scene);
	plui_output = scene;
}

static bool topbar_frame = false;
static const char **topbar_items = NULL;
void plui_begin_topbar(void) {
	if (!root)
		init_root();
	
	UIRectangle *top_bar = instance_find_first_child(root, "TopBar");
	if (!top_bar) {
		top_bar = instance_new_ui_rectangle();
		top_bar->transform.scale.x = 1024.f;
		top_bar->transform.scale.y = 0.f;
		top_bar->color = v4f(0.2f, 0.2f, 0.2f, 1.f);

		instance_set_name(top_bar, "TopBar");
		instance_set_parent(top_bar, root);
	}
	topbar_frame = true;
	
	top_bar->transform.scale.y = 24.f;
}

void plui_set_topbar_items(const char **items) {
	topbar_items = items;
}

void plui_end() {
	if (!topbar_frame) {
		UIRectangle *top_bar = (UIRectangle*)instance_find_first_child(root, "TopBar");
		if (top_bar) {
			top_bar->transform.scale.y = 0.f;
		}
	}
}
