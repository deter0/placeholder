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

#include <la/la.h>

#include <placeholder/fonts.h>
#include <placeholder/ff.h>

// ++ Components

typedef enum ComponentType {
	componentDefault,
	componentTransform
} ComponentType;

typedef struct TransformComponent {
	V2f   position;
	V2f   scale;
	float rotation;
} TransformComponent;

// -- Components

// ++ Instances

// TODO(kay): Change class_name to an enum not a string
#define MAX_CHILDREN 128*2
#define ExtendsInstance char             *name;                                          \
												char             *class_name;                                    \
												size_t            id;                                            \
												struct Instance  *parent;                                        \
												struct Instance  *children[MAX_CHILDREN];                        \
												uint32_t          children_count;                                \
												method           (m_draw, void)(struct Instance *self);          \
												method           (m_debug_draw, void)(struct Instance *self);    \
												hashtable_t       children_fi;											             \
												bool              enable_debugging


typedef struct Instance { ExtendsInstance; } Instance;

typedef struct Scene {
	ExtendsInstance;
	bool        is_active; // READ-ONLY
} Scene;
constructor function Scene* instance_new_scene(void);

typedef struct ImageSprite {
	ExtendsInstance;
	char           *image_path;
	bool            image_is_loaded; // READ-ONLY
	ALLEGRO_BITMAP *bm;
	uint32_t        bm_w;
	uint32_t        bm_h;
	
	V2f             anchor_point; // 0 to 1
	TransformComponent transform;
} ImageSprite;
ImageSprite *instance_new_image_sprite(void);
function int instance_image_sprite_load_image(ImageSprite *img_sprite);

typedef struct TextLabel {
	ExtendsInstance;
	enum Font   font;
	uint32_t    font_size;
	char       *text;
	V4f         text_color;
	TransformComponent transform;
	
	method(m_setText, void)(Instance *self, const char *fmt, ...);
} TextLabel;
constructor function TextLabel* instance_new_text_label(void);

typedef enum {
	DirectionHorizontal,
	DirectionVertical
} Direction;

typedef struct UIContainer {
	ExtendsInstance;
	bool enabled;
	method(m_fadeOut, void)(Instance *self, float duration);
} UIContainer;
constructor function UIContainer* instance_new_ui_container(void);

typedef struct UILayout {
	ExtendsInstance;
	Direction layout_direction;
	float paddingL;	
	float paddingR;
	float paddingT;
	float paddingB;
	bool  active;
} UILayout;
UIContainer *instance_new_ui_layout(void);

typedef struct UIRectangle {
	ExtendsInstance;
	TransformComponent transform;
	V4f                color;
} UIRectangle;
UIRectangle *instance_new_ui_rectangle(void);

// -- Instances


// ++ Basic Functions

Instance *instance_new_zeroed_any(size_t inst_size, const char *class_name);
int instance_set_parent(Instance* subject, Instance *new_parent);
int instance_destroy(Instance *subject);
// `new_name` will be duplicated
int instance_set_name(Instance *subject, const char *new_name);

Instance *instance_find_first_child(Instance *subject, const char *child_name);

// -- Basic Functions

// ++ Services ++

#include <placeholder/instances/services/input_service.h>

// -- Services --


