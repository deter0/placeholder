#include <string.h>
#include <err.h>

#define LA_IMPLEMENTATION
#include <la/la.h>
#undef LA_IMPLEMENTATION

#include <hashtable/hashtable.h>

#include <placeholder/instances/instances.h>
#include <placeholder/ff.h>

#define init_children_fi(X) hashtable_init( \
	&((X)->children_fi),                      \
	sizeof(const void*),                      \
	sizeof(Instance**),                       \
	MAX_CHILDREN,                             \
	NULL                                      \
)

#include <placeholder/instances/instance_methods.c>

static size_t cur_id = 0;
static function size_t gen_instance_id(void) {
	cur_id += 1;
	return cur_id;
}

// ! Be sure to free
static function char* get_instance_debug_info(Instance *instance) {
	char *buffer = (char*)malloc(512);

	assert(buffer);
	memset(buffer, 0, 512);

	sprintf(buffer, "[Instance: %lu] (%p) %s '%s'", instance->id, instance, instance->class_name, instance->name);

	return buffer;
}

function int instance_destroy(Instance *subject) {
	assert(subject != NULL);

	for (size_t i = 0; i < subject->children_count; i++) {
		instance_destroy(subject->children[i]);
	}

	hashtable_term(&(subject->children_fi));
	if (strcmp(subject->class_name, "TextLabel") == 0) {
		TextLabel *promoted = (TextLabel*)subject;
		
		if (promoted->text != NULL)
			free(promoted->text);
	} else if (strcmp(subject->class_name, "ImageSprite") == 0) {
		ImageSprite *promoted = (ImageSprite*)subject;
		
		if (promoted->image_path != NULL)
			free(promoted->image_path);
		if (promoted->image_is_loaded)
			al_destroy_bitmap(promoted->bm);
	}
	
	free(subject->name);
	free(subject->class_name);
	free(subject);
	
	return 0;
}

function Instance* instance_find_first_child(Instance *subject, const char *child_name) {
	if (subject == NULL) {
		errx(1, "instance_find_first_child subject instance is NULL\n");
	}
	if (subject->children_count <= 0)
		return NULL;
	
	Instance **possible_find = hashtable_find(
			&subject->children_fi,
			hash_str(child_name),
			(const void*)child_name
		);
	if (possible_find != NULL && *possible_find != NULL) {
		if (strcmp((*possible_find)->name, child_name) == 0) {
			return *possible_find;
		}
	}
	
	for (size_t i = 0; i < subject->children_count; i++) {
		if (subject->children[i] != NULL && strcmp(subject->children[i]->name, child_name) == 0) {
			return subject->children[i];
		}
	}
	
	return NULL;
}

function int instance_set_name(Instance *subject, const char *new_name) {
	if (subject->parent != NULL) {
		Instance **possible_find = hashtable_find(
			&subject->parent->children_fi,
			hash_str(subject->name),
			(const void*)(subject->name)
		);
		if (possible_find != NULL && *possible_find != NULL) {
			if ((*possible_find)->id == subject->id) {
				// !!!!! TODO(kay): Fix this
				// printf("%s\n", subject->name); 
				// hashtable_remove(
				// 	&subject->parent->children_fi,
				// 	hash_str(subject->name),
				// 	(const void*)subject->name
				// );
				// hashtable_term(&subject->parent->children_fi);
				hashtable_insert(
					&subject->parent->children_fi,
					hash_str(new_name),
					strdup(new_name),
					&subject
				);
			}
		}
	}
	free(subject->name);
	subject->name = strdup(new_name);
	
	return 0;
}

// ! FIXME(kay): Instance with the same name crashes (FIXED)
function int instance_set_parent(Instance* subject, Instance *new_parent) {
	Instance *old_parent = subject->parent;
	if (!(subject != NULL)) {
		errx(1, "instance_set_parent subject instance is NULL\n");
	}

	if (old_parent) {
		size_t self_index = UINT32_MAX;
		for (size_t i = 0; i < old_parent->children_count; i++) {
			if (old_parent->children[i]->id == subject->id) {
				self_index = i;
				break;
			}
		}
		// ! Maybe some bugs here.. dont trust myself
		if (self_index != UINT32_MAX) {
			for (size_t i = self_index; i < old_parent->children_count; i++) {
				old_parent->children[i] = old_parent->children[i + 1];
			}
		}
		
		Instance **maybe_subject = hashtable_find(
			&old_parent->children_fi,
			hash_str(subject->name),
			subject->name
		);
		if (maybe_subject != NULL && *maybe_subject != NULL) {
			if ((*maybe_subject)->id == subject->id) {
				hashtable_remove(
					&old_parent->children_fi,
					hash_str(subject->name),
					subject->name
				);			
				goto next;
			}
		}
		
		int count = hashtable_count((const hashtable_t*)&old_parent->children_fi);
		const char** keys = (const char**)hashtable_keys(
			(const hashtable_t*)&old_parent->children_fi
		);
		Instance **items = (Instance**)hashtable_items(
			(const hashtable_t *)&old_parent->children_fi
		);
			
		for (size_t i = 0; i < count; i++) {
			if (items[i]->id == subject->id) {
				hashtable_remove(
					&old_parent->children_fi,
					hash_str(keys[i]),
					keys[i]
				);
				goto next;
			}
		}
	}
	next:
		if (new_parent != NULL) {
			if (new_parent->children_count >= MAX_CHILDREN) {
				// No need to free since we're exiting
				errx(1, "Instance exceeds max children. `%s`\n", get_instance_debug_info(new_parent));
			}
			new_parent->children[new_parent->children_count++] = subject;
			Instance **similar_exists = hashtable_find(
				&new_parent->children_fi,
				hash_str(subject->name),
				subject->name
			);
			if (!(similar_exists != NULL && (*similar_exists) != NULL)) {
				hashtable_insert(
					&new_parent->children_fi,
					hash_str(subject->name),
					strdup(subject->name), // ! MAYBE MEM LEAK?
					&subject
				);
			}
			subject->parent = new_parent;
		} else {
			subject->parent = NULL;
		}
	
	return 0;
}

