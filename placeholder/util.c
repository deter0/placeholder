#include <placeholder/instances/instances.h>
#include <placeholder/instances/services/input_service.h>

bool ks_just_pressed(e_keyState ks) {
  return (ks & KS_JUST_PRESSED) != 0;
}

bool ks_just_released(e_keyState ks) {
  return (ks & KS_JUST_RELEASED) != 0;
}

bool ks_is_down(e_keyState ks) {
  return (ks & (KS_JUST_PRESSED | KS_HELD)) != 0;
}
bool ks_is_not_pressed(e_keyState ks) {
  return (ks & KS_NOT_PRESSED) != 0;
}

bool file_exists(const char *file_path) {
  if (access(file_path, F_OK) == 0) {
      return true;
  }

  return false;
}
