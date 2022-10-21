#include <placeholder/instances/instances.h>
#include <placeholder/instances/services/input_service.h>
#include <placeholder/ff.h>

private function m_getKeyState(InputService *self, char key_code) {
	if (al_key_down(&self->now_state, (int)key_code) == 1) {
		if (al_key_down(&self->last_state, (int)key_code) == 1) {
			return KS_HELD;
		}
		return KS_JUST_PRESSED;
	} else if (al_key_down(&self->last_state, (int)key_code) == 1) {
		return KS_JUST_RELEASED;
	}
	return KS_NOT_PRESSED;
}

private function m_frameBegun(InputService *self) {
	self->last_state = self->now_state;
	al_get_keyboard_state(&self->now_state);
}

constructor function InputService* service_create_input_service(void) {
	InputService *input_service  = (InputService*)instance_new_zeroed_any(sizeof(*input_service), "InputService");
	input_service->m_frameBegun  = m_frameBegun;
	input_service->m_getKeyState = m_getKeyState;
	
	return input_service;
}
