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
#define ExtendsInstance // & The name of the Instance                                   
												char             *name;                                          \
												// & The class of the Instance, this is read-only and set in it's constructor.
												char             *class_name;                                    \
												// & The ID, maily for internal use.
												size_t            id;                                            \
												// & The Instance's parent.
												struct Instance  *parent;                                        \
												// & An array of the Instance's children. See `@constant MAX_CHILDREN`
												struct Instance  *children[MAX_CHILDREN];                        \
												// & The number of children in `@property Instance->children`
												uint32_t          children_count;                                \
												// & The draw function of the Instance, this can be NULL.
												method           (m_draw, void)(struct Instance *self);          \
												// & The debug draw (to draw debug info.) function of the Instance, this can be NULL.
												method           (m_debugDraw, void)(struct Instance *self);    \
												// & Mainly for internal use this is a `@class hashtable_t` used for `@function instance_find_first_child.`
												hashtable_t       children_fi;											             \
												// & Debugging flag, this will call `@property Instance->m_debugDraw` when it's render time.
												bool              enable_debugging


typedef struct Instance { ExtendsInstance; } Instance;

typedef struct Scene {
	ExtendsInstance;
	readOnly bool        is_active;
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
constructor function UIContainer* instance_new_ui_layout(void);

typedef struct UIRectangle {
	ExtendsInstance;
	TransformComponent transform;
	V4f                color;
} UIRectangle;
constructor function UIRectangle* instance_new_ui_rectangle(void);

// -- Instances


// ++ Basic Functions

// & Mainly for internal use. Used to construct an instance of `@param inst_size` bytes.
// => Puts `@property Instance->id` and generates `@property Instance->name` and `@property Instance->class_name` to `@param class_name`
Instance *instance_new_zeroed_any(size_t inst_size, const char *class_name);

// & Sets `@param subject`'s Parent (`@property Instance->parent`) to `@param new_parent`
// => Note: This also modifies internals such as `@property Instance->children_fi` and the other obvious ones.
// => See: `@class Instance`
int instance_set_parent(Instance* subject, Instance *new_parent);


// & Destroys `@param subject`
// => Note also destroys it's decendants and other stuff like `@property Instance->name` and `@property Instance->class_name`
// => See: `@class Instance`
int instance_destroy(Instance *subject);

// & Set's a name use this instead of directly modifying `@property Instance->Name`.
// => Note: This modifies other properties not just the name, it also modifies the `@property Instance->Parent` if it exists.
// => Note: `@param new_name` will be `@function strdup`'d.
int instance_set_name(Instance *subject, const char *new_name);

// & Find's a child that `@param object` has by it's name (`@param child_name`)
// => Note: If a child with that name does not exist, it will return `NULL`
// => Note: If there are multiple children with the same name it will return the one registered in `@property Instance->children_fi`. \\
// In other words the most recent `@class Instance` to has changed it's name or set child of `@param subject`. 
Instance *instance_find_first_child(Instance *subject, const char *child_name);

// -- Basic Functions

// ++ Services ++

#include <placeholder/instances/services/input_service.h>

// -- Services --


