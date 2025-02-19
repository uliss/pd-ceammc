#ifndef ceammc_rust_hw_h
#define ceammc_rust_hw_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>
#include <cstddef>


constexpr static const int32_t ceammc_HW_MAX7219_ADDRESS_ALL = -1;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DECODE_MODE = 9;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DIGIT_0 = 1;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DIGIT_1 = 2;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DIGIT_2 = 3;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DIGIT_3 = 4;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DIGIT_4 = 5;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DIGIT_5 = 6;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DIGIT_6 = 7;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DIGIT_7 = 8;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_DISPLAY_TEST = 15;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_INTENSITY = 10;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_SCAN_LIMIT = 11;

constexpr static const uint8_t ceammc_HW_MAX7219_REG_SHUTDOWN = 12;

constexpr static const uint16_t ceammc_HW_SR04_DEF_POLL_INTERVAL = 20;

constexpr static const uint16_t ceammc_HW_SR04_MAX_POLL_INTERVAL = 1000;

constexpr static const uint16_t ceammc_HW_SR04_MIN_POLL_INTERVAL = 10;

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

enum class ceammc_hw_gpio_bias {
    None,
    PullUp,
    PullDown,
};

enum class ceammc_hw_gpio_mode {
    Output,
    Input,
};

enum class ceammc_hw_gpio_trigger {
    None,
    RisingEdge,
    FallingEdge,
    Both,
};

enum class ceammc_hw_max7219_string_align {
    Left,
    Right,
    Center,
};

enum class ceammc_hw_printer_state {
    READY,
    PAUSED,
    PRINTING,
    UNKNOWN,
};

enum class ceammc_hw_spi_bus {
    SPI0,
    SPI1,
    SPI2,
    SPI3,
    SPI4,
    SPI5,
    SPI6,
};

enum class ceammc_hw_spi_cs {
    CS0,
    CS1,
    CS2,
    CS3,
};

/// gamepad opaque type
struct ceammc_hw_gamepad;

struct ceammc_hw_gpio;

/// gpio opaque type
struct ceammc_hw_gpio;

struct ceammc_hw_gpio_dht11;

struct ceammc_hw_gpio_sr04;

struct ceammc_hw_i2c;

struct ceammc_hw_lcd1602;

struct ceammc_hw_max7219;

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
struct ceammc_hw_msg_cb {
    /// pointer to user data (can be NULL)
    void *user;
    /// can be NULL
    void (*cb)(void*, const char*);
};

/// error callback
struct ceammc_hw_dht11_cb {
    /// pointer to user data (can be NULL)
    void *user;
    /// not NULL!
    void (*cb)(void*, double temp, double hum);
};

/// pin value callback
struct ceammc_hw_gpio_pin_cb {
    /// pointer to user data (can be NULL)
    void *user;
    /// can not be NULL
    void (*cb)(void*, uint8_t, bool);
};

/// pin list callback
struct ceammc_hw_gpio_pin_list_cb {
    /// pointer to user data (can be NULL)
    void *user;
    /// can not be NULL
    void (*cb)(void*, const uint8_t*, size_t);
};

struct ceammc_hw_gpio_poll_cb {
    size_t id;
    void (*cb)(size_t id, int data);
};

