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
mod max7219_impl;

#[derive(Debug)]
#[repr(C)]

pub enum hw_max7219_string_align {
    Left,
    Right,
    Center,
}

#[derive(Debug)]
pub enum Request {
    Intensity(Option<usize>, u8),
    WriteInt(usize, i32),
    WriteHex(usize, u32),
    WriteFloat(usize, f32, u8),
    WriteDigit(usize, u8, u8),
    WriteString(usize, String, hw_max7219_string_align, u8),
    PowerOn(bool),
    Clear(Option<usize>),
}

pub struct hw_max7219 {
    tx: std::sync::mpsc::Sender<Request>,
    on_err: hw_msg_cb,
}

/// create new max7219
/// @param num_displays - number of connected lcd modules
/// @param notify - notify callback
/// @param on_err - error callback
/// @return pointer to max7219 on NULL on error
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_new(
    num_displays: usize,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_max7219 {
    rpi_check!(null_mut(), {
        match hw_max7219::new(num_displays, notify, on_err) {
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
/// @param intensity in 0..0xF range
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_intensity(mx: *mut hw_max7219, intens: u8, addr: i64) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        if addr < 0 {
            mx.send(Request::Intensity(None, intens));
        } else {
            mx.send(Request::Intensity(Some(addr as usize), intens));
        }
        true
    });
}

/// set max7219 power on/off
/// @param max7219 - pointer to max7219 struct
/// @param state
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_power(mx: *mut hw_max7219, state: bool) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(Request::PowerOn(state));
        true
    });
}

/// clear max7219 display
/// @param max7219 - pointer to max7219 struct
/// @param addr - lcd address in chain, if <0 clear all connected addresses
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_clear(mx: *mut hw_max7219, addr: i64) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        if addr < 0 {
            mx.send(Request::Clear(None));
        } else {
            mx.send(Request::Clear(Some(addr as usize)));
        }
        true
    });
}

/// write max7219 int value to 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param val - signed int value to display
/// @param addr - display address in chain
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_int(mx: *mut hw_max7219, val: i32, addr: usize) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(Request::WriteInt(addr, val));
        true
    });
}

/// write max7219 unsigned hex value to 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param val - unsigned int value to display
/// @param addr - display address in chain
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_hex(mx: *mut hw_max7219, val: u32, addr: usize) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(Request::WriteHex(addr, val));
        true
    });
}

/// write max7219 digit data to 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param digit - digit index
/// @param data - digit data
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_digit_data(
    mx: *mut hw_max7219,
    addr: usize,
    digit: u8,
    data: u8,
) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(Request::WriteDigit(addr, digit, data));
        true
    });
}

/// write float to max7219 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param value - float value
/// @param precision - float precision
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_float(
    mx: *mut hw_max7219,
    addr: usize,
    value: f32,
    precision: u8,
) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(Request::WriteFloat(addr, value, precision));
        true
    });
}

/// write string to max7219 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param str
/// @param align - string align on display
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_str(
    mx: *mut hw_max7219,
    addr: usize,
    str: *const c_char,
    align: hw_max7219_string_align,
    dots: u8,
) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        let str = unsafe { CStr::from_ptr(str) }.to_string_lossy().to_string();
        mx.send(Request::WriteString(addr, str, align, dots));
        true
    });
}
