// Placeholder UI - Plui

#include "../instances/instances.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct PluiRoot {
	ExtendsInstance;
	TransformComponent transform;
} PluiRoot;

void plui_set_scene(Scene *scene);
void plui_begin_topbar(void);
void plui_set_topbar_items(const char**);
void plui_end(void);
