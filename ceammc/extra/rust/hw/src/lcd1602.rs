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
mod lcd1602_impl;

#[derive(Debug)]
pub enum Request {
    WriteText(String),
    Clear,
    Backlight(bool),
    CursorOn(bool),
    CursorBlink(bool),
    CursorPos(u8, u8),
    CursorMove(i8),
    TextScroll(i8),
}

pub enum Reply {
    Error(CString),
}

pub struct hw_lcd1602 {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_new(
    rows: u8,
    i2c_addr: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_lcd1602 {
    rpi_check!(null_mut(), {
        match hw_lcd1602::new(
            rows,
            if i2c_addr < 0x08 {
                None
            } else {
                Some(i2c_addr)
            },
            on_err,
            notify,
        ) {
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

/// turn on/off lcd1602 cursor
/// @param lcd1602 - pointer to LCD1602 struct
/// @param state - on/off state
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_cursor_on(lcd1602: *mut hw_lcd1602, state: bool) -> bool {
    rpi_check!({
        if lcd1602.is_null() {
            error!("NULL lcd1602 pointer");
            return false;
        }

        let lcd1602 = unsafe { &*lcd1602 };
        lcd1602.send(Request::CursorOn(state))
    });
}

/// turn on/off lcd1602 cursor
/// @param lcd1602 - pointer to LCD1602 struct
/// @param state - on/off state
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_cursor_blink(lcd1602: *mut hw_lcd1602, state: bool) -> bool {
    rpi_check!({
        if lcd1602.is_null() {
            error!("NULL lcd1602 pointer");
            return false;
        }

        let lcd1602 = unsafe { &*lcd1602 };
        lcd1602.send(Request::CursorBlink(state))
    });
}

/// set lcd1602 cursor position
/// @param lcd1602 - pointer to LCD1602 struct
/// @param row - cursor row
/// @param col - cursor column
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_cursor_pos(lcd1602: *mut hw_lcd1602, row: u8, col: u8) -> bool {
    rpi_check!({
        if lcd1602.is_null() {
            error!("NULL lcd1602 pointer");
            return false;
        }

        let lcd1602 = unsafe { &*lcd1602 };
        lcd1602.send(Request::CursorPos(row, col))
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

/// move lcd1602 cursor right/left
/// @param lcd1602 - pointer to LCD1602 struct
/// @param dir - <0 left, 0>right
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_move_cursor(lcd1602: *mut hw_lcd1602, dir: i8) -> bool {
    rpi_check!({
        if lcd1602.is_null() {
            error!("NULL lcd1602 pointer");
            return false;
        }

        let lcd1602 = unsafe { &*lcd1602 };
        lcd1602.send(Request::CursorMove(dir))
    });
}

/// scroll lcd1602 text right/left
/// @param lcd1602 - pointer to LCD1602 struct
/// @param dir - <0 left, 0>right
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_scroll_text(lcd1602: *mut hw_lcd1602, dir: i8) -> bool {
    rpi_check!({
        if lcd1602.is_null() {
            error!("NULL lcd1602 pointer");
            return false;
        }

        let lcd1602 = unsafe { &*lcd1602 };
        lcd1602.send(Request::TextScroll(dir))
    });
}

/// process lcd1602 events
/// @param lcd1602 - pointer to LCD1602 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_process(lcd1602: *mut hw_lcd1602) -> bool {
    rpi_check!({
        if lcd1602.is_null() {
            error!("NULL lcd1602 pointer");
            return false;
        }

        let lcd1602 = unsafe { &*lcd1602 };
        lcd1602.process_reply();
        true
    });
}
