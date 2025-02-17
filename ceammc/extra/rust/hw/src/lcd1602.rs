#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_char, CStr},
    ptr::null_mut,
};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb};

#[cfg(target_os = "linux")]
mod lcd1602_impl;

pub enum Request {
    WriteText(String),
    Clear,
    Backlight(bool),
}

pub struct hw_lcd1602 {
    // result: Arc<(Mutex<Option<Reply>>, std::sync::Condvar)>,
    tx: std::sync::mpsc::Sender<Request>,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_new(
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_lcd1602 {
    rpi_check!(null_mut(), {
        match hw_lcd1602::new(None, notify, on_err) {
            Ok(lcd1602) => return Box::into_raw(Box::new(lcd1602)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

/// free lcd1602 display
/// @param lcd1602 - pointer to LCD1602 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_free(lcd1602: *mut hw_lcd1602) {
    rpi_check!((), {
        if !lcd1602.is_null() {
            drop(unsafe { Box::from_raw(lcd1602) })
        }
    });
}

/// clear lcd1602 display
/// @param lcd1602 - pointer to LCD1602 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_clear(lcd1602: *mut hw_lcd1602) -> bool {
    rpi_check!({
        if lcd1602.is_null() {
            error!("NULL lcd1602 pointer");
            return false;
        }

        let lcd1602 = unsafe { &*lcd1602 };
        lcd1602.send(Request::Clear)
    });
}

/// turn on/off lcd1602 backlight
/// @param lcd1602 - pointer to LCD1602 struct
/// @param state - on/off state
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_backlight(lcd1602: *mut hw_lcd1602, state: bool) -> bool {
    rpi_check!({
        if lcd1602.is_null() {
            error!("NULL lcd1602 pointer");
            return false;
        }

        let lcd1602 = unsafe { &*lcd1602 };
        lcd1602.send(Request::Backlight(state))
    });
}

/// write text to lcd1602
/// @param lcd1602 - pointer to LCD1602 struct
/// @param txt - text
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_write_text(
    lcd1602: *mut hw_lcd1602,
    txt: *const c_char,
) -> bool {
    rpi_check!({
        if lcd1602.is_null() {
            error!("NULL lcd1602 pointer");
            return false;
        }

        let lcd1602 = unsafe { &*lcd1602 };
        let txt = unsafe { CStr::from_ptr(txt).to_str().unwrap() };
        lcd1602.send(Request::WriteText(txt.to_owned()))
    });
}
