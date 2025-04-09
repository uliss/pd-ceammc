#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_char, CString},
    ptr::null_mut,
};

use lib_macro::PdMessage;
use log::error;

use crate::{hw_msg_cb, hw_msg_level, hw_notify_cb, i2c::I2cAddress, ptr_to_cstr, HwThreadWorker, MakePdMessage};

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

type Lcd1602Worker = HwThreadWorker<Request, Reply>;

pub struct hw_lcd1602 {
    worker: Lcd1602Worker,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_new(
    i2c_bus: i8,
    i2c_addr: i8,
    rows: u8,
    notify: hw_notify_cb,
    on_msg: hw_msg_cb,
) -> *mut hw_lcd1602 {
    rpi_check!(null_mut(), {
        match hw_lcd1602::new(i2c_bus, I2cAddress::new(i2c_addr), rows, on_msg, notify) {
            Ok(lcd1602) => return Box::into_raw(Box::new(lcd1602)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_msg.error_cstr(err);
                return null_mut();
            }
        }
    });
}

/// free lcd1602 display
///  @param lcd - pointer to LCD1602 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_free(lcd1602: *mut hw_lcd1602) {
    rpi_check!((), {
        if !lcd1602.is_null() {
            drop(unsafe { Box::from_raw(lcd1602) })
        }
    });
}

/// clear lcd1602 display
/// @param lcd - pointer to LCD1602 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_clear(lcd: *mut hw_lcd1602) -> bool {
    rpi_check!({ hw_lcd1602::send_request_ptr(lcd, Request::Clear) });
}

/// turn on/off lcd1602 backlight
/// @param lcd - pointer to LCD1602 struct
/// @param state - on/off state
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_backlight(lcd: *mut hw_lcd1602, state: bool) -> bool {
    rpi_check!({ hw_lcd1602::send_request_ptr(lcd, Request::Backlight(state)) });
}

/// turn on/off lcd1602 cursor
/// @param lcd - pointer to LCD1602 struct
/// @param state - on/off state
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_cursor_on(lcd: *mut hw_lcd1602, state: bool) -> bool {
    rpi_check!({ hw_lcd1602::send_request_ptr(lcd, Request::CursorOn(state)) });
}

/// turn on/off lcd1602 cursor
/// @param lcd - pointer to LCD1602 struct
/// @param state - on/off state
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_cursor_blink(lcd: *mut hw_lcd1602, state: bool) -> bool {
    rpi_check!({ hw_lcd1602::send_request_ptr(lcd, Request::CursorBlink(state)) });
}

/// set lcd1602 cursor position
/// @param lcd - pointer to LCD1602 struct
/// @param row - cursor row
/// @param col - cursor column
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_cursor_pos(lcd: *mut hw_lcd1602, row: u8, col: u8) -> bool {
    rpi_check!({ hw_lcd1602::send_request_ptr(lcd, Request::CursorPos(row, col)) });
}

/// write text to lcd1602
/// @param lcd - pointer to LCD1602 struct
/// @param txt - text
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_write_text(lcd: *mut hw_lcd1602, txt: *const c_char) -> bool {
    rpi_check!({ hw_lcd1602::send_request_ptr(lcd, Request::WriteText(ptr_to_cstr(txt))) });
}

/// move lcd1602 cursor right/left
/// @param lcd - pointer to LCD1602 struct
/// @param dir - <0 left, 0>right
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_move_cursor(lcd: *mut hw_lcd1602, dir: i8) -> bool {
    rpi_check!({ hw_lcd1602::send_request_ptr(lcd, Request::CursorMove(dir)) });
}

/// scroll lcd1602 text right/left
/// @param lcd - pointer to LCD1602 struct
/// @param dir - <0 left, 0>right
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_scroll_text(lcd: *mut hw_lcd1602, dir: i8) -> bool {
    rpi_check!({ hw_lcd1602::send_request_ptr(lcd, Request::TextScroll(dir)) });
}

/// set lcd1602 text font
/// @param lcd - pointer to LCD1602 struct
/// @param font
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_set_font(lcd: *mut hw_lcd1602, font: hw_hd44780_font) -> bool {
    rpi_check!({ hw_lcd1602::send_request_ptr(lcd, Request::SetFont(font)) });
}

/// process lcd1602 events
/// @param lcd - pointer to LCD1602 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_lcd1602_process(lcd: *mut hw_lcd1602) -> bool {
    rpi_check!({ hw_lcd1602::process_reply_ptr(lcd) });
}
