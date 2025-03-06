#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_char, CStr, CString},
    ptr::null_mut,
};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb};

#[cfg(target_os = "linux")]
mod ssd1306_impl;

#[derive(Debug)]
pub enum Request {
    DrawText(CString, i16, i16),
}

#[derive(Debug)]
pub enum Reply {
    Error(CString),
}

pub struct hw_display_ssd1306 {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_new(
    channel: i8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_display_ssd1306 {
    rpi_check!(null_mut(), {
        match hw_display_ssd1306::new(channel, notify, on_err) {
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
pub extern "C" fn ceammc_hw_display_ssd1306_free(disp: *mut hw_display_ssd1306) {
    rpi_check!((), {
        if !disp.is_null() {
            drop(unsafe { Box::from_raw(disp) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_proc_reply(disp: *const hw_display_ssd1306) -> bool {
    rpi_check!({ hw_display_ssd1306::process_reply(disp) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_text(
    disp: *const hw_display_ssd1306,
    txt: *const c_char,
    x: i16,
    y: i16,
) -> bool {
    rpi_check!({
        let txt = unsafe { CStr::from_ptr(txt).to_str().unwrap() };
        let txt = CString::new(txt).unwrap_or_default();
        hw_display_ssd1306::send_request(disp, Request::DrawText(txt, x, y))
    });
}
