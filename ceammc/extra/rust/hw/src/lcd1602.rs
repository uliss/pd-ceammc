#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_char, CString},
    ptr::null_mut,
};

use ceammc_rs_msg::msg_notify;
use lib_macro::PdMessage;
use log::error;

use crate::{hw_msg_cb, hw_msg_level, i2c::I2cAddress, ptr_to_cstr, HwThreadWorker, MakePdMessage};

#[cfg(target_os = "linux")]
mod lcd1602_impl;

#[repr(C)]
#[derive(Debug)]
pub enum hw_hd44780_font {
    FONT_5x8,
    FONT_5x10,
}

#[derive(Debug)]
pub enum Request {
    WriteText(CString),
    WriteChar(u32),
    Clear,
    Backlight(bool),
    CursorOn(bool),
    CursorBlink(bool),
    CursorPos(u8, u8),
    CursorMove(i8),
    TextScroll(i8),
    SetFont(hw_hd44780_font),
}

#[derive(PdMessage)]
pub enum Reply {
    Message(hw_msg_level, CString),
}

type Hd44780Worker = HwThreadWorker<Request, Reply>;

pub struct hw_hd44780 {
    worker: Hd44780Worker,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_new(
    i2c_bus: i8,
    i2c_addr: i8,
    rows: u8,
    notify: msg_notify,
    on_msg: hw_msg_cb,
) -> *mut hw_hd44780 {
    rpi_check!(null_mut(), {
        match hw_hd44780::new(i2c_bus, I2cAddress::new(i2c_addr), rows, on_msg, notify) {
            Ok(lcd) => return Box::into_raw(Box::new(lcd)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_msg.error_cstr(err);
                return null_mut();
            }
        }
    });
}

/// free hd44780 display
///  @param lcd - pointer to HD44780 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_free(lcd: *mut hw_hd44780) {
    rpi_check!((), {
        if !lcd.is_null() {
            drop(unsafe { Box::from_raw(lcd) })
        }
    });
}

/// clear hd44780 display
/// @param lcd - pointer to HD44780 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_clear(lcd: *mut hw_hd44780) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::Clear) });
}

/// turn on/off hd44780 backlight
/// @param lcd - pointer to HD44780 struct
/// @param state - on/off state
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_backlight(lcd: *mut hw_hd44780, state: bool) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::Backlight(state)) });
}

/// turn on/off hd44780 cursor
/// @param lcd - pointer to HD44780 struct
/// @param state - on/off state
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_cursor_on(lcd: *mut hw_hd44780, state: bool) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::CursorOn(state)) });
}

/// turn on/off hd44780 cursor
/// @param lcd - pointer to HD44780 struct
/// @param state - on/off state
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_cursor_blink(lcd: *mut hw_hd44780, state: bool) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::CursorBlink(state)) });
}

/// set hd44780 cursor position
/// @param lcd - pointer to HD44780 struct
/// @param row - cursor row
/// @param col - cursor column
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_cursor_pos(lcd: *mut hw_hd44780, row: u8, col: u8) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::CursorPos(row, col)) });
}

/// write text to hd44780
/// @param lcd - pointer to HD44780 struct
/// @param txt - text
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_write_text(lcd: *mut hw_hd44780, txt: *const c_char) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::WriteText(ptr_to_cstr(txt))) });
}

/// write char to hd44780
/// @param lcd - pointer to HD44780 struct
/// @param c - char
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_write_char(lcd: *mut hw_hd44780, c: u8) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::WriteChar(c.into())) });
}

/// move hd44780 cursor right/left
/// @param lcd - pointer to HD44780 struct
/// @param dir - <0 left, 0>right
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_move_cursor(lcd: *mut hw_hd44780, dir: i8) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::CursorMove(dir)) });
}

/// scroll hd44780 text right/left
/// @param lcd - pointer to HD44780 struct
/// @param dir - <0 left, 0>right
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_scroll_text(lcd: *mut hw_hd44780, dir: i8) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::TextScroll(dir)) });
}

/// set hd44780 text font
/// @param lcd - pointer to HD44780 struct
/// @param font
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_set_font(lcd: *mut hw_hd44780, font: hw_hd44780_font) -> bool {
    rpi_check!({ hw_hd44780::send_request_ptr(lcd, Request::SetFont(font)) });
}

/// process hd44780 events
/// @param lcd - pointer to HD44780 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_hd44780_process(lcd: *mut hw_hd44780) -> bool {
    rpi_check!({ hw_hd44780::process_reply_ptr(lcd) });
}
