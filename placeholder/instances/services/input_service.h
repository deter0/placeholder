#pragma once

#include <placeholder/instances/instances.h>
#include <placeholder/ff.h>

typedef enum e_keyState {
	KS_NOT_PRESSED   = 0b00000000,
	KS_JUST_PRESSED  = 0b00000010,
	KS_JUST_RELEASED = 0b00000100, 
	KS_HELD          = 0b00001010
} e_keyState;

// % (KeyDown, KeyPressed, KeyReleased, KeyUp) -> `@property InputService->m_getKeyState`

typedef struct InputService {
	Extends_Instance;
	// & `(e_keyState & KS_DOWN) == 0` -> Key is being held
	// => `(e_keyState & KS_RELEASED) == 0` -> Key was just released
	// => ... see `@enum e_keyState`
	public method(m_getKeyState, e_keyState)(struct InputService *self, char key_code);
	internal method(m_frameBegun, void)(struct InputService *self);
	
	internal readOnly ALLEGRO_KEYBOARD_STATE last_state;
	internal readOnly ALLEGRO_KEYBOARD_STATE now_state;
} InputService;
constructor function InputService* service_create_input_service(void);
