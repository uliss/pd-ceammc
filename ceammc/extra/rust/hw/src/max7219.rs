#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_char, CStr},
    ptr::null_mut,
    slice::from_raw_parts,
};

use log::{debug, error};

use crate::{hw_msg_cb, hw_notify_cb};

pub const HW_MAX7219_REG_DIGIT_0: u8 = 0x1;
pub const HW_MAX7219_REG_DIGIT_1: u8 = 0x2;
pub const HW_MAX7219_REG_DIGIT_2: u8 = 0x3;
pub const HW_MAX7219_REG_DIGIT_3: u8 = 0x4;
pub const HW_MAX7219_REG_DIGIT_4: u8 = 0x5;
pub const HW_MAX7219_REG_DIGIT_5: u8 = 0x6;
pub const HW_MAX7219_REG_DIGIT_6: u8 = 0x7;
pub const HW_MAX7219_REG_DIGIT_7: u8 = 0x8;
pub const HW_MAX7219_REG_DECODE_MODE: u8 = 0x9;
pub const HW_MAX7219_REG_INTENSITY: u8 = 0xA;
pub const HW_MAX7219_REG_SCAN_LIMIT: u8 = 0xB;
pub const HW_MAX7219_REG_SHUTDOWN: u8 = 0xC;
pub const HW_MAX7219_REG_DISPLAY_TEST: u8 = 0xF;

pub const HW_MAX7219_ADDRESS_ALL: i32 = -1;

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
    WriteRegister(u8, u8),
    WriteRaw([u8; 8]),
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
pub extern "C" fn ceammc_hw_max7219_intensity(mx: *mut hw_max7219, addr: i32, intens: u8) -> bool {
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
/// @param addr - display address in chain
/// @param val - signed int value to display
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_int(mx: *mut hw_max7219, addr: i32, val: i32) -> bool {
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
/// @param addr - display address in chain
/// @param val - unsigned int value to display
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_hex(mx: *mut hw_max7219, addr: i32, val: u32) -> bool {
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

/// write raw data to max7219 register
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param reg - register index
/// @param data - register data
/// @note this is low level write function!
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_reg(
    mx: *mut hw_max7219,
    addr: i32,
    reg: u8,
    data: u8,
) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        let mx = unsafe { &*mx };
        mx.send(addr, Request::WriteRegister(reg, data));
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

/// write data to max7219
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param data - pointer to data
/// @param len - data length
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_bytes(
    mx: *mut hw_max7219,
    addr: i32,
    data: *const u8,
    len: usize,
) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        if data.is_null() {
            error!("NULL data pointer");
            return false;
        }

        let mx = unsafe { &*mx };

        let data = unsafe { from_raw_parts(data, len) };
        let mut buf: [u8; 8] = [0; 8];
        for (a, b) in buf.iter_mut().zip(data.iter()) {
            *a = *b;
        }

        mx.send(addr, Request::WriteRaw(buf));
        true
    });
}

/// write raw bits to max7219
/// @param max7219 - pointer to max7219 struct
/// @param addr - display address in chain
/// @param data - pointer to bit array
/// @param len - data length
#[no_mangle]
pub extern "C" fn ceammc_hw_max7219_write_bits(
    mx: *mut hw_max7219,
    addr: i32,
    bits: *const u8,
    len: usize,
) -> bool {
    rpi_check!({
        if mx.is_null() {
            error!("NULL max7219 pointer");
            return false;
        }

        if bits.is_null() {
            error!("NULL data pointer");
            return false;
        }

        let mx = unsafe { &*mx };

        let bits = unsafe { from_raw_parts(bits, len) };
        let mut buf: [u8; 8] = [0; 8];
        for i in 0..buf.len() {
            for j in 0..8 {
                let idx = i * 8 + j;
                if idx >= len {
                    break;
                }
           
                buf[i] |= (0b1000_0000 >> j) * bits[idx];
            }
        }

        mx.send(addr, Request::WriteRaw(buf));
        true
    });
}
