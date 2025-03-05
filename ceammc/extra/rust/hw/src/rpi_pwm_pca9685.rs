#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb};

#[cfg(target_os = "linux")]
mod pca9685_impl;

#[derive(Debug)]
pub enum Request {
    Enable(bool),
    SetChanOnOff(u8, u16, u16),
    // SetPeriod(f64),
    // SetPolarity(hw_rpi_pwm_polarity),
    // SetPulseWidth(f64),
    // SetDutyCycle(f64),
    // SetPwm(f64, f64),
}

#[derive(Debug)]
pub enum Reply {
    Error(CString),
}

pub struct hw_rpi_pwm_pca9685 {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_err: hw_msg_cb,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_pca9685_new(
    bus: i8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_rpi_pwm_pca9685 {
    rpi_check!(null_mut(), {
        match hw_rpi_pwm_pca9685::new(bus, notify, on_err) {
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
pub extern "C" fn ceammc_hw_rpi_pwm_pca9685_free(pwm: *mut hw_rpi_pwm_pca9685) {
    rpi_check!((), {
        if !pwm.is_null() {
            drop(unsafe { Box::from_raw(pwm) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_pca9685_proc_reply(pwm: *const hw_rpi_pwm_pca9685) -> bool {
    rpi_check!({ hw_rpi_pwm_pca9685::process_reply(pwm) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_pca9685_enable(
    pwm: *const hw_rpi_pwm_pca9685,
    state: bool,
) -> bool {
    rpi_check!({ hw_rpi_pwm_pca9685::send_request(pwm, Request::Enable(state)) });
}

// #[no_mangle]
// pub extern "C" fn ceammc_hw_rpi_pwm_set_freq(
//     pwm: *const hw_rpi_pwm,
//     freq_hz: f64,
//     duty_cycle: f64,
// ) -> bool {
//     rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetFreq(freq_hz, duty_cycle)) });
// }

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_pca9685_set_on_off(
    pwm: *const hw_rpi_pwm_pca9685,
    chan: u8,
    on: u16,
    off: u16,
) -> bool {
    rpi_check!({ hw_rpi_pwm_pca9685::send_request(pwm, Request::SetChanOnOff(chan, on, off)) });
}

// #[no_mangle]
// pub extern "C" fn ceammc_hw_rpi_pwm_set_period(pwm: *const hw_rpi_pwm, period_ms: f64) -> bool {
//     rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetPeriod(period_ms)) });
// }

// #[no_mangle]
// pub extern "C" fn ceammc_hw_rpi_pwm_set_pulse_width(pwm: *const hw_rpi_pwm, width_ms: f64) -> bool {
//     rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetPulseWidth(width_ms)) });
// }

// #[no_mangle]
// pub extern "C" fn ceammc_hw_rpi_pwm_set_duty_cycle(
//     pwm: *const hw_rpi_pwm,
//     duty_cycle: f64,
// ) -> bool {
//     rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetDutyCycle(duty_cycle)) });
// }

// #[no_mangle]
// pub extern "C" fn ceammc_hw_rpi_pwm_set_polarity(
//     pwm: *const hw_rpi_pwm,
//     polarity: hw_rpi_pwm_polarity,
// ) -> bool {
//     rpi_check!({ hw_rpi_pwm::send_ptr(pwm, Request::SetPolarity(polarity)) });
// }
