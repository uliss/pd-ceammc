use std::{
    ffi::CString,
    os::raw::{c_char, c_void},
};

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct hw_notify_cb {
    /// dispatcher ID
    id: usize,
    /// dispatcher callback (not NULL!)
    f: extern "C" fn(id: usize),
}

impl hw_notify_cb {
    fn notify(&self) {
        (self.f)(self.id);
    }
}

pub mod gamepad;
pub mod printers;

#[repr(C)]
#[allow(non_camel_case_types)]
/// error callback
pub struct hw_error_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can be NULL
    cb: Option<extern "C" fn(*mut c_void, *const c_char)>,
}

impl hw_error_cb {
    pub fn exec(&self, msg: &str) {
        self.cb.map(|f| {
            let msg = CString::new(msg).unwrap_or_default();
            f(self.user, msg.as_ptr());
        });
    }

    pub fn exec_raw(&self, msg: *const c_char) {
        self.cb.map(|f| {
            if !msg.is_null() {
                f(self.user, msg);
            }
        });
    }
}

#[cfg(target_family = "unix")]
pub mod printers_cups;

#[cfg(target_os = "windows")]
pub mod printers_win;