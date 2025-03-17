#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{f32, ffi::CString, ptr::null_mut};

use lib_macro::PdMessage;
use log::error;

use crate::{
    hw_msg_cb, hw_msg_level, hw_notify_cb, i2c::I2cAddress, rpi_pwm::hw_rpi_pwm_polarity,
    HwThreadWorker, MakePdMessage,
};

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
    SetChanDutyCycle(u8, f32, Option<f32>),
    SetChanConst(u8, bool, f32),
}

#[derive(Debug, PdMessage)]
pub enum Reply {
    Message(hw_msg_level, CString),
}

type Pca9685Worker = HwThreadWorker<Request, Reply>;

pub struct hw_pca9685 {
    worker: Pca9685Worker,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_new(
    i2c_bus: i8,
    i2c_addr: i8,
    notify: hw_notify_cb,
    on_msg: hw_msg_cb,
) -> *mut hw_pca9685 {
    rpi_check!(null_mut(), {
        match hw_pca9685::new(i2c_bus, I2cAddress::new(i2c_addr), notify, on_msg) {
            Ok(pwm) => return Box::into_raw(Box::new(pwm)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_msg.error_cstr(err);
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
    rpi_check!({ hw_pca9685::process_reply_ptr(pwm) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_enable(pwm: *const hw_pca9685, state: bool) -> bool {
    rpi_check!({ hw_pca9685::send_request_ptr(pwm, Request::Enable(state)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_freq(pwm: *const hw_pca9685, freq_hz: f32) -> bool {
    rpi_check!({ hw_pca9685::send_request_ptr(pwm, Request::SetFreq(freq_hz)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_on_off(
    pwm: *const hw_pca9685,
    chan: u8,
    on: u16,
    off: u16,
) -> bool {
    rpi_check!({ hw_pca9685::send_request_ptr(pwm, Request::SetChanOnOff(chan, on, off)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_period(pwm: *const hw_pca9685, period_ms: f32) -> bool {
    rpi_check!({ hw_pca9685::send_request_ptr(pwm, Request::SetPeriod(period_ms)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_pulse_width(
    pwm: *const hw_pca9685,
    chan: u8,
    width_ms: f32,
    phase: f32,
) -> bool {
    rpi_check!({
        hw_pca9685::send_request_ptr(pwm, Request::SetChanPulseWidth(chan, width_ms, phase))
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_const(
    pwm: *const hw_pca9685,
    chan: u8,
    value: bool,
    delay: f32,
) -> bool {
    rpi_check!({ hw_pca9685::send_request_ptr(pwm, Request::SetChanConst(chan, value, delay)) });
}

/// set duty cycle
/// @param pwm - pointer to pca9685 struct (nullable)
/// @param chan - target PWM channel
/// @param duty_cycle in 0.0-1.0 range
/// @param phase - pointer to phase offset (nullable)
///
/// @note if phase is NULL and duty_cycle = 1.0 - turns PWM always on
#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685_set_duty_cycle(
    pwm: *const hw_pca9685,
    chan: u8,
    duty_cycle: f32,
    phase: *const f32,
) -> bool {
    rpi_check!({
        hw_pca9685::send_request_ptr(
            pwm,
            Request::SetChanDutyCycle(
                chan,
                duty_cycle,
                if phase.is_null() {
                    None
                } else {
                    Some(unsafe { *phase })
                },
            ),
        )
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_pca9685i_set_polarity(
    pwm: *const hw_pca9685,
    polarity: hw_rpi_pwm_polarity,
) -> bool {
    rpi_check!({ hw_pca9685::send_request_ptr(pwm, Request::SetPolarity(polarity)) });
}
