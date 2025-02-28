#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use log::error;

use crate::{
    hw_msg_cb, hw_notify_cb,
    max7219::{hw_spi_bus, hw_spi_cs},
};

#[cfg(target_os = "linux")]
mod ws2812_impl;

#[derive(Debug)]
pub enum Request {
    SetColorRGB(usize, u8, u8, u8),
    SetBrightness(u8),
    Flush,
    Rotate(i32),
}

#[derive(Debug)]
pub enum Reply {
    Error(CString),
}

pub struct hw_spi_ws2812 {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_err: hw_msg_cb,
    notify: hw_notify_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_new(
    bus: hw_spi_bus,
    cs: hw_spi_cs,
    size: usize,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_spi_ws2812 {
    rpi_check!(null_mut(), {
        match hw_spi_ws2812::new(bus, cs, size, notify, on_err) {
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
pub extern "C" fn ceammc_hw_spi_ws2812_free(pwm: *mut hw_spi_ws2812) {
    rpi_check!((), {
        if !pwm.is_null() {
            drop(unsafe { Box::from_raw(pwm) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_set_color(
    pwm: *const hw_spi_ws2812,
    idx: usize,
    r: u8,
    g: u8,
    b: u8,
) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(pwm, Request::SetColorRGB(idx, r, g, b)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_set_brightness(pwm: *const hw_spi_ws2812, b: u8) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(pwm, Request::SetBrightness(b)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_rotate(pwm: *const hw_spi_ws2812, delta: i32) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(pwm, Request::Rotate(delta)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_flush(pwm: *const hw_spi_ws2812) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(pwm, Request::Flush) });
}
