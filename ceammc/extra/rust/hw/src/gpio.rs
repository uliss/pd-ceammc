#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![allow(non_camel_case_types)]

use crate::{hw_msg_cb, hw_notify_cb};
use log::{debug, error};
use std::{
    ffi::{c_int, c_void},
    ptr::null_mut, time::Duration,
};

#[cfg(not(target_os="linux"))]
pub struct hw_gpio;

#[cfg(target_os="linux")]
use gpio_impl::{hw_gpio, HwGpioReply, HwGpioRequest};
#[cfg(target_os="linux")]
use rppal::gpio;

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

macro_rules! return_unsupported {
    ($x:expr) => {
        #[cfg(not(target_os = "linux"))]
        {
            error!("OS != \"linux\" is not supported");
            return $x;
        }
    };
}

macro_rules! gpio_check {
    ($res:expr, $code:block) => {
        return_unsupported!($res);

        #[cfg(target_os = "linux")]
        $code
    };
    ($code:block) => {
        return_unsupported!(false);

        #[cfg(target_os = "linux")]
        return $code;
    };
}

#[cfg(target_os = "linux")]
mod gpio_impl;

mod dht11;

#[cfg(target_os = "linux")]
mod hc_sr04;

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
    gpio_check!(null_mut(), {
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
    gpio_check!((), {
        if !gpio.is_null() {
            drop(unsafe { Box::from_raw(gpio) })
        }
    });
}

/// process events
/// @param gp - pointer to gpio struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_process_events(gp: *mut hw_gpio) {
    gpio_check!((), {
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return;
        }

        let gp = unsafe { &mut *gp };

        while let Ok(reply) = gp.try_recv() {
            match reply {
                HwGpioReply::PinLevel(pin, level) => {
                    gp.exec_pin(pin, level);
                    debug!("pin [{pin}] = {level}");
                }
                HwGpioReply::Error(msg) => {
                    gp.on_err.exec_raw(msg.as_ptr());
                }
                HwGpioReply::Debug(msg) => {
                    gp.on_dbg.exec_raw(msg.as_ptr());
                }
                HwGpioReply::Pins(items) => {
                    gp.exec_pin_list(&items);
                }
            }
        }
    });
}

/// write pin value
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param level - pin level (=0: low, >0: high)
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_write_pin(gp: *mut hw_gpio, pin: u8, level: bool) -> bool {
    gpio_check!({
        if gp.is_null() {
            error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };

        gp.send(HwGpioRequest::Write(pin, level))
    });
}

/// read pin request
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_read_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };

        gp.send(HwGpioRequest::Read(pin))
    });
}

/// toggle pin level
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_toggle_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.send(HwGpioRequest::Toggle(pin))
    });
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
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.send(HwGpioRequest::SetPwmFreq(pin, freq, duty_cycle))
    });
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
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.send(HwGpioRequest::SetPwm(pin, period, width))
    });
}

/// clear software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_clear_pwm(gp: *mut hw_gpio, pin: u8) -> bool {
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.send(HwGpioRequest::ClearPwm(pin))
    });
}

/// reset pin to initial state
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_reset_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.send(HwGpioRequest::ResetPin(pin))
    });
}

/// set pin mode
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param mode - pin mode
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_mode(gp: *mut hw_gpio, pin: u8, mode: hw_gpio_mode) -> bool {
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        match mode {
            hw_gpio_mode::Output => gp.send(HwGpioRequest::SetOutput(pin)),
            hw_gpio_mode::Input => gp.send(HwGpioRequest::SetInput(pin)),
        }
    });
}

/// list used pins
/// @param gpio - pointer to gpio struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_list_pins(gp: *mut hw_gpio) -> bool {
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.send(HwGpioRequest::ListPins)
    });
}

/// set pin bias
/// @param gpio - pointer to gpio struct
/// @param pin - pin BCM number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_bias(gp: *mut hw_gpio, pin: u8, bias: hw_gpio_bias) -> bool {
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.send(HwGpioRequest::SetBias(
            pin,
            match bias {
                hw_gpio_bias::None => gpio::Bias::Off,
                hw_gpio_bias::PullUp => gpio::Bias::PullUp,
                hw_gpio_bias::PullDown => gpio::Bias::PullDown,
            },
        ))
    });
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
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.send(HwGpioRequest::SetInterrupt(
            pin,
            match trigger {
                hw_gpio_trigger::None => gpio::Trigger::Disabled,
                hw_gpio_trigger::RisingEdge => gpio::Trigger::RisingEdge,
                hw_gpio_trigger::FallingEdge => gpio::Trigger::FallingEdge,
                hw_gpio_trigger::Both => gpio::Trigger::Both,
            },
            if debounce_ms <= 0.0 {
                None
            } else {
                Some(Duration::from_secs_f64(debounce_ms * 0.001))
            },
        ))
    });
}

/// clear pin event polling
/// @param gpio - pointer to gpio struct
/// @param pin - pin BCM number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_clear_poll(gp: *mut hw_gpio, pin: u8) -> bool {
    gpio_check!({
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.send(HwGpioRequest::ClearInterrupt(pin))
    });
}
