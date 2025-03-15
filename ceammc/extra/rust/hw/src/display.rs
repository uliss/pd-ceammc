#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_char, CStr, CString},
    ptr::null_mut,
    slice::from_raw_parts,
};

use lib_macro::PdError;
use log::error;

use crate::{hw_msg_cb, hw_notify_cb, i2c::I2cAddress, ptr_to_cstr, HwThreadWorker, MakePdMessage, hw_msg_level};

type Ssd1306Worker = HwThreadWorker<Request, Reply>;

#[cfg(target_os = "linux")]
mod ssd1306_impl;

#[derive(Debug)]
#[repr(C)]
pub enum hw_display_rotation {
    ROTATE_0,
    ROTATE_90,
    ROTATE_180,
    ROTATE_270,
}

#[derive(Debug)]
pub enum Request {
    Clear(bool),
    Flush,
    DrawText(CString, i16, i16),
    SetFont(CString),
    Invert(bool),
    Mirror(bool),
    SwitchOn(bool),
    SetPixel(u16, u16, bool),
    SetData(Vec<u8>),
    DrawBitmap(i16, i16, u16, Vec<u8>),
    SetRotation(hw_display_rotation),
    SetBrightness(u8),
}

#[derive(PdError, Debug)]
pub enum Reply {
    Message(hw_msg_level, CString),
}

pub struct hw_display_ssd1306 {
    worker: Ssd1306Worker,
}

