#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <hashtable/hashtable.h>
#include <mathc/mathc.h>

// ++ Components

typedef enum ComponentType {
	component_default
} ComponentType;

typedef struct TransformComponent {
	mfloat_t Position[VEC3_SIZE];
	mfloat_t Scale[VEC3_SIZE];
	mfloat_t Rotation;
} TransformComponent;

// -- Components

// ++ Instances

// TODO(kay): Change class_name to an enum not a string
#define MAX_CHILDREN 256
#define ExtendsInstance char             *name;                   \
												char             *class_name;             \
												size_t            id;                     \
												struct Instance  *parent;                 \
												struct Instance  *children[MAX_CHILDREN]; \
												uint32_t          children_count;         \
												hashtable_t       children_fi											


typedef struct Instance { ExtendsInstance; } Instance;

typedef struct Scene {
	ExtendsInstance;
	bool        is_active; // READ-ONLY
} Scene;
Scene *instance_new_scene(void);

typedef struct ImageSprite {
	ExtendsInstance;
	const char *image_path;
	bool        image_is_loaded; // READ-ONLY
	
	TransformComponent transform;
} ImageSprite;
ImageSprite *instance_new_image_sprite(void);

typedef struct FontObject {
	ExtendsInstance;
	char       *font_path;
	bool        font_is_loaded; // READ-ONLY
	
	uint32_t    font_size;
} FontObject;
FontObject *instance_new_font_object(void);

typedef struct TextLabel {
	ExtendsInstance;
	FontObject  font;
	char       *text; // TODO: Text Color
} TextLabel;
TextLabel *instance_new_text_label(void);

// -- Instances


// ++ Basic Functions

int instance_set_parent(Instance* subject, Instance *new_parent);
int instance_destroy(Instance *subject);
// `new_name` will be duplicated
int instance_set_name(Instance *subject, const char *new_name);

Instance *instance_find_first_child(Instance *subject, const char *child_name);

// -- Basic Functions
