#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
};

use lib_macro::PdMessage;
use log::error;

use crate::{hw_msg_cb, hw_msg_level, hw_notify_cb, HwThreadWorker, MakePdMessage};

#[cfg(target_os = "linux")]
mod rotenc_impl;

#[derive(Debug)]
pub enum Request {
    SetValue(f64),
    SetStep(f64),
    SetMin(f64),
    SetMax(f64),
    ResetValue,
    GetValue,
}

#[derive(Debug, PdMessage)]
pub enum Reply {
    Message(hw_msg_level, CString),
    Click(bool),
    Data(f64, i8),
}

type RotEncoderWorker = HwThreadWorker<Request, Reply>;

pub struct hw_gpio_rotenc {
    worker: RotEncoderWorker,
    on_data: hw_gpio_rotenc_data,
    on_click: hw_gpio_rotenc_click,
}

#[repr(C)]
pub struct hw_gpio_rotenc_data {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can not be NULL
    cb: extern "C" fn(*mut c_void, f64, i8),
}

#[repr(C)]
pub struct hw_gpio_rotenc_click {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can not be NULL
    cb: extern "C" fn(*mut c_void, bool),
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_new(
    dt: u8,
    clk: u8,
    btn: u8,
    init: f64,
    step: f64,
    min_value: f64,
    max_value: f64,
    notify: hw_notify_cb,
    on_data: hw_gpio_rotenc_data,
    on_click: hw_gpio_rotenc_click,
    on_msg: hw_msg_cb,
) -> *mut hw_gpio_rotenc {
    rpi_check!(null_mut(), {
        match hw_gpio_rotenc::new(
            dt, clk, btn, init, step, min_value, max_value, notify, on_data, on_click, on_msg,
        ) {
            Ok(pwm) => return Box::into_raw(Box::new(pwm)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_msg.error_cstr(err);
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
    rpi_check!((), { hw_gpio_rotenc::process_reply_ptr(enc) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_reset(enc: *mut hw_gpio_rotenc) -> bool {
    rpi_check!({ hw_gpio_rotenc::send_request_ptr(enc, Request::ResetValue) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_get_value(enc: *mut hw_gpio_rotenc) -> bool {
    rpi_check!({ hw_gpio_rotenc::send_request_ptr(enc, Request::GetValue) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_set_value(enc: *mut hw_gpio_rotenc, value: f64) -> bool {
    rpi_check!({ hw_gpio_rotenc::send_request_ptr(enc, Request::SetValue(value)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_set_step(enc: *mut hw_gpio_rotenc, step: f64) -> bool {
    rpi_check!({ hw_gpio_rotenc::send_request_ptr(enc, Request::SetStep(step)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_set_min(enc: *mut hw_gpio_rotenc, val: f64) -> bool {
    rpi_check!({ hw_gpio_rotenc::send_request_ptr(enc, Request::SetMin(val)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_rotenc_set_max(enc: *mut hw_gpio_rotenc, val: f64) -> bool {
    rpi_check!({ hw_gpio_rotenc::send_request_ptr(enc, Request::SetMax(val)) });
}
