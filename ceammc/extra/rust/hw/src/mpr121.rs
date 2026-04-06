#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use crate::{i2c::I2cAddress, HwThreadWorker, MakePdMessage};
use ceammc_rs_msg::{msg_cb, msg_level, msg_notify};
use lib_macro::PdMessage;
use log::error;
use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
};

#[cfg(target_os = "linux")]
mod mpr121_impl;

#[derive(Debug)]
pub enum Request {
    ReadAll,
    Reset,
    SetThresholds(u8, u8),
    SetDebounce(u8, u8),
    GetFiltered(u8),
    GetBaseline(u8),
}

/// max debounce count
pub const HW_RPI_MPR121_DEBOUNCE_MAX: u8 = 7;
/// default touch threshold
pub const HW_RPI_MPR121_DEFAULT_TOUCH_THRESHOLD: u8 = 12;
/// default release threshold
pub const HW_RPI_MPR121_DEFAULT_RELEASE_THRESHOLD: u8 = 6;

#[derive(Debug, PdMessage)]
pub enum Reply {
    Message(msg_level, CString),
    AllTouches {
        touched: u16,
        previous: u16,
        over_current: bool,
    },
    Filtered {
        value: u16,
        channel: u8,
    },
    Baseline {
        value: u8,
        channel: u8,
    },
    InvalidDevice,
}

type Mpr212SensorWorker = HwThreadWorker<Request, Reply>;

pub struct hw_sensor_mpr121 {
    worker: Mpr212SensorWorker,
    cb: hw_mpr121_reply_cb,
}

#[repr(C)]
pub struct hw_mpr121_reply_cb {
    user: *mut c_void,
    on_touch: extern "C" fn(user: *mut c_void, touched: u16, previous: u16, over_current: bool),
    on_baseline: extern "C" fn(user: *mut c_void, channel: u8, data: u8),
    on_filtered: extern "C" fn(user: *mut c_void, channel: u8, data: u16),
    on_disconnect: extern "C" fn(user: *mut c_void),
}

impl hw_mpr121_reply_cb {
    pub(crate) fn all_touches(&self, touched: u16, previous: u16, over_current: bool) {
        (self.on_touch)(self.user, touched, previous, over_current)
    }

    pub(crate) fn filtered(&self, channel: u8, data: u16) {
        (self.on_filtered)(self.user, channel, data)
    }

    pub(crate) fn baseline(&self, channel: u8, data: u8) {
        (self.on_baseline)(self.user, channel, data)
    }

    pub(crate) fn disconnect(&self) {
        (self.on_disconnect)(self.user)
    }
}

#[no_mangle]
/// create new mpr121 device handle
/// @param i2c_bus - i2c bus
/// @param i2c_addr - i2c address
/// @param caller notify callback
/// @param on_msg - on message callback
/// @param on_reply - on worker data callback
/// @return pointer to device handle or NULL on error
pub extern "C" fn ceammc_hw_sensor_mpr121_new(
    i2c_bus: i8,
    i2c_addr: i8,
    irq_pin: *const u8,
    notify: msg_notify,
    on_msg: msg_cb,
    on_reply: hw_mpr121_reply_cb,
) -> *mut hw_sensor_mpr121 {
    rpi_check!(null_mut(), {
        match hw_sensor_mpr121::new(i2c_bus, I2cAddress::new(i2c_addr), irq_pin, notify, on_msg, on_reply) {
            Ok(ir) => return Box::into_raw(Box::new(ir)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_msg.error_cstr(err);
                return null_mut();
            }
        }
    });
}

#[no_mangle]
/// free mpr121 device handle
/// @param mpr - device handle, nullable
pub extern "C" fn ceammc_hw_sensor_mpr121_free(mpr: *mut hw_sensor_mpr121) {
    rpi_check!((), {
        if !mpr.is_null() {
            hw_sensor_mpr121::free(*unsafe { Box::from_raw(mpr) });
        }
    });
}

#[no_mangle]
/// process mpr121 replies
/// @param mpr - device handle, nullable
pub extern "C" fn ceammc_hw_sensor_mpr121_proc_reply(mpr: *const hw_sensor_mpr121) -> bool {
    rpi_check!({ hw_sensor_mpr121::process_reply(mpr) });
}

#[no_mangle]
/// single request to read all touches
/// @param mpr - device handle, nullable
pub extern "C" fn ceammc_hw_sensor_mpr121_readall(mpr: *const hw_sensor_mpr121) -> bool {
    rpi_check!({ hw_sensor_mpr121::send_request(mpr, Request::ReadAll) });
}

#[no_mangle]
/// performs a software reset on the device, resetting the MPR121 Touch sensor back to default configuration
/// @param mpr - device handle, nullable
pub extern "C" fn ceammc_hw_sensor_mpr121_reset(mpr: *const hw_sensor_mpr121) -> bool {
    rpi_check!({ hw_sensor_mpr121::send_request(mpr, Request::Reset) });
}

#[no_mangle]
/// Set the touch and release threshold for all channels. Usually the touch threshold is a little bigger than the release threshold. This creates some debounce characteristics. The correct thresholds depend on the application.
/// @param mpr - device handle, nullable
/// @param on - touch threshold
/// @param off - release threshold
pub extern "C" fn ceammc_hw_sensor_mpr121_set_thresholds(mpr: *const hw_sensor_mpr121, on: u8, off: u8) -> bool {
    rpi_check!({ hw_sensor_mpr121::send_request(mpr, Request::SetThresholds(on, off)) });
}

#[no_mangle]
/// Sets the count for both touch and release. See 5.7 of the Mpr121 Data Sheet.
/// @param mpr - device handle, nullable
pub extern "C" fn ceammc_hw_sensor_mpr121_set_debounce(mpr: *const hw_sensor_mpr121, on: u8, off: u8) -> bool {
    rpi_check!({ hw_sensor_mpr121::send_request(mpr, Request::SetDebounce(on, off)) });
}

#[no_mangle]
/// Reads the filtered data from touch channels. Noise gets filtered out by the chip. See 5.3 in the data sheet.
/// Note that the resulting value is only 10bit wide.
/// @param mpr - device handle, nullable
pub extern "C" fn ceammc_hw_sensor_mpr121_get_filtered(mpr: *const hw_sensor_mpr121, channel: u8) -> bool {
    rpi_check!({ hw_sensor_mpr121::send_request(mpr, Request::GetFiltered(channel)) });
}

#[no_mangle]
/// Reads the baseline data for the channel. Note that this has only a resolution of 8bit.
/// @param mpr - device handle, nullable
pub extern "C" fn ceammc_hw_sensor_mpr121_get_baseline(mpr: *const hw_sensor_mpr121, channel: u8) -> bool {
    rpi_check!({ hw_sensor_mpr121::send_request(mpr, Request::GetBaseline(channel)) });
}
