#pragma once

#include <placeholder/instances/instances.h>
#include <placeholder/ff.h>

typedef enum {
	KS_NOT_PRESSED   = 0b00000000,
	KS_JUST_PRESSED  = 0b00000010,
	KS_JUST_RELEASED = 0b00000100, 
	KS_HELD          = 0b00000010
} e_keyState;

typedef struct InputService {
	ExtendsInstance;
	// (KeyDown, KeyPressed, KeyReleased, KeyUp) -> InputService::m_getKeyState(char key_code) > e_keyState
	
	// `(e_keyState & KS_DOWN) == 0` -> Key is being held
	// => `(e_keyState & KS_RELEASED) == 0` -> Key was just released
	// => ... see `e_keyState`
	
	public method(m_getKeyState, e_keyState)(char key_code);
	
	internal method(m_frameBegun, void)(void);
} InputService;
constructor function InputService* service_create_input_service(void);
