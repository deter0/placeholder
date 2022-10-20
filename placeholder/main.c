#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LA_IMPLEMENTATION
#include <la/la.h>
#undef LA_IMPLEMENTATION


#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>


#include <placeholder/instances/instances.h>
#include <placeholder/characters.h>
#include <placeholder/world.h>
#include <placeholder/fonts.h>

// #define ALL_CHARACTERS_IMPL
// #include "all_characters.c"

#define HASHTABLE_IMPLEMENTATION
#include "hashtable/hashtable.h"

#include <err.h>

ALLEGRO_DISPLAY *init_allegro() {
    uint32_t version = al_get_allegro_version();
    int major = version >> 24;
    int minor = (version >> 16) & 255;
    int revision = (version >> 8) & 255;
    int release = version & 255;

    printf("[LOG] Library version: %d.%d.%d.%d\n", major, minor, revision, release);
    printf("[LOG] Header version: %s\n", ALLEGRO_VERSION_STR);

    if (!al_init()) {
        err(-1, "Allegro failed to init!\n");
    }   
    if (!al_init_font_addon()) {
        err(-1, "Failed to init allegro font addon!\n");
    }
    if (!al_init_ttf_addon()) {
        err(-1, "Failed to init allegro ttf addon!\n");
    }
    if (!al_init_image_addon()) {
        err(-1, "Failed to init allegro image addon!\n");
    }
    if (!al_init_primitives_addon()) {
        err(-1, "Failed to init allegro primatives addon!\n");
    }
    
    al_install_keyboard();
    
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY *display = al_create_display(1024, 1024);
    if (!display)
        err(-1, "Failed to create allegro display!\n");
    
    return display;
}

int hashtable_test() {
    // typedef const char* key_t;
    // typedef const char* value_t;
    
    hashtable_t table;
    hashtable_init(&table, sizeof(char*), 256, 0, 0);
    
    hashtable_insert(
        &table,
        0,
        (const void*)"hi!! one",
        "Goodbye"
    );
    hashtable_insert(
        &table,
        0,
        (const void*)"hi!! two",
        "Well whatever"
    );
    hashtable_insert(
        &table,
        0,
        (const void*)"jgiasjgiesjg",
        "gracie grace grace"
    );
    
    char* value_a = (char*)hashtable_find(&table, 0, (const void*)"hi!! one");
    printf("First item: %s\n", value_a); 
    assert(strcmp("Goodbye", value_a) == 0);
    char* value_b = (char*)hashtable_find(&table, 0, (const void*)"jgiasjgiesjg");
    printf("Second item: %s\n", value_b);
    assert(strcmp("gracie grace grace", value_b) == 0);
    
    hashtable_term(&table);
    
    return 0;
}
int instance_parenting_test(void) {
    Scene *scene = instance_new_scene();
    TextLabel *txt = instance_new_text_label();
    TextLabel *txt2 = instance_new_text_label();
    instance_set_name((Instance*)txt2, "PlayerName");
    
    printf("txt2 name: %s\n", txt2->name);
    
    instance_set_parent((Instance*)txt, (Instance*)scene);
    instance_set_parent((Instance*)txt2, (Instance*)scene);

    printf("\nscene->children[0]->name = `%s`\n", scene->children[0]->name);
    printf("scene->children[1]->name = `%s`\n\n", scene->children[1]->name);
    printf("txt->parent->name = `%s`\n", txt->parent->name);
    
    Instance *inst = instance_find_first_child((Instance*)scene, "TextLabel");
    assert(inst != NULL);
    printf("%p == %p\n", txt, inst);
    Instance *inst_2 = instance_find_first_child((Instance*)scene, "PlayerName");
    
    assert(inst_2 != NULL);
    printf("%p == %p\n", inst_2, inst_2);
    
    instance_set_name((Instance*)txt, "Hello World!");
    if (1) {
        instance_destroy((Instance*)scene);
        return 0;
    }
    inst = instance_find_first_child((Instance*)scene, "Hello World!");
    assert(inst != NULL);
    
    inst = instance_find_first_child((Instance*)scene, "Text Label");
    assert(inst == NULL);
    
    inst = instance_find_first_child((Instance*)scene, "TextLabel");
    assert(inst == NULL);
    
    return 0;
}
// #define INSTANCE_PARENT_TEST
#ifdef HSHTBL_TEST
int main() {
    hashtable_test();
}
#elif defined(INSTANCE_PARENT_TEST)
int main() {
    instance_parenting_test();
}
#else
int main() {
    ALLEGRO_DISPLAY *display = init_allegro();
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *frame_timer = al_create_timer(1.0 / 60.0); // TODO: Support multiple refresh rates
    
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(frame_timer));
    
    // fonts_create_font("Alegreya", "../assets/fonts/Alegreya-Regular.ttf");
    fonts_load_defaults();
    // ALLEGRO_FONT *font = al_load_ttf_font("../assets/fonts/Alegreya-Regular.ttf", 64, 0);
    // ALLEGRO_ASSERT(font != NULL);
    
    al_start_timer(frame_timer);
    
    // Load Default Characters
    
    load_characters(get_all_characters());
    
    World world = {
        .player_character = 0,
        .player_pos = {1.0f, 1.0f}
    };
    
    bool running = true;
    
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        
        switch (event.type) {
            case (ALLEGRO_EVENT_DISPLAY_CLOSE): {
                running = false;
            } break;
            case (ALLEGRO_EVENT_TIMER): {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                
                ALLEGRO_KEYBOARD_STATE state;
                al_get_keyboard_state(&state);
                
                if (al_key_down(&state, ALLEGRO_KEY_ESCAPE)) {
                    running = false;
                }
                
                int width = al_get_display_width(display);
                int height = al_get_display_height(display);
                world_draw(&world, &state, width, height);
                
                al_flip_display();
            } break;
            case (ALLEGRO_EVENT_DISPLAY_RESIZE): {
                al_acknowledge_resize(display);
            } break;
            default: {
                warnx("Unhandled event: %d\n", event.type);
            } break;
        }
    }    
    
    // al_destroy_font(font);
    al_destroy_timer(frame_timer);
    al_destroy_display(display);
    return 0;
}
#endif
