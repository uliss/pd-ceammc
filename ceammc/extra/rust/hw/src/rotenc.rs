#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::{c_void, CString}, ptr::null_mut};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb};

#[cfg(target_os = "linux")]
mod rotenc_impl;

#[derive(Debug)]
pub enum Request {
    SetValue(i32),
    ResetValue,
}

#[derive(Debug)]
pub enum Reply {
    Error(CString),
    Click,
    Data(i32, i8),
}

pub struct hw_gpio_rotenc {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_data: hw_gpio_rotenc_data,
    on_err: hw_msg_cb,
}

#[repr(C)]
pub struct hw_gpio_rotenc_data {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can not be NULL
    cb: extern "C" fn(*mut c_void, i32, i8),
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_new(
    dt: u8,
    clk: u8,
    btn: u8,
    init: i32,
    notify: hw_notify_cb,
    on_data: hw_gpio_rotenc_data,
    on_err: hw_msg_cb,
) -> *mut hw_gpio_rotenc {
    rpi_check!(null_mut(), {
        match hw_gpio_rotenc::new(dt, clk, btn, init, notify, on_data, on_err) {
            Ok(pwm) => return Box::into_raw(Box::new(pwm)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_free(enc: *mut hw_gpio_rotenc) {
    rpi_check!((), {
        if !enc.is_null() {
            drop(unsafe { Box::from_raw(enc) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_process_events(enc: *mut hw_gpio_rotenc) {
    rpi_check!((), { hw_gpio_rotenc::process_ptr(enc) });
}
