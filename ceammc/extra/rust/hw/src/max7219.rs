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

#[repr(C)]
#[derive(Debug)]
pub enum hw_spi_bus {
    SPI0,
    SPI1,
    SPI2,
    SPI3,
    SPI4,
    SPI5,
    SPI6,
}

#[repr(C)]
#[derive(Debug)]
pub enum hw_spi_cs {
    CS0,
    CS1,
    CS2,
    CS3,
}

#[derive(Debug, Clone, Copy)]
#[repr(C)]

pub enum hw_max7219_string_align {
    Left,
    Right,
    Center,
}

#[derive(Debug)]
pub enum Request {
    Intensity(u8),
    WriteInt(i32),
    WriteHex(u32),
    WriteFloat(f32, u8),
    WriteDigit(u8, u8),
    WriteString(String, hw_max7219_string_align, u8),
    PowerOn(bool),
    Clear,
    Test(bool),
}

#[derive(Debug)]
pub enum Address {
    All,
    Single(u8),
}

pub struct hw_max7219 {
    displays: u8,
    tx: std::sync::mpsc::Sender<(Address, Request)>,
    on_err: hw_msg_cb,
}

/// create new max7219
/// @param num_displays - number of connected lcd modules (1-8)
/// @param spi - RPi SPI bus
/// @param cs - RPi chip select
/// @param notify - notify callback
/// @param on_err - error callback
/// @return pointer to max7219 on NULL on error
///
/// @note The Raspberry Pi’s GPIO header exposes several SPI buses.
/// SPI0 is available on all Raspberry Pi models.
/// SPI1 is available on models with a 40-pin header.
/// SPI2 is only available on the Compute and Compute 3.
/// SPI3 through SPI6 are only available on the Raspberry Pi 4 B, 400 and 5.
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_new(
    num_displays: u8,
    spi: hw_spi_bus,
    cs: hw_spi_cs,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_max7219 {
    rpi_check!(null_mut(), {
        match hw_max7219::new(num_displays, spi, cs, notify, on_err) {
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
pub extern "C" fn ceammc_hw_max7219_intensity(mx: *mut hw_max7219, intens: u8, addr: i32) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(addr, Request::Intensity(intens));
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
        // send to all internally in max7219 crate
        mx.send(0, Request::PowerOn(state));
        true
    });
}

/// clear max7219 display
/// @param max7219 - pointer to max7219 struct
/// @param addr - lcd address in chain, if <0 clear all connected addresses
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_clear(mx: *mut hw_max7219, addr: i32) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(addr, Request::Clear);
        true
    });
}

/// write max7219 int value to 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param val - signed int value to display
/// @param addr - display address in chain
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_int(mx: *mut hw_max7219, val: i32, addr: i32) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(addr, Request::WriteInt(val));
        true
    });
}

/// write max7219 unsigned hex value to 7 segment display
/// @param max7219 - pointer to max7219 struct
/// @param val - unsigned int value to display
/// @param addr - display address in chain
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_hex(mx: *mut hw_max7219, val: u32, addr: i32) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(addr, Request::WriteHex(val));
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
    addr: i32,
    digit: u8,
    data: u8,
) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(addr, Request::WriteDigit(digit, data));
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
    addr: i32,
    value: f32,
    precision: u8,
) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(addr, Request::WriteFloat(value, precision));
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
    addr: i32,
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
        mx.send(addr, Request::WriteString(str, align, dots));
        true
    });
}

/// test max7219 display
/// @param max7219 - pointer to max7219 struct
/// @param addr
/// @param state
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_test(mx: *mut hw_max7219, addr: i32, state: bool) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(addr, Request::Test(state));
        true
    });
}
