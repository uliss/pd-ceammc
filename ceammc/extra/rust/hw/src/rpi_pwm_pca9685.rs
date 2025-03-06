#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb, rpi_pwm::hw_rpi_pwm_polarity};

#[cfg(target_os = "linux")]
mod pca9685_impl;

// 255 = round(osc_value / (4096 * 24)) - 1
pub const HW_PCA9685_OSC_VALUE: u32 = 256 * 24;
pub const HW_PCA9685_MIN_FREQ_HZ: u32 = HW_PCA9685_OSC_VALUE / 255;
pub const HW_PCA9685_MAX_FREQ_HZ: u32 = HW_PCA9685_OSC_VALUE / 3;
pub const HW_PCA9685_MIN_PERIOD_MS: f32 = 1000.0 / HW_PCA9685_MAX_FREQ_HZ as f32;
pub const HW_PCA9685_MAX_PERIOD_MS: f32 = 1000.0 / HW_PCA9685_MIN_FREQ_HZ as f32;
pub const HW_PCA9685_ALL_CHAN: u8 = 16;

#[derive(Debug)]
pub enum Request {
    Enable(bool),
    SetChanOnOff(u8, u16, u16),
    SetFreq(f32),
    SetPeriod(f32),
    SetPolarity(hw_rpi_pwm_polarity),
    SetChanPulseWidth(u8, f32, f32),
    SetChanDutyCycle(u8, f32, f32),
    // SetPwm(u8, f64),
}

#[derive(Debug)]
pub enum Reply {
    Error(CString),
}

pub struct hw_pca9685 {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_new(
    bus: i8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_pca9685 {
    rpi_check!(null_mut(), {
        match hw_pca9685::new(bus, notify, on_err) {
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
pub extern "C" fn ceammc_hw_pca9685_free(pwm: *mut hw_pca9685) {
    rpi_check!((), {
        if !pwm.is_null() {
            drop(unsafe { Box::from_raw(pwm) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_proc_reply(pwm: *const hw_pca9685) -> bool {
    rpi_check!({ hw_pca9685::process_reply(pwm) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_enable(pwm: *const hw_pca9685, state: bool) -> bool {
    rpi_check!({ hw_pca9685::send_request(pwm, Request::Enable(state)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_freq(pwm: *const hw_pca9685, freq_hz: f32) -> bool {
    rpi_check!({ hw_pca9685::send_request(pwm, Request::SetFreq(freq_hz)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_on_off(
    pwm: *const hw_pca9685,
    chan: u8,
    on: u16,
    off: u16,
) -> bool {
    rpi_check!({ hw_pca9685::send_request(pwm, Request::SetChanOnOff(chan, on, off)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_period(pwm: *const hw_pca9685, period_ms: f32) -> bool {
    rpi_check!({ hw_pca9685::send_request(pwm, Request::SetPeriod(period_ms)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_pulse_width(pwm: *const hw_pca9685, chan: u8, width_ms: f32, phase: f32) -> bool {
    rpi_check!({ hw_pca9685::send_request(pwm, Request::SetChanPulseWidth(chan, width_ms, phase)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_duty_cycle(
    pwm: *const hw_pca9685,
    chan: u8,
    duty_cycle: f32,
    phase: f32,
) -> bool {
    rpi_check!({ hw_pca9685::send_request(pwm, Request::SetChanDutyCycle(chan, duty_cycle, phase)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685i_set_polarity(
    pwm: *const hw_pca9685,
    polarity: hw_rpi_pwm_polarity,
) -> bool {
    rpi_check!({ hw_pca9685::send_request(pwm, Request::SetPolarity(polarity)) });
}
