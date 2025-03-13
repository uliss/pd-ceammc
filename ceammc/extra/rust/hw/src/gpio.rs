#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use crate::{hw_msg_cb, hw_notify_cb, HwThreadWorker, MakePdError};
use lib_macro::PdError;
use log::error;
use std::{
    ffi::{c_int, c_void, CString},
    ptr::null_mut,
    time::Duration,
};

pub const HW_GPIO_IMPULSE_LENGTH_MIN_MSEC: f64 = 0.001;
pub const HW_GPIO_IMPULSE_LENGTH_MAX_MSEC: f64 = 100.0;


#[derive(PdError, Clone)]
pub enum Reply {
    PinLevel(u8, bool),
    Error(CString),
    Debug(CString),
    Pins(Vec<u8>),
}

pub enum Request {
    SetOutput(u8),
    SetInput(u8),
    ResetPin(u8),
    Read(u8),
    Write(u8, bool),
    Toggle(u8),
    SetPwmFreq(u8, f64, f64),
    SetPwm(u8, f64, f64),
    ClearPwm(u8),
    SetBias(u8, hw_gpio_bias),
    SetInterrupt(u8, hw_gpio_trigger, Option<Duration>),
    ClearInterrupt(u8),
    Impulse(u8, f64),
    ListPins,
}

type GpioThreadWorker = HwThreadWorker<Request, Reply>;

/// gpio opaque type
pub struct hw_gpio {
    worker: GpioThreadWorker,
    pub on_dbg: hw_msg_cb,
    on_pin: hw_gpio_pin_cb,
    on_pin_list: hw_gpio_pin_list_cb,
}

#[repr(C)]
/// pin value callback
pub struct hw_gpio_pin_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can not be NULL
    cb: extern "C" fn(*mut c_void, u8, bool),
}

#[repr(C)]
/// pin list callback
pub struct hw_gpio_pin_list_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can not be NULL
    cb: extern "C" fn(*mut c_void, *const u8, usize),
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct hw_gpio_poll_cb {
    id: usize,
    cb: extern "C" fn(id: usize, data: c_int),
}

#[repr(C)]
pub enum hw_gpio_bias {
    None,
    PullUp,
    PullDown,
}

#[repr(C)]
pub enum hw_gpio_mode {
    Output,
    Input,
}

#[repr(C)]
pub enum hw_gpio_trigger {
    None,
    RisingEdge,
    FallingEdge,
    Both,
}

#[cfg(target_os = "linux")]
mod gpio_impl;

/// create new gpio
/// @param on_err - on error callback for output error messages
/// @param on_dbg - on error callback for output error messages
/// @param notify - notification update callback
/// @param on_pin - called on pin value output
/// @param on_pin_list - called on pin list reply
/// @param on_pin_poll - called on pin poll event
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_new(
    on_err: hw_msg_cb,
    on_dbg: hw_msg_cb,
    notify: hw_notify_cb,
    on_pin: hw_gpio_pin_cb,
    on_pin_list: hw_gpio_pin_list_cb,
    on_pin_poll: hw_gpio_poll_cb,
) -> *mut hw_gpio {
    rpi_check!(null_mut(), {
        match hw_gpio::new(on_err, on_dbg, notify, on_pin, on_pin_list, on_pin_poll) {
            Ok(gpio) => return Box::into_raw(Box::new(gpio)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                return null_mut();
            }
        }
    });
}

/// create new gpio
/// @param gpio - pointer to gpio struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_free(gpio: *mut hw_gpio) {
    rpi_check!((), {
        if !gpio.is_null() {
            drop(unsafe { Box::from_raw(gpio) })
        }
    });
}

/// process events
/// @param gp - pointer to gpio struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_process_events(gp: *mut hw_gpio) {
    rpi_check!((), { hw_gpio::process_ptr(gp) });
}

/// write pin value
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param level - pin level (=0: low, >0: high)
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_write_pin(gp: *mut hw_gpio, pin: u8, level: bool) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::Write(pin, level)) });
}

/// read pin request
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_read_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::Read(pin)) });
}

/// toggle pin level
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_toggle_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::Toggle(pin)) });
}

/// set software pwm freq on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param freq - freq in Hertz
/// @param duty_cycle - duty cycle in % [0-100] range
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_pwm_freq(
    gp: *mut hw_gpio,
    pin: u8,
    freq: f64,
    duty_cycle: f64,
) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::SetPwmFreq(pin, freq, duty_cycle)) });
}

/// set software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param period - period in msec
/// @param width - duty_cycle width in msec
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_pwm(
    gp: *mut hw_gpio,
    pin: u8,
    period: f64,
    width: f64,
) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::SetPwm(pin, period, width)) });
}

/// clear software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_clear_pwm(gp: *mut hw_gpio, pin: u8) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::ClearPwm(pin)) });
}

/// reset pin to initial state
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_reset_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::ResetPin(pin)) });
}

/// set pin mode
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param mode - pin mode
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_mode(gp: *mut hw_gpio, pin: u8, mode: hw_gpio_mode) -> bool {
    rpi_check!({
        match mode {
            hw_gpio_mode::Output => hw_gpio::send_request_ptr(gp, Request::SetOutput(pin)),
            hw_gpio_mode::Input => hw_gpio::send_request_ptr(gp, Request::SetInput(pin)),
        }
    });
}

/// list used pins
/// @param gpio - pointer to gpio struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_list_pins(gp: *mut hw_gpio) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::ListPins) });
}

/// set pin bias
/// @param gpio - pointer to gpio struct
/// @param pin - pin BCM number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_bias(gp: *mut hw_gpio, pin: u8, bias: hw_gpio_bias) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::SetBias(pin, bias)) });
}

/// poll pin events
/// @param gpio - pointer to gpio struct
/// @param pin - pin BCM number
/// @param trigger - event trigger
/// @param debounce_ms - debounce time in ms
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_poll(
    gp: *mut hw_gpio,
    pin: u8,
    trigger: hw_gpio_trigger,
    debounce_ms: f64,
) -> bool {
    rpi_check!({
        hw_gpio::send_request_ptr(
            gp,
            Request::SetInterrupt(
                pin,
                trigger,
                if debounce_ms <= 0.0 {
                    None
                } else {
                    Some(Duration::from_secs_f64(debounce_ms * 0.001))
                },
            ),
        )
    });
}

/// clear pin event polling
/// @param gpio - pointer to gpio struct
/// @param pin - pin BCM number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_clear_poll(gp: *mut hw_gpio, pin: u8) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::ClearInterrupt(pin)) });
}

/// send single impulse
/// @param gpio - pointer to gpio struct
/// @param length - impulse length in milliseconds >0 and <500
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_impulse(gp: *mut hw_gpio, pin: u8, length: f64) -> bool {
    rpi_check!({ hw_gpio::send_request_ptr(gp, Request::Impulse(pin, length)) });
}
