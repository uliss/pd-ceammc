#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use lib_macro::PdMessage;
use rgb::RGB8;

use crate::{
    hw_msg_cb, hw_msg_level, hw_notify_cb,
    max7219::{hw_spi_bus, hw_spi_cs},
    MakePdMessage,
};

mod led_fx;
#[cfg(target_os = "linux")]
mod ws2812_impl;

#[derive(Debug)]
pub struct Slice {
    first: i32,
    last: i32,
    step: usize,
}

#[derive(Debug)]
pub struct Range {
    first: i32,
    length: usize,
}

#[derive(Debug)]
#[repr(C)]
pub enum hw_led_fx {
    Rainbow,
    Breathe,
}

#[derive(Debug)]
pub enum Request {
    SetPixelColor(usize, RGB8),
    SetSliceColor(Slice, RGB8),
    SetRangeColor(Range, RGB8),
    ApplyEffect(Range, hw_led_fx, f32, bool),
    EffectNext,
    Fill(RGB8),
    SetBrightness(u8),
    Flush,
    Clear,
    Rotate(i32),
    Quit(bool),
}

#[derive(Debug, PdMessage)]
pub enum Reply {
    Message(hw_msg_level, CString),
    Done,
}

pub struct hw_spi_ws2812 {
    tx: tokio::sync::mpsc::Sender<Request>,
    rx: tokio::sync::mpsc::Receiver<Reply>,
    on_msg: hw_msg_cb,
    notify: hw_notify_cb,
    clear_on_exit: bool,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_new(
    bus: hw_spi_bus,
    cs: hw_spi_cs,
    size: usize,
    notify: hw_notify_cb,
    on_msg: hw_msg_cb,
    clear_on_exit: bool,
) -> *mut hw_spi_ws2812 {
    rpi_check!(null_mut(), {
        match hw_spi_ws2812::new(bus, cs, size, notify, on_msg, clear_on_exit) {
            Ok(pwm) => return Box::into_raw(Box::new(pwm)),
            Err(err) => {
                if !err.is_empty() {
                    on_msg.error_cstr(err);
                }
                return null_mut();
            }
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_free(ws: *mut hw_spi_ws2812) {
    rpi_check!((), {
        if !ws.is_null() {
            drop(unsafe { Box::from_raw(ws) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_set_color(pwm: *const hw_spi_ws2812, idx: usize, r: u8, g: u8, b: u8) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(pwm, Request::SetPixelColor(idx, RGB8 { r, g, b })) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_set_brightness(ws: *const hw_spi_ws2812, b: u8) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::SetBrightness(b)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_rotate(ws: *const hw_spi_ws2812, delta: i32) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::Rotate(delta)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_flush(ws: *const hw_spi_ws2812) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::Flush) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_clear(ws: *const hw_spi_ws2812) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::Clear) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_fill(ws: *const hw_spi_ws2812, r: u8, g: u8, b: u8) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::Fill(RGB8 { r, g, b })) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_set_range(
    ws: *const hw_spi_ws2812,
    start: i32,
    len: usize,
    r: u8,
    g: u8,
    b: u8,
) -> bool {
    rpi_check!({
        hw_spi_ws2812::send_ptr(
            ws,
            Request::SetRangeColor(
                Range {
                    first: start,
                    length: len,
                },
                RGB8 { r, g, b },
            ),
        )
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_set_slice(
    ws: *const hw_spi_ws2812,
    first: i32,
    last: i32,
    step: usize,
    r: u8,
    g: u8,
    b: u8,
) -> bool {
    rpi_check!({
        hw_spi_ws2812::send_ptr(
            ws,
            Request::SetSliceColor(Slice { first, last, step }, RGB8 { r, g, b }),
        )
    });
}

/// process events
#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_process_reply(ws: *mut hw_spi_ws2812) {
    rpi_check!((), { hw_spi_ws2812::process_ptr(ws) });
}

/// apply fx
#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_apply_fx(
    ws: *mut hw_spi_ws2812,
    first: i32,
    length: usize,
    fx: hw_led_fx,
    arg: f32,
) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::ApplyEffect(Range { first, length }, fx, arg, true),) });
}

/// calc next fx
#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_fx_next(ws: *mut hw_spi_ws2812) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::EffectNext) });
}
