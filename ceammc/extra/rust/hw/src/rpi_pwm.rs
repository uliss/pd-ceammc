#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{ffi::CString, ptr::null_mut};

use log::error;

use crate::{hw_msg_cb, hw_notify_cb};

#[cfg(target_os = "linux")]
mod rpi_pwm_impl;

#[derive(Debug)]
pub enum Request {
    Enable(bool),
    SetFreq(f64, f64),
    SetPeriod(f64),
    SetPolarity(hw_rpi_pwm_polarity),
    SetPulseWidth(f64),
}

#[derive(Debug)]
pub enum Reply {
    Error(CString),
}

pub struct hw_rpi_pwm {
    tx: std::sync::mpsc::Sender<Request>,
    rx: std::sync::mpsc::Receiver<Reply>,
    on_err: hw_msg_cb,
    notify: hw_notify_cb,
}

#[derive(Debug)]
#[repr(C)]
pub enum hw_rpi_pwm_polarity {
    NORMAL,
    INVERSE,
}

#[no_mangle]
pub extern "C" fn ceammc_hw_rpi_pwm_new(
    channel: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
) -> *mut hw_rpi_pwm {
    rpi_check!(null_mut(), {
        match hw_rpi_pwm::new(channel, notify, on_err) {
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
pub extern "C" fn ceammc_hw_rpi_pwm_free(pwm: *mut hw_rpi_pwm) {
    rpi_check!((), {
        if !pwm.is_null() {
            drop(unsafe { Box::from_raw(pwm) })
        }
    });
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

// #[no_mangle]
// pub extern "C" fn ceammc_hw_gpio_sr04_process(sr04: *const hw_gpio_sr04) -> bool {
//     rpi_check!({ hw_gpio_sr04::check_result_ptr(sr04) });
// }
