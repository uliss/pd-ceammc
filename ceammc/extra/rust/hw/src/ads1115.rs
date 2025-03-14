#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
};

use lib_macro::PdError;
use log::error;

use crate::{hw_msg_cb, hw_notify_cb, i2c::I2cAddress, HwThreadWorker, MakePdError};

#[cfg(target_os = "linux")]
mod ads1115_impl;

type Ads1115Worker = HwThreadWorker<Request, Reply>;

#[repr(C)]
pub struct hw_i2c_ads1115_data_cb {
    user: *mut c_void,
    cb_chan: extern "C" fn(user: *mut c_void, chan: u8, data: i16),
    cb_all: extern "C" fn(user: *mut c_void, data: [i16; 4]),
}

pub struct hw_i2c_ads1115 {
    worker: Ads1115Worker,
    on_data: hw_i2c_ads1115_data_cb,
}

#[repr(C)]
#[derive(Debug)]
pub enum hw_i2c_ads1115_measure_mode {
    Single,
    Diff,
}

#[repr(C)]
#[derive(Debug)]
pub enum hw_i2c_ads1115_range {
    Within_0_256V,
    Within_0_512V,
    Within_1_024V,
    Within_2_048V,
    Within_4_096V,
    Within_6_144V,
}

#[derive(Debug)]
pub enum Request {
    MeasureChan(u8),
    MeasureAll,
    SetFullScaleRange(hw_i2c_ads1115_range),
    Poll(bool),
    SetPollTime(u16),
    SetMeasureMode(hw_i2c_ads1115_measure_mode),
}

#[derive(PdError)]
pub enum Reply {
    Error(CString),
    Measure(u8, i16),
    MeasureAll([i16; 4]),
}

#[no_mangle]
pub extern "C" fn ceammc_hw_ads1115_new(
    i2c_bus: i8,
    i2c_addr: i8,
    mode: hw_i2c_ads1115_measure_mode,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
    on_data: hw_i2c_ads1115_data_cb,
) -> *mut hw_i2c_ads1115 {
    rpi_check!(null_mut(), {
        match hw_i2c_ads1115::new(
            i2c_bus,
            I2cAddress::new(i2c_addr),
            mode,
            notify,
            on_err,
            on_data,
        ) {
            Ok(adc) => return Box::into_raw(Box::new(adc)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_ads1115_free(adc: *mut hw_i2c_ads1115) {
    rpi_check!((), {
        if !adc.is_null() {
            drop(unsafe { Box::from_raw(adc) })
        }
    });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_ads1115_measure_all(adc: *mut hw_i2c_ads1115) -> bool {
    rpi_check!({ hw_i2c_ads1115::send_request_ptr(adc, Request::MeasureAll) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_ads1115_measure_chan(adc: *mut hw_i2c_ads1115, chan: u8) -> bool {
    rpi_check!({ hw_i2c_ads1115::send_request_ptr(adc, Request::MeasureChan(chan)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_ads1115_poll(adc: *mut hw_i2c_ads1115, state: bool) -> bool {
    rpi_check!({ hw_i2c_ads1115::send_request_ptr(adc, Request::Poll(state)) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_ads1115_process_reply(adc: *mut hw_i2c_ads1115) -> bool {
    rpi_check!({ hw_i2c_ads1115::process_reply_ptr(adc) });
}

#[no_mangle]
pub extern "C" fn ceammc_hw_ads1115_set_input_range(adc: *mut hw_i2c_ads1115, range: hw_i2c_ads1115_range) -> bool {
    rpi_check!({ hw_i2c_ads1115::send_request_ptr(adc, Request::SetFullScaleRange(range)) });
}
