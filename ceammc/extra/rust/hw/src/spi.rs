#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use ceammc_rs_msg::{msg_cb, msg_notify};
use std::{ffi::c_void, ptr::null_mut};

#[cfg(target_os = "linux")]
pub mod spi_impl;

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub enum hw_spi_bus {
    NONE = -1,
    SPI0,
    SPI1,
    SPI2,
    SPI3,
    SPI4,
    SPI5,
    SPI6,
}

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub enum hw_spi_cs {
    CS0,
    CS1,
    CS2,
    CS3,
}

#[derive(Debug)]
pub enum Request {
    LoopbackTest,
    Transfer { tx_data: Vec<u8>, rx_size: usize },
}

#[derive(Debug)]
pub enum Reply {
    Loopback(bool),
    ReceivedData(Vec<u8>),
}

#[repr(C)]
pub struct hw_spi_cb {
    user: *mut c_void,
    loopback_cb: extern "C" fn(user: *mut c_void, result: bool),
    received_cb: extern "C" fn(user: *mut c_void, data: *const u8, size: usize),
}

impl hw_spi_cb {
    pub fn loopback(&self, result: bool) {
        (self.loopback_cb)(self.user, result)
    }

    pub fn received(&self, data: &Vec<u8>) {
        (self.received_cb)(self.user, data.as_ptr(), data.len())
    }
}

#[allow(non_camel_case_types)]

pub struct hw_spi {
    obj: ceammc_rs_msg::Client<Request, Reply>,
    cb: hw_spi_cb,
}

#[no_mangle]
/// create new spi device
/// @return device handle or nullptr
/// @param bus - spi bus
/// @param cs - GPIO chip select pin
/// @param notify - notify pd caller
/// @param on_msg - on message from worker callback
/// @param on_data - on data from worker callback
pub extern "C" fn ceammc_hw_spi_new(
    bus: hw_spi_bus,
    cs: hw_spi_cs,
    notify: msg_notify,
    on_msg: msg_cb,
    on_data: hw_spi_cb,
) -> *mut hw_spi {
    rpi_check!(null_mut(), {
        match hw_spi::new(bus, cs, notify, on_msg, on_data) {
            Ok(spi) => return Box::into_raw(Box::new(spi)),
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
/// free spi handle
/// @param spi - device handle (nullable)
pub extern "C" fn ceammc_hw_spi_free(spi: *mut hw_spi) {
    rpi_check!((), {
        if !spi.is_null() {
            drop(unsafe { Box::from_raw(spi) })
        }
    });
}

#[no_mangle]
/// process all available data from spi worker
/// @param spi - spi pointer (nullable)
pub extern "C" fn ceammc_hw_spi_process_reply(spi: *mut hw_spi) -> bool {
    rpi_check!(false, {
        return hw_spi::process_reply_ptr(spi);
    });
}

#[no_mangle]
/// request loopback test
/// @param spi - device handle (nullable)
pub extern "C" fn ceammc_hw_spi_loopback_test(spi: *mut hw_spi) -> bool {
    rpi_check!(false, {
        return hw_spi::send_request(spi, Request::LoopbackTest);
    });
}

#[no_mangle]
/// transfer bytes
/// @param spi - device handle (nullable)
/// @param rx_size - received size
/// @param tx_data - pointer to data (nullable)
/// @param len - data length
pub extern "C" fn ceammc_hw_spi_transfer(spi: *mut hw_spi, rx_size: usize, tx_data: *const u8, len: usize) -> bool {
    rpi_check!(false, {
        if tx_data.is_null() {
            return false;
        }

        let tx_data = unsafe { std::slice::from_raw_parts(tx_data, len) };
        return hw_spi::send_request(
            spi,
            Request::Transfer {
                tx_data: tx_data.to_vec(),
                rx_size: rx_size,
            },
        );
    });
}