pub struct DisplaySpiArgs {
    spi_bus: i8,
    dc_pin: u8,
    cs_pin: u8,
    rs_pin: u8,
    freq: u32,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_new_spi(
    spi_bus: i8,
    dc_pin: u8,
    cs_pin: u8,
    rs_pin: u8,
    freq: u32,
    width: u16,
    height: u16,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_display_ssd1306 {
    rpi_check!(null_mut(), {
        use ssd1306::size::*;

        let args = DisplaySpiArgs {
            spi_bus,
            dc_pin,
            cs_pin,
            rs_pin,
            freq,
            notify,
            on_err,
        };

        match match (width, height) {
            (128, 64) => hw_display_ssd1306::new_spi(args, DisplaySize128x64),
            (128, 32) => hw_display_ssd1306::new_spi(args, DisplaySize128x32),
            (96, 63) => hw_display_ssd1306::new_spi(args, DisplaySize96x16),
            (72, 40) => hw_display_ssd1306::new_spi(args, DisplaySize72x40),
            (64, 48) => hw_display_ssd1306::new_spi(args, DisplaySize64x48),
            (64, 32) => hw_display_ssd1306::new_spi(args, DisplaySize64x32),
            _ => {
                let msg = format!(
                    "unsupported display size: {width}x{height}. Supported size are: 128x64, 128x32, 96x16, 72x40, 64x48, 64x32"
                );
                on_err.exec(msg.as_str());
                return null_mut();
            }
        } {
            Ok(pwm) => return Box::into_raw(Box::new(pwm)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

pub struct DisplayI2cArgs {
    i2c_bus: i8,
    i2c_addr: I2cAddress,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_new_i2c(
    i2c_bus: i8,
    i2c_addr: i8,
    width: u16,
    height: u16,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_display_ssd1306 {
    rpi_check!(null_mut(), {
        use ssd1306::size::*;

        let args = DisplayI2cArgs {
            i2c_bus,
            i2c_addr: I2cAddress::new(i2c_addr),
            notify,
            on_err,
        };

        match match (width, height) {
            (128, 64) => hw_display_ssd1306::new_i2c(args, DisplaySize128x32),
            (128, 32) => hw_display_ssd1306::new_i2c(args, DisplaySize128x32),
            (96, 63) => hw_display_ssd1306::new_i2c(args, DisplaySize96x16),
            (72, 40) => hw_display_ssd1306::new_i2c(args, DisplaySize72x40),
            (64, 48) => hw_display_ssd1306::new_i2c(args, DisplaySize64x48),
            (64, 32) => hw_display_ssd1306::new_i2c(args, DisplaySize64x32),
            _ => {
                let msg = format!(
                    "unsupported display size: {width}x{height}. Supported size are: 128x64, 128x32, 96x16, 72x40, 64x48, 64x32"
                );
                on_err.exec(msg.as_str());
                return null_mut();
            }
        } {
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
pub extern "C" fn ceammc_hw_display_ssd1306_free(display: *mut hw_display_ssd1306) {
    rpi_check!((), {
        if !display.is_null() {
            drop(unsafe { Box::from_raw(display) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_proc_reply(display: *const hw_display_ssd1306) -> bool {
    rpi_check!({ hw_display_ssd1306::process_reply(display) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_text(
    display: *const hw_display_ssd1306,
    txt: *const c_char,
    x: i16,
    y: i16,
) -> bool {
    rpi_check!({
        let txt = unsafe { CStr::from_ptr(txt).to_str().unwrap() };
        let txt = CString::new(txt).unwrap_or_default();
        hw_display_ssd1306::send_request(display, Request::DrawText(txt, x, y))
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_flush(display: *const hw_display_ssd1306) -> bool {
    rpi_check!({ hw_display_ssd1306::send_request(display, Request::Flush) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_clear(
    display: *const hw_display_ssd1306,
    flush: bool,
) -> bool {
    rpi_check!({ hw_display_ssd1306::send_request(display, Request::Clear(flush)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_set_font(
    display: *const hw_display_ssd1306,
    font: *const c_char,
) -> bool {
    rpi_check!({ hw_display_ssd1306::send_request(display, Request::SetFont(ptr_to_cstr(font))) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_invert(
    display: *const hw_display_ssd1306,
    state: bool,
) -> bool {
    rpi_check!({ hw_display_ssd1306::send_request(display, Request::Invert(state)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_mirror(
    display: *const hw_display_ssd1306,
    state: bool,
) -> bool {
    rpi_check!({ hw_display_ssd1306::send_request(display, Request::Mirror(state)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_set_rotation(
    display: *const hw_display_ssd1306,
    rotation: hw_display_rotation,
) -> bool {
    rpi_check!({ hw_display_ssd1306::send_request(display, Request::SetRotation(rotation)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_switch_on(
    display: *const hw_display_ssd1306,
    state: bool,
) -> bool {
    rpi_check!({ hw_display_ssd1306::send_request(display, Request::SwitchOn(state)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_set_pixel(
    display: *const hw_display_ssd1306,
    x: u16,
    y: u16,
    state: bool,
) -> bool {
    rpi_check!({ hw_display_ssd1306::send_request(display, Request::SetPixel(x, y, state)) });
}

/// set display brightness
/// @param display - pointer to hw_display_ssd1306 struct
/// @param level - value in 0..4 range from dimmest to brightest
#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_set_brightness(
    display: *const hw_display_ssd1306,
    level: u8,
) -> bool {
    rpi_check!({ hw_display_ssd1306::send_request(display, Request::SetBrightness(level)) });
}

/// writes raw data to the display
/// @param display - pointer to hw_display_ssd1306 struct
/// @param data - pointer to data (not NULL!)
/// @param len - data size
#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_write_bytes(
    display: *const hw_display_ssd1306,
    data: *const u8,
    len: usize,
) -> bool {
    rpi_check!({
        let data = unsafe { from_raw_parts(data, len) };
        hw_display_ssd1306::send_request(display, Request::SetData(data.to_vec()))
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_display_ssd1306_write_bitmap(
    display: *const hw_display_ssd1306,
    x: i16,
    y: i16,
    width: u16,
    data: *const u8,
    len: usize,
) -> bool {
    rpi_check!({
        let data = unsafe { from_raw_parts(data, len) };
        hw_display_ssd1306::send_request(display, Request::DrawBitmap(x, y, width, data.to_vec()))
    });
}