function Scene* instance_new_scene(void) {
	Scene *scene = malloc(sizeof(*scene));
	if (!scene)
		return NULL;
	
	memset(scene, 0, sizeof(*scene));
	scene->class_name = strdup("Scene");
	scene->id 				= gen_instance_id();
	scene->name       = strdup("Scene");
	scene->parent 	  = NULL;
	
	// hashtable_init(
	// 	&scene->children_fi,
	// 	sizeof(char*),
	// 	sizeof(Instance*),
	// 	MAX_CHILDREN,
	// 	0
	// );
	init_children_fi(scene);
	
	scene->is_active = false;
	return scene;
}

function ImageSprite* instance_new_image_sprite(void) {
	ImageSprite *image_sprite = malloc(sizeof(*image_sprite));
	if (!image_sprite)
		return NULL;
	
	memset(image_sprite, 0, sizeof(*image_sprite));
	image_sprite->class_name  = strdup("ImageSprite");
	image_sprite->name        = strdup("ImageSprite");
	image_sprite->id   				= gen_instance_id();
	image_sprite->parent      = NULL;
	// Children should already be zeroed?
	// hashtable_init(&image_sprite->children_fi, sizeof(char*), sizeof(Instance*), MAX_CHILDREN, 0);
	init_children_fi(image_sprite);

	image_sprite->transform   = (TransformComponent){0};
	image_sprite->image_path  = NULL;
	image_sprite->image_is_loaded = false;
	image_sprite->bm = NULL;
	
	return image_sprite;
}

function int instance_image_sprite_load_image(ImageSprite *img_sprite) {
	if (img_sprite->image_is_loaded == false) {
		if (img_sprite->bm != NULL) {
			al_destroy_bitmap(img_sprite->bm);
		}
		assert(img_sprite->image_path);

		ALLEGRO_BITMAP *bm = al_load_bitmap(img_sprite->image_path);
		assert(bm != NULL);
		img_sprite->bm = bm;
		
		img_sprite->bm_w = al_get_bitmap_width(bm);
		img_sprite->bm_h = al_get_bitmap_height(bm);
		
		img_sprite->image_is_loaded = true;
		
		return 0;
	} else {
		char *info = get_instance_debug_info((Instance*)img_sprite);
		warnx("[WARNING]: called `instance_image_sprite_load_image` on image_sprite thats already loaded (%s).\n", info);
		free(info);

		img_sprite->bm = NULL;
		img_sprite->bm_w = 0;
		img_sprite->bm_h = 0;

		return -1;
	}
}

/* Legacy - Removed
FontObject *instance_new_font_object(void) {
	FontObject *font_object = malloc(sizeof(*font_object));
	if (!font_object)
		return NULL;
	memset(font_object, 0, sizeof(*font_object));
	font_object->class_name  = strdup("FontObject");
	font_object->name        = strdup("FontObject");
	font_object->id  	 	 	 	 = gen_instance_id();
	font_object->parent      = NULL;
	
	// hashtable_init(
	// 	&font_object->children_fi,
	// 	sizeof(char*),
	// 	sizeof(Instance*),
	// 	MAX_CHILDREN,
	// 	0
	// );
	init_children_fi(font_object);
	
	font_object->font_path = NULL;
	font_object->font_is_loaded = false;
	font_object->font_size = 32;
	return font_object;
}
*/
function TextLabel* instance_new_text_label(void) {
	TextLabel *text_label = malloc(sizeof(*text_label));
	if (!text_label)
		return 0;
	memset(text_label, 0, sizeof(*text_label));
	
	text_label->class_name = strdup("TextLabel");
	text_label->name       = strdup("TextLabel");
	text_label->id 				 = gen_instance_id();
	text_label->parent     = NULL;
	
	init_children_fi(text_label);
	
	text_label->font = Alegreya;
	text_label->font_size = 32;
	text_label->text_color = v4ff(1.f);
	text_label->text = strdup("Hello, World!");
	return text_label;
}

function UIRectangle* instance_new_ui_rectangle(void) {
	UIRectangle *rectangle = malloc(sizeof(*rectangle));
	if (!rectangle)
		return 0;
	memset(rectangle, 0, sizeof(*rectangle));
	
	rectangle->class_name = strdup("UIRectangle");
	rectangle->name       = strdup("UIRectangle");
	rectangle->id 				 = gen_instance_id();
	rectangle->parent     = NULL;
	
	init_children_fi(rectangle);
	
	rectangle->color = v4ff(1.f);
	rectangle->transform.scale = v2ff(100.f);

	return rectangle;
}

function Instance* instance_new_zeroed_any(size_t inst_size, const char *class_name) {
	assert(inst_size > sizeof(Instance));
	Instance *inst = (Instance*)malloc(inst_size);
	assert(inst != NULL);
	
	init_children_fi(inst);
	
	inst->name = strdup(class_name);
	inst->class_name = strdup(class_name);
	inst->id = gen_instance_id();

	return inst;
}

function UIContainer* instance_new_ui_container(void) {
	UIContainer *ui_container = (UIContainer*)instance_new_zeroed_any(sizeof(UIContainer), "UIContainer");

	ui_container->enabled = true;
	ui_container->m_fadeOut = m_fadeOut;
	
	return ui_container;
}
