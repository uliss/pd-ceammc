#![cfg_attr(not(target_os = "linux"), allow(unused_imports))]
#![cfg_attr(not(target_os = "linux"), allow(unused_variables))]
#![cfg_attr(not(target_os = "linux"), allow(dead_code))]
#![allow(non_camel_case_types)]

use std::ffi::CString;

use crate::hw2_msg_cb;

#[cfg(target_os = "linux")]
mod stepper_impl;

#[derive(Debug)]
pub enum Reply {
    Error(CString),
}

#[derive(Debug)]
pub enum Request {
}

pub struct hw_rpi_pwm {
    tx: std::sync::mpsc::Sender<Request>,
    // rx: std::sync::mpsc::Receiver<Reply>,
    on_err: hw2_msg_cb,
}
