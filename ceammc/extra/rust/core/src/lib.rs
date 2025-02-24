pub mod bitmap;
pub mod mdns_sd;
pub mod net_iface;

use env_logger;
use log::error;

use std::{
    ffi::{c_char, c_void, CStr, CString},
    slice::from_raw_parts,
    sync::Once,
};
static LOG_INIT: Once = Once::new();

/// init rust env_logger
/// logger config is done with a RUST_LOG env variable
#[no_mangle]
pub extern "C" fn ceammc_rust_log_init() {
    LOG_INIT.call_once(|| {
        env_logger::builder().format_timestamp(None).init();
    });
}

#[allow(non_camel_case_types)]
#[derive(Clone)]
#[repr(C)]
pub struct core_on_msg {
    user: *mut c_void,
    // not NULL
    cb: extern "C" fn(user: *mut c_void, msg: *const c_char),
}

impl core_on_msg {
    fn exec_raw(&self, msg: &CString) {
        (self.cb)(self.user, msg.as_ptr());
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
pub struct core_notify {
    /// dispatcher ID
    id: usize,
    /// dispatcher callback (not NULL!)
    f: extern "C" fn(id: usize),
}

impl core_notify {
    fn notify(&self) {
        (self.f)(self.id);
    }
}

fn cstr_to_string(cstr: *const c_char) -> String {
    unsafe { CStr::from_ptr(cstr) }
        .to_str()
        .unwrap_or_else(|err| {
            error!("{err}");
            ""
        })
        .to_owned()
}

fn data_to_vec<T>(data: *const T, len: usize) -> Vec<T>
where
    T: Clone,
{
    if data.is_null() {
        error!("NULL data pointer");
        return vec![];
    } else {
        unsafe { from_raw_parts(data, len) }.to_vec()
    }
}
