#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]

use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
    sync::{mpsc::TryRecvError, Arc, Mutex},
    time::Duration,
};

use log::{debug, error};

use crate::{hw_msg_cb, hw_notify_cb};

#[repr(C)]
#[allow(non_camel_case_types)]
/// error callback
pub struct hw_dht11_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// not NULL!
    cb: extern "C" fn(*mut c_void, temp: f64, hum: f64),
}

impl hw_dht11_cb {
    pub fn exec(&self, temp: f64, hum: f64) {
        (self.cb)(self.user, temp, hum);
    }
}

enum Reply {
    Measure(f64, f64),
    Error(CString),
}

enum Request {
    Poll(bool),
    OneShot,
}

pub struct hw_gpio_dht11 {
    result: Arc<Mutex<Option<Reply>>>,
    tx: std::sync::mpsc::Sender<Request>,
    on_err: hw_msg_cb,
    on_data: hw_dht11_cb,
}

#[cfg(target_os = "linux")]
mod dht11_impl;

/// create new DHT11 sensor struct
/// @param pin - connected GPIO pin
/// @param notify - data check callback
/// @param on_err - error message callback
/// @param on_data - data callback
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_new(
    pin: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
    on_data: hw_dht11_cb,
) -> *mut hw_gpio_dht11 {
    gpio_check!(null_mut(), {
        match hw_gpio_dht11::new(pin, notify, on_err, on_data) {
            Ok(dht) => return Box::into_raw(Box::new(dht)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

/// free dht11 sensor
/// @param dht - pointer to DHT11 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_free(dht: *mut hw_gpio_dht11) {
    gpio_check!((), {
        if !dht.is_null() {
            drop(unsafe { Box::from_raw(dht) })
        }
    });
}

/// singe measure request
/// @param dht - pointer to DHT11 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_measure(dht: *const hw_gpio_dht11) -> bool {
    gpio_check!({
        if dht.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let dht = unsafe { &*dht };
        dht.send(Request::OneShot)
    });
}

/// poll request
/// @param dht - pointer to DHT11 struct
/// @param state - poll state
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_poll(dht: *const hw_gpio_dht11, state: bool) -> bool {
    gpio_check!({
        if dht.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let dht = unsafe { &*dht };
        dht.send(Request::Poll(state))
    });
}

/// check measure data
/// @param dht - pointer to DHT11 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_process(dht: *const hw_gpio_dht11) -> bool {
    gpio_check!({
        if dht.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let dht = unsafe { &*dht };
        dht.check_result();
        true
    });
}
