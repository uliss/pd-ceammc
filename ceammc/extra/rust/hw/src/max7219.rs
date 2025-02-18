#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::ptr::null_mut;

use log::error;

use crate::{hw_msg_cb, hw_notify_cb};

#[cfg(target_os = "linux")]
mod max7219_impl;

pub enum Request {
    // ScanDevices,
    Intensity(u8),
}

pub struct hw_max7219 {
    tx: std::sync::mpsc::Sender<Request>,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_new(
    addr: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_max7219 {
    rpi_check!(null_mut(), {
        match hw_max7219::new(addr, notify, on_err) {
            Ok(max2719) => return Box::into_raw(Box::new(max2719)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

/// free max7219 struct
/// @param max7219 - pointer to max7219 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_free(mx: *mut hw_max7219) {
    rpi_check!((), {
        if !mx.is_null() {
            drop(unsafe { Box::from_raw(mx) })
        }
    });
}

/// set max7219 intensity
/// @param max7219 - pointer to max7219 struct
/// @param intensity
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_intensity(mx: *mut hw_max7219, intens: u8) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(Request::Intensity(intens));
        true
    });
}
