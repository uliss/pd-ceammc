#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use lib_macro::PdMessage;
use log::error;

use crate::{hw_msg_cb, hw_msg_level, hw_notify_cb, HwThreadWorker, MakePdMessage};

pub const HW_RPI_PWM_MIN_CHAN: i8 = 0x0;
pub const HW_RPI_PWM_MAX_CHAN: i8 = 0x3;
pub const HW_RPI_PWM_NONE_CHAN: i8 = -1;

#[cfg(target_os = "linux")]
mod rpi_pwm_impl;

#[derive(Debug)]
pub enum Request {
    Enable(bool),
    SetFreq(f64, f64),
    SetPeriod(f64),
    SetPolarity(hw_rpi_pwm_polarity),
    SetPulseWidth(f64),
    SetDutyCycle(f64),
    SetPwm(f64, f64),
}

#[derive(Debug, PdMessage)]
pub enum Reply {
    Message(hw_msg_level, CString),
}

type PwmWorker = HwThreadWorker<Request, Reply>;

pub struct hw_rpi_pwm {
    worker: PwmWorker
}

#[derive(Debug)]
#[repr(C)]
pub enum hw_rpi_pwm_polarity {
    NORMAL,
    INVERSE,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_new(
    channel: i8,
    notify: hw_notify_cb,
    on_msg: hw_msg_cb,
) -> *mut hw_rpi_pwm {
    rpi_check!(null_mut(), {
        match hw_rpi_pwm::new(channel, notify, on_msg) {
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
pub extern "C" fn ceammc_hw_rpi_pwm_free(pwm: *mut hw_rpi_pwm) {
    rpi_check!((), {
        if !pwm.is_null() {
            drop(unsafe { Box::from_raw(pwm) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_proc_reply(pwm: *const hw_rpi_pwm) -> bool {
    rpi_check!({ hw_rpi_pwm::process_reply(pwm) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_enable(pwm: *const hw_rpi_pwm, state: bool) -> bool {
    rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::Enable(state)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_set_freq(
    pwm: *const hw_rpi_pwm,
    freq_hz: f64,
    duty_cycle: f64,
) -> bool {
    rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetFreq(freq_hz, duty_cycle)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_set_pwm(
    pwm: *const hw_rpi_pwm,
    period_ms: f64,
    width_ms: f64,
) -> bool {
    rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetPwm(period_ms, width_ms)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_set_period(pwm: *const hw_rpi_pwm, period_ms: f64) -> bool {
    rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetPeriod(period_ms)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_set_pulse_width(pwm: *const hw_rpi_pwm, width_ms: f64) -> bool {
    rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetPulseWidth(width_ms)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_set_duty_cycle(
    pwm: *const hw_rpi_pwm,
    duty_cycle: f64,
) -> bool {
    rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetDutyCycle(duty_cycle)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_set_polarity(
    pwm: *const hw_rpi_pwm,
    polarity: hw_rpi_pwm_polarity,
) -> bool {
    rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetPolarity(polarity)) });
}
