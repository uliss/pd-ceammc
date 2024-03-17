#ifndef ceammc_rust_hw_h
#define ceammc_rust_hw_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>
#include <cstddef>
struct ceammc_rs_hw_gamepad;

enum class ceammc_rs_hw_gamepad_btn {
  South,
  East,
  North,
  West,
  C,
  Z,
  LeftTrigger,
  LeftTrigger2,
  RightTrigger,
  RightTrigger2,
  Select,
  Start,
  Mode,
  LeftThumb,
  RightThumb,
  DPadUp,
  DPadDown,
  DPadLeft,
  DPadRight,
  Unknown,
};

enum class ceammc_rs_hw_gamepad_event_axis {
  LeftStickX,
  LeftStickY,
  LeftZ,
  RightStickX,
  RightStickY,
  RightZ,
  DPadX,
  DPadY,
  Unknown,
};

enum class ceammc_rs_hw_gamepad_event_type {
  ButtonPressed,
  ButtonRepeated,
  ButtonReleased,
  ButtonChanged,
  AxisChanged,
  Connected,
  Disconnected,
  Dropped,
};

enum class ceammc_rs_hw_gamepad_powerstate {
  Unknown = 0,
  Wired,
  Discharging,
  Charging,
  Charged,
};

enum class ceammc_rs_hw_gamepad_rc {
  Ok = 0,
  InvalidHandle,
};

struct ceammc_rs_hw_gamepad_powerinfo {
  uint8_t data;
  ceammc_rs_hw_gamepad_powerstate state;
};

struct ceammc_rs_hw_gamepad_event {
  size_t id;
  ceammc_rs_hw_gamepad_btn button;
  ceammc_rs_hw_gamepad_event_type event;
  ceammc_rs_hw_gamepad_event_axis axis;
  float value;
};


extern "C" {

void ceammc_rs_hw_gamepad_free(ceammc_rs_hw_gamepad *gp);

ceammc_rs_hw_gamepad_rc ceammc_rs_hw_gamepad_list(ceammc_rs_hw_gamepad *gp,
                                                  void (*cb)(void *user_data, const char *name, const char *os_name, size_t id, uint16_t vid, uint16_t pid, bool is_connected, bool has_ff, const ceammc_rs_hw_gamepad_powerinfo *power),
                                                  void *user_data);

ceammc_rs_hw_gamepad *ceammc_rs_hw_gamepad_new(void *user_data, void (*err_cb)(void*, const char*));

ceammc_rs_hw_gamepad_rc ceammc_rs_hw_gamepad_runloop(ceammc_rs_hw_gamepad *gp,
                                                     void (*cb)(void *user_data, const ceammc_rs_hw_gamepad_event *event),
                                                     void *user_data);

} // extern "C"

#endif // ceammc_rust_hw_h