/// error callback
struct ceammc_hw_sr04_cb {
    /// pointer to user data (can be NULL)
    void *user;
    /// not NULL!
    void (*cb)(void*, float distance_cm, bool is_inf);
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

/// clear pin event polling
/// @param gpio - pointer to gpio struct
/// @param pin - pin BCM number
bool ceammc_hw_gpio_clear_poll(ceammc_hw_gpio *gp, uint8_t pin);

/// clear software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
bool ceammc_hw_gpio_clear_pwm(ceammc_hw_gpio *gp, uint8_t pin);

/// free dht11 sensor
/// @param dht - pointer to DHT11 struct
void ceammc_hw_gpio_dht11_free(ceammc_hw_gpio_dht11 *dht);

/// singe measure request
/// @param dht - pointer to DHT11 struct
bool ceammc_hw_gpio_dht11_measure(const ceammc_hw_gpio_dht11 *dht);

/// create new DHT11 sensor struct
/// @param pin - connected GPIO pin
/// @param notify - data check callback
/// @param on_err - error message callback
/// @param on_data - data callback
ceammc_hw_gpio_dht11 *ceammc_hw_gpio_dht11_new(uint8_t pin,
                                               ceammc_hw_notify_cb notify,
                                               ceammc_hw_msg_cb on_err,
                                               ceammc_hw_dht11_cb on_data);

/// poll request
/// @param dht - pointer to DHT11 struct
/// @param state - poll state
bool ceammc_hw_gpio_dht11_poll(const ceammc_hw_gpio_dht11 *dht, bool state);

/// check measure data
/// @param dht - pointer to DHT11 struct
bool ceammc_hw_gpio_dht11_process(const ceammc_hw_gpio_dht11 *dht);

/// create new gpio
/// @param gpio - pointer to gpio struct
void ceammc_hw_gpio_free(ceammc_hw_gpio *gpio);

/// list used pins
/// @param gpio - pointer to gpio struct
bool ceammc_hw_gpio_list_pins(ceammc_hw_gpio *gp);

/// create new gpio
/// @param on_err - on error callback for output error messages
/// @param on_dbg - on error callback for output error messages
/// @param notify - notification update callback
/// @param on_pin - called on pin value output
/// @param on_pin_list - called on pin list reply
/// @param on_pin_poll - called on pin poll event
ceammc_hw_gpio *ceammc_hw_gpio_new(ceammc_hw_msg_cb on_err,
                                   ceammc_hw_msg_cb on_dbg,
                                   ceammc_hw_notify_cb notify,
                                   ceammc_hw_gpio_pin_cb on_pin,
                                   ceammc_hw_gpio_pin_list_cb on_pin_list,
                                   ceammc_hw_gpio_poll_cb on_pin_poll);

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

/// set pin bias
/// @param gpio - pointer to gpio struct
/// @param pin - pin BCM number
bool ceammc_hw_gpio_set_bias(ceammc_hw_gpio *gp, uint8_t pin, ceammc_hw_gpio_bias bias);

/// set pin mode
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param mode - pin mode
bool ceammc_hw_gpio_set_mode(ceammc_hw_gpio *gp, uint8_t pin, ceammc_hw_gpio_mode mode);

/// poll pin events
/// @param gpio - pointer to gpio struct
/// @param pin - pin BCM number
/// @param trigger - event trigger
/// @param debounce_ms - debounce time in ms
bool ceammc_hw_gpio_set_poll(ceammc_hw_gpio *gp,
                             uint8_t pin,
                             ceammc_hw_gpio_trigger trigger,
                             double debounce_ms);

/// set software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param period - period in msec
/// @param width - duty_cycle width in msec
bool ceammc_hw_gpio_set_pwm(ceammc_hw_gpio *gp, uint8_t pin, double period, double width);

/// set software pwm freq on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param freq - freq in Hertz
/// @param duty_cycle - duty cycle in % [0-100] range
bool ceammc_hw_gpio_set_pwm_freq(ceammc_hw_gpio *gp, uint8_t pin, double freq, double duty_cycle);

/// free dht11 sensor
/// @param sr04 - pointer to SR04 struct
void ceammc_hw_gpio_sr04_free(ceammc_hw_gpio_sr04 *sr04);

/// singe measure request
/// @param sr04 - pointer to SR04 struct
bool ceammc_hw_gpio_sr04_measure(const ceammc_hw_gpio_sr04 *sr04);

/// create new SR04 sensor struct
/// @param trigger_pin - connected GPIO pin
/// @param trigger_pin - connected GPIO pin
/// @param notify - data check callback
/// @param on_err - error message callback
/// @param on_data - data callback
ceammc_hw_gpio_sr04 *ceammc_hw_gpio_sr04_new(uint8_t trigger_pin,
                                             uint8_t echo_pin,
                                             ceammc_hw_notify_cb notify,
                                             ceammc_hw_msg_cb on_err,
                                             ceammc_hw_sr04_cb on_data);

/// polling in cycle
/// @param sr04 - pointer to SR04 struct
/// @param state - poll state
bool ceammc_hw_gpio_sr04_poll(const ceammc_hw_gpio_sr04 *sr04, bool state);

/// check measure data
/// @param sr04 - pointer to SR04 struct
bool ceammc_hw_gpio_sr04_process(const ceammc_hw_gpio_sr04 *sr04);

/// set polling interval
/// @param sr04 - pointer to SR04 struct
/// @param poll_interval - polling interval (msec)
bool ceammc_hw_gpio_sr04_set_poll_interval(const ceammc_hw_gpio_sr04 *sr04, uint16_t poll_interval);

/// toggle pin level
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
bool ceammc_hw_gpio_toggle_pin(ceammc_hw_gpio *gp, uint8_t pin);

/// write pin value
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param level - pin level (=0: low, >0: high)
bool ceammc_hw_gpio_write_pin(ceammc_hw_gpio *gp, uint8_t pin, bool level);

/// free i2c struct
/// @param i2c - pointer to i2c struct
void ceammc_hw_i2c_free(ceammc_hw_i2c *i2c);

ceammc_hw_i2c *ceammc_hw_i2c_new(uint8_t addr, ceammc_hw_notify_cb notify, ceammc_hw_msg_cb on_err);

/// turn on/off lcd1602 backlight
/// @param lcd1602 - pointer to LCD1602 struct
/// @param state - on/off state
bool ceammc_hw_lcd1602_backlight(ceammc_hw_lcd1602 *lcd1602, bool state);

/// clear lcd1602 display
/// @param lcd1602 - pointer to LCD1602 struct
bool ceammc_hw_lcd1602_clear(ceammc_hw_lcd1602 *lcd1602);

/// turn on/off lcd1602 cursor
/// @param lcd1602 - pointer to LCD1602 struct
/// @param state - on/off state
bool ceammc_hw_lcd1602_cursor_blink(ceammc_hw_lcd1602 *lcd1602, bool state);

/// turn on/off lcd1602 cursor
/// @param lcd1602 - pointer to LCD1602 struct
/// @param state - on/off state
bool ceammc_hw_lcd1602_cursor_on(ceammc_hw_lcd1602 *lcd1602, bool state);

/// set lcd1602 cursor position
/// @param lcd1602 - pointer to LCD1602 struct
/// @param row - cursor row
/// @param col - cursor column
bool ceammc_hw_lcd1602_cursor_pos(ceammc_hw_lcd1602 *lcd1602, uint8_t row, uint8_t col);

/// free lcd1602 display
/// @param lcd1602 - pointer to LCD1602 struct
void ceammc_hw_lcd1602_free(ceammc_hw_lcd1602 *lcd1602);

/// move lcd1602 cursor right/left
/// @param lcd1602 - pointer to LCD1602 struct
/// @param dir - <0 left, 0>right
bool ceammc_hw_lcd1602_move_cursor(ceammc_hw_lcd1602 *lcd1602, int8_t dir);

ceammc_hw_lcd1602 *ceammc_hw_lcd1602_new(uint8_t rows,
                                         uint8_t i2c_addr,
                                         ceammc_hw_notify_cb notify,
                                         ceammc_hw_msg_cb on_err);

/// process lcd1602 events
/// @param lcd1602 - pointer to LCD1602 struct
bool ceammc_hw_lcd1602_process(ceammc_hw_lcd1602 *lcd1602);

/// scroll lcd1602 text right/left
/// @param lcd1602 - pointer to LCD1602 struct
/// @param dir - <0 left, 0>right
bool ceammc_hw_lcd1602_scroll_text(ceammc_hw_lcd1602 *lcd1602, int8_t dir);

/// write text to lcd1602
/// @param lcd1602 - pointer to LCD1602 struct
/// @param txt - text
bool ceammc_hw_lcd1602_write_text(ceammc_hw_lcd1602 *lcd1602, const char *txt);

/// clear max7219 display
/// @param max7219 - pointer to max7219 struct
/// @param addr - lcd address in chain, if <0 clear all connected addresses
bool ceammc_hw_max7219_clear(ceammc_hw_max7219 *mx, int32_t addr);

/// free max7219 struct
/// @param max7219 - pointer to max7219 struct
void ceammc_hw_max7219_free(ceammc_hw_max7219 *mx);

/// set max7219 intensity
/// @param max7219 - pointer to max7219 struct
/// @param intensity in 0..0xF range
bool ceammc_hw_max7219_intensity(ceammc_hw_max7219 *mx, int32_t addr, uint8_t intens);

/// create new max7219
/// @param num_displays - number of connected lcd modules (1-8)
/// @param spi - RPi SPI bus
/// @param cs - RPi chip select
/// @param notify - notify callback
/// @param on_err - error callback
/// @return pointer to max7219 on NULL on error
///
/// @note The Raspberry Pi’s GPIO header exposes several SPI buses.
/// SPI0 is available on all Raspberry Pi models.
/// SPI1 is available on models with a 40-pin header.
/// SPI2 is only available on the Compute and Compute 3.
/// SPI3 through SPI6 are only available on the Raspberry Pi 4 B, 400 and 5.
ceammc_hw_max7219 *ceammc_hw_max7219_new(uint8_t num_displays,
                                         ceammc_hw_spi_bus spi,
                                         ceammc_hw_spi_cs cs,
                                         ceammc_hw_notify_cb notify,
                                         ceammc_hw_msg_cb on_err);

/// set max7219 power on/off
/// @param max7219 - pointer to max7219 struct
/// @param state
bool ceammc_hw_max7219_power(ceammc_hw_max7219 *mx, bool state);

/// test max7219 display
/// @param max7219 - pointer to max7219 struct
/// @param addr
/// @param state
bool ceammc_hw_max7219_test(ceammc_hw_max7219 *mx, int32_t addr, bool state);

/// write data to max7219
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param data - pointer to data
/// @param len - data length
bool ceammc_hw_max7219_write_bits(ceammc_hw_max7219 *mx,
                                  int32_t addr,
                                  const uint8_t *data,
                                  size_t len);

/// write float to max7219 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param value - float value
/// @param precision - float precision
bool ceammc_hw_max7219_write_float(ceammc_hw_max7219 *mx,
                                   int32_t addr,
                                   float value,
                                   uint8_t precision);

/// write max7219 unsigned hex value to 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param val - unsigned int value to display
bool ceammc_hw_max7219_write_hex(ceammc_hw_max7219 *mx, int32_t addr, uint32_t val);

/// write max7219 int value to 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param val - signed int value to display
bool ceammc_hw_max7219_write_int(ceammc_hw_max7219 *mx, int32_t addr, int32_t val);

/// write raw data to max7219 register
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param reg - register index
/// @param data - register data
/// @note this is low level write function!
bool ceammc_hw_max7219_write_reg(ceammc_hw_max7219 *mx, int32_t addr, uint8_t reg, uint8_t data);

/// write string to max7219 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param str
/// @param align - string align on display
bool ceammc_hw_max7219_write_str(ceammc_hw_max7219 *mx,
                                 int32_t addr,
                                 const char *str,
                                 ceammc_hw_max7219_string_align align,
                                 uint8_t dots);

int32_t ceammc_hw_print_file(const char *printer,
                             const char *path,
                             const ceammc_hw_print_options *opts,
                             ceammc_hw_msg_cb on_err,
                             ceammc_hw_msg_cb on_debug);

bool ceammc_hw_printer_default(ceammc_hw_printer_info_cb info_cb);

} // extern "C"

#endif // ceammc_rust_hw_h
