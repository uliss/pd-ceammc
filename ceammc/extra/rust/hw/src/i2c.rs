#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::ptr::null_mut;

use log::error;

use crate::{hw_msg_cb, hw_notify_cb};

#[cfg(target_os = "linux")]
mod i2c_impl;

pub enum Request {
    ScanDevices,
}

pub struct hw_i2c {
    // result: Arc<(Mutex<Option<Reply>>, std::sync::Condvar)>,
    tx: std::sync::mpsc::Sender<Request>,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_i2c_new(
    addr: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_i2c {
    rpi_check!(null_mut(), {
        match hw_i2c::new(addr, notify, on_err) {
            Ok(i2c) => return Box::into_raw(Box::new(i2c)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

/// free i2c struct
/// @param i2c - pointer to i2c struct
#[no_mangle]
pub extern "C" fn ceammc_hw_i2c_free(i2c: *mut hw_i2c) {
    rpi_check!((), {
        if !i2c.is_null() {
            drop(unsafe { Box::from_raw(i2c) })
        }
    });
}
