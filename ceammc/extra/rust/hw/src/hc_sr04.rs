#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

#[cfg(target_os = "linux")]
mod hc_sr04_impl;

use std::{
    ffi::c_void,
    ptr::null_mut,
    sync::{Arc, Mutex},
};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb};

#[repr(C)]
#[allow(non_camel_case_types)]
/// error callback
pub struct hw_sr04_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// not NULL!
    cb: extern "C" fn(*mut c_void, distance_cm: f32, is_inf: bool),
}

impl hw_sr04_cb {
    pub fn exec(&self, data: f32, inf: bool) {
        (self.cb)(self.user, data, inf);
    }
}

pub const HW_SR04_MIN_POLL_INTERVAL: u16 = 10;
pub const HW_SR04_DEF_POLL_INTERVAL: u16 = 20;
pub const HW_SR04_MAX_POLL_INTERVAL: u16 = 1000;

#[derive(Debug)]
enum Reply {
    Measure(f32),
}

pub enum Request {
    Poll(bool),
    OneShot,
    SetPollTime(u16),
}

pub struct hw_gpio_sr04 {
    result: Arc<(Mutex<Option<Reply>>, std::sync::Condvar)>,
    tx: std::sync::mpsc::Sender<Request>,
    on_err: hw_msg_cb,
    on_data: hw_sr04_cb,
}

/// create new SR04 sensor struct
/// @param trigger_pin - connected GPIO pin
/// @param trigger_pin - connected GPIO pin
/// @param notify - data check callback
/// @param on_err - error message callback
/// @param on_data - data callback
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_new(
    trigger_pin: u8,
    echo_pin: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
    on_data: hw_sr04_cb,
) -> *mut hw_gpio_sr04 {
    rpi_check!(null_mut(), {
        match hw_gpio_sr04::new(trigger_pin, echo_pin, notify, on_err, on_data) {
            Ok(sr04) => return Box::into_raw(Box::new(sr04)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

/// free dht11 sensor
/// @param sr04 - pointer to SR04 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_free(sr04: *mut hw_gpio_sr04) {
    rpi_check!((), {
        if !sr04.is_null() {
            drop(unsafe { Box::from_raw(sr04) })
        }
    });
}

/// singe measure request
/// @param sr04 - pointer to SR04 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_measure(sr04: *const hw_gpio_sr04) -> bool {
    rpi_check!({
        if sr04.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let sr04 = unsafe { &*sr04 };
        sr04.send(Request::OneShot)
    });
}

/// polling in cycle
/// @param sr04 - pointer to SR04 struct
/// @param state - poll state
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_poll(sr04: *const hw_gpio_sr04, state: bool) -> bool {
    rpi_check!({
        if sr04.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let sr04 = unsafe { &*sr04 };
        sr04.send(Request::Poll(state))
    });
}

/// set polling interval
/// @param sr04 - pointer to SR04 struct
/// @param poll_interval - polling interval (msec)
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_set_poll_interval(
    sr04: *const hw_gpio_sr04,
    poll_interval: u16,
) -> bool {
    rpi_check!({
        if sr04.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let sr04 = unsafe { &*sr04 };
        sr04.send(Request::SetPollTime(poll_interval))
    });
}

/// check measure data
/// @param sr04 - pointer to SR04 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_process(sr04: *const hw_gpio_sr04) -> bool {
    rpi_check!({
        if sr04.is_null() {
            error!("NULL sr04 pointer");
            return false;
        }

        let sr04 = unsafe { &*sr04 };
        sr04.check_result();
        true
    });
}
