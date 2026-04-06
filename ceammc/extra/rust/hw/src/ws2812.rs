#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use lib_macro::PdMessage;
use std::{ffi::CString, ptr::null_mut};

use crate::{
    MakePdMessage, hw_bits, hw_color_rgb8, hw_indexes, hw_msg_cb, hw_msg_level, hw_notify_cb, hw_slice, spi::{hw_spi_bus, hw_spi_cs}
};

// mod led_fx;
#[cfg(target_os = "linux")]
mod ws2812_impl;

#[derive(Debug)]
#[repr(C)]
pub enum hw_led_fx {
    /// The bounce effect will generate a number of balls that bounce up and down the strip
    Bounce,
    /// The breathe effect will generate a single color that fades in and out
    Breathe,
    /// Generates two particles that can collide and bounce or shatter
    Collision,
    /// Rotates around the HSV color space
    Cycle,
    /// Generates the cylon eye effect
    Cylon,
    /// Generates an effect like a flickering flame
    Fire,
    /// Generates a meteor that goes down the strip trailing bits of fading debris
    Meteor,
    /// Static rainbow
    Rainbow,
    /// Strobe light/blinder effect
    Strobe,
    /// Generates random twinkles
    Twinkle,
    /// Generates random sparkles
    SnowSparkle,
}

#[derive(Debug)]
pub enum Request {
    SetPixelColor(hw_color_rgb8, usize),
    SetPixels { colors: Vec<u32>, offset: usize },
    FillSlice(hw_color_rgb8, Option<hw_slice>),
    FillBits(hw_color_rgb8, i32, fixedbitset::FixedBitSet),
    FillPixels(hw_color_rgb8, Vec<i32>),
    Rotate(i32, Option<hw_slice>),
    Clear,
    Flush, // EffectNext,
    SetBrightness(u8),
    ApplyEffect(hw_led_fx, Option<hw_slice>),
    Quit,
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
/// free ws21812 control struct
pub extern "C" fn ceammc_hw_spi_ws2812_free(ws: *mut hw_spi_ws2812) {
    rpi_check!((), {
        if !ws.is_null() {
            drop(unsafe { Box::from_raw(ws) })
        }
    });
}

#[no_mangle]
/// set pixel color
/// @param ws - pointer to led strip handle
/// @param idx - pixel index
/// @param color - pixel color
pub extern "C" fn ceammc_hw_spi_ws2812_set_pixel_color(
    ws: *const hw_spi_ws2812,
    idx: usize,
    color: hw_color_rgb8,
) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::SetPixelColor(color, idx)) });
}

#[no_mangle]
/// set total output brightness
/// @param ws - pointer to led strip handle
/// @param b - target brightness in 0..25 range
pub extern "C" fn ceammc_hw_spi_ws2812_set_brightness(ws: *const hw_spi_ws2812, b: u8) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::SetBrightness(b)) });
}

#[no_mangle]
/// rotate (shift) pixels
/// @param ws - pointer to led strip handle
/// @param delta - shift in steps
/// @param slice - apply to given slice (if NULL: shift all leds)
pub extern "C" fn ceammc_hw_spi_ws2812_rotate(ws: *const hw_spi_ws2812, delta: i32, slice: *const hw_slice) -> bool {
    let slice = if slice.is_null() { None } else { Some(unsafe { *slice }) };
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::Rotate(delta, slice)) });
}

#[no_mangle]
/// write internal buffer to strip
/// @param ws - pointer to led strip handle
pub extern "C" fn ceammc_hw_spi_ws2812_flush(ws: *const hw_spi_ws2812) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::Flush) });
}

#[no_mangle]
/// clear (turn off) the led strip
/// @param ws - pointer to the led strip handle
pub extern "C" fn ceammc_hw_spi_ws2812_clear(ws: *const hw_spi_ws2812) -> bool {
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::Clear) });
}

#[no_mangle]
/// fill leds slice with color
/// @param ws - pointer to the led strip handle
/// @param color - fill color
/// @param slice - apply to given slice (if NULL: set all leds)
pub extern "C" fn ceammc_hw_spi_ws2812_fill_slice(
    ws: *const hw_spi_ws2812,
    color: hw_color_rgb8,
    slice: *const hw_slice,
) -> bool {
    let slice = if slice.is_null() { None } else { Some(unsafe { *slice }) };
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::FillSlice(color, slice),) });
}

#[no_mangle]
/// fill leds addressed by bits with specified color
/// @param ws - pointer to the led strip handle
/// @param color - fill color
/// @param bits - not NULL(!)
pub extern "C" fn ceammc_hw_spi_ws2812_fill_bits(
    ws: *const hw_spi_ws2812,
    color: hw_color_rgb8,
    bits: &hw_bits,
) -> bool {
    if bits.data.is_null() {
        return false;
    }

    let data = unsafe { std::slice::from_raw_parts(bits.data, bits.size as usize) };
    let mut bitset = fixedbitset::FixedBitSet::with_capacity(data.len());
    for (i, b) in data.iter().enumerate() {
        bitset.set(i, *b > 0);
    }

    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::FillBits(color, bits.offset, bitset),) });
}

#[no_mangle]
/// fill leds addressed by pixel indexes with specified color
/// @param ws - pointer to the led strip handle
/// @param color - fill color
/// @param pixels - not NULL(!)
pub extern "C" fn ceammc_hw_spi_ws2812_fill_pixels(
    ws: *const hw_spi_ws2812,
    color: hw_color_rgb8,
    pixels: &hw_indexes,
) -> bool {
    if pixels.data.is_null() {
        return false;
    }

    let data = unsafe { std::slice::from_raw_parts(pixels.data, pixels.size) };
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::FillPixels(color, data.to_vec()),) });
}

/// process events
#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_process_reply(ws: *mut hw_spi_ws2812) {
    rpi_check!((), { hw_spi_ws2812::process_ptr(ws) });
}

/// apply fx to specified slice
#[no_mangle]
pub extern "C" fn ceammc_hw_spi_ws2812_apply_fx(ws: *mut hw_spi_ws2812, fx: hw_led_fx, slice: *const hw_slice) -> bool {
    let slice = if slice.is_null() { None } else { Some(unsafe { *slice }) };
    rpi_check!({ hw_spi_ws2812::send_ptr(ws, Request::ApplyEffect(fx, slice)) });
}

#[no_mangle]
/// write pixels colors into the led strip
/// @param ws - device handle (nullable)
/// @param colors - pointer to pixel colors (RGB24 format)
/// @param length - length of colors array
/// @param offset - offset
pub extern "C" fn ceammc_hw_spi_ws2812_write_pixels(
    ws: *const hw_spi_ws2812,
    colors: *const u32,
    length: usize,
    offset: usize,
) -> bool {
    if colors.is_null() {
        return false;
    }

    let colors = unsafe { std::slice::from_raw_parts(colors, length) };
    rpi_check!({
        hw_spi_ws2812::send_ptr(
            ws,
            Request::SetPixels {
                colors: colors.to_vec(),
                offset,
            },
        )
    });
}
