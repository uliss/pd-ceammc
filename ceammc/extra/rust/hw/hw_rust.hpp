#ifndef ceammc_rust_hw_h
#define ceammc_rust_hw_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>
#include <cstddef>


constexpr static const int32_t ceammc_JOB_ERROR = -1;

enum class ceammc_hw_gamepad_btn {
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

enum class ceammc_hw_gamepad_event_axis {
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

enum class ceammc_hw_gamepad_event_type {
    ButtonPressed,
    ButtonRepeated,
    ButtonReleased,
    ButtonChanged,
    AxisChanged,
    Connected,
    Disconnected,
    Dropped,
};

enum class ceammc_hw_gamepad_powerstate {
    Unknown = 0,
    Wired,
    Discharging,
    Charging,
    Charged,
};

enum class ceammc_hw_gpio_mode {
    Output,
    Input,
};

enum class ceammc_hw_printer_state {
    READY,
    PAUSED,
    PRINTING,
    UNKNOWN,
};

/// gamepad opaque type
struct ceammc_hw_gamepad;

/// gpio opaque type
struct ceammc_hw_gpio;

struct ceammc_gamepad_err_cb {
    /// pointer to user data
    void *user;
    void (*cb)(void*, const char*);
};

struct ceammc_hw_gamepad_event {
    /// gamepad id
    size_t id;
    ceammc_hw_gamepad_btn button;
    ceammc_hw_gamepad_event_type event;
    ceammc_hw_gamepad_event_axis axis;
    float value;
};

struct ceammc_gamepad_event_cb {
    /// pointer to user data
    void *user;
    void (*cb)(void *user, const ceammc_hw_gamepad_event *event);
};

struct ceammc_hw_gamepad_powerinfo {
    ceammc_hw_gamepad_powerstate state;
    uint8_t data;
};

struct ceammc_gamepad_dev_info {
    /// gamepad name
    const char *name;
    /// gamepad os_name
    const char *os_name;
    /// gamepad id
    size_t id;
    /// gamepad power info
    ceammc_hw_gamepad_powerinfo power;
    /// gamepad vendor id
    uint16_t vid;
    /// gamepad product id
    uint16_t pid;
    /// gamepad is connected
    bool is_connected;
    /// gamepad has force feedback capability
    bool has_ff;
};

struct ceammc_gamepad_listdev_cb {
    void *user;
    void (*cb)(void *user, const ceammc_gamepad_dev_info *info);
};

struct ceammc_hw_notify_cb {
    /// dispatcher ID
    size_t id;
    /// dispatcher callback (not NULL!)
    void (*f)(size_t id);
};

struct ceammc_hw_printer_info {
    const char *name;
    const char *system_name;
    const char *driver_name;
    const char *uri;
    const char *location;
    bool is_default;
    bool is_shared;
    ceammc_hw_printer_state state;
};

struct ceammc_hw_printer_info_cb {
    void *user;
    void (*cb)(void *user, const ceammc_hw_printer_info *info);
};

/// error callback
struct ceammc_hw_error_cb {
    /// pointer to user data (can be NULL)
    void *user;
    /// can be NULL
    void (*cb)(void*, const char*);
};

/// pin value callback
struct ceammc_hw_gpio_pin_cb {
    /// pointer to user data (can be NULL)
    void *user;
    /// can not be NULL
    void (*cb)(void*, uint8_t, bool);
};

struct ceammc_hw_print_options {
    bool landscape;
};


extern "C" {

/// free gamepad
/// @param gp - pointer to gp
void ceammc_hw_gamepad_free(ceammc_hw_gamepad *gp);

/// list connected devices
bool ceammc_hw_gamepad_list_devices(ceammc_hw_gamepad *gp);

/// create new gamepad
/// @param on_err - error callback
/// @param on_event - gamepad event callback
/// @param on_devinfo - gamepad list connected devices callback
/// @param cb_notify - dispatcher notification
/// @return pointer to new gamepad or NULL on error
ceammc_hw_gamepad *ceammc_hw_gamepad_new(ceammc_gamepad_err_cb on_err,
                                         ceammc_gamepad_event_cb on_event,
                                         ceammc_gamepad_listdev_cb on_devinfo,
                                         ceammc_hw_notify_cb cb_notify,
                                         uint64_t poll_time_ms);

/// process events
/// @param gp - pointer to gp
void ceammc_hw_gamepad_process_events(ceammc_hw_gamepad *gp);

/// get printers info via specified callback
/// @param info_cb - called on every found printer
/// @return number of printers found
size_t ceammc_hw_get_printers(ceammc_hw_printer_info_cb info_cb);

/// clear software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
bool ceammc_hw_gpio_clear_pwm(ceammc_hw_gpio *gp, uint8_t pin);

/// create new gpio
/// @param gpio - pointer to gpio struct
void ceammc_hw_gpio_free(ceammc_hw_gpio *gpio);

/// create new gpio
/// @param on_err - on error callback for output error messages
/// @param notify - notification update callback
/// @param on_pin_value - called on pin value output
ceammc_hw_gpio *ceammc_hw_gpio_new(ceammc_hw_error_cb on_err,
                                   ceammc_hw_notify_cb notify,
                                   ceammc_hw_gpio_pin_cb on_pin);

/// process events
/// @param gp - pointer to gpio struct
void ceammc_hw_gpio_process_events(ceammc_hw_gpio *gp);

/// read pin request
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
bool ceammc_hw_gpio_read_pin(ceammc_hw_gpio *gp, uint8_t pin);

/// reset pin to initial state
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
bool ceammc_hw_gpio_reset_pin(ceammc_hw_gpio *gp, uint8_t pin);

/// set pin mode
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param mode - pin mode
bool ceammc_hw_gpio_set_mode(ceammc_hw_gpio *gp, uint8_t pin, ceammc_hw_gpio_mode mode);

/// set software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param period - period in msec
/// @param width - duty_cycle width in msec
bool ceammc_hw_gpio_set_pwm(ceammc_hw_gpio *gp, uint8_t pin, double period, double width);

/// set software pwm freq on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param freq - freq in Herz
/// @param duty_cycle - duty cycle in % [0-100] range
bool ceammc_hw_gpio_set_pwm_freq(ceammc_hw_gpio *gp, uint8_t pin, double freq, double duty_cycle);

/// toggle pin level
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
bool ceammc_hw_gpio_toggle_pin(ceammc_hw_gpio *gp, uint8_t pin);

/// write pin value
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param level - pin level (=0: low, >0: high)
bool ceammc_hw_gpio_write_pin(ceammc_hw_gpio *gp, uint8_t pin, bool level);

int32_t ceammc_hw_print_file(const char *printer,
                             const char *path,
                             const ceammc_hw_print_options *opts,
                             ceammc_hw_error_cb on_err,
                             ceammc_hw_error_cb on_debug);

bool ceammc_hw_printer_default(ceammc_hw_printer_info_cb info_cb);

} // extern "C"

#endif // ceammc_rust_hw_h
