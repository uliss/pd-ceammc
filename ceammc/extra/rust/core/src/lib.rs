pub mod bitmap;
pub mod mdns_sd;
pub mod net_iface;

use env_logger;

use std::{
    ffi::{c_char, c_void, CString},
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
