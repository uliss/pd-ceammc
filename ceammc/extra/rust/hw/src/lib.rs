use std::{
    ffi::CString,
    os::raw::{c_char, c_void},
};

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
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

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Clone, Copy)]
/// error callback
pub struct hw_msg_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can be NULL
    cb: Option<extern "C" fn(*mut c_void, *const c_char)>,
}

impl hw_msg_cb {
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

macro_rules! return_not_rpi {
    ($x:expr) => {
        #[cfg(not(target_os = "linux"))]
        {
            error!("OS != \"linux\" is not supported");
            return $x;
        }
    };
}

macro_rules! rpi_check {
    ($res:expr, $code:block) => {
        return_not_rpi!($res);

        #[cfg(target_os = "linux")]
        $code
    };
    ($code:block) => {
        return_not_rpi!(false);

        #[cfg(target_os = "linux")]
        return $code;
    };
}

pub mod printers;

#[cfg(feature = "cups")]
pub mod printers_cups;

#[cfg(target_os = "windows")]
pub mod printers_win;

pub mod gpio;
pub mod dht11;
pub mod hc_sr04;
pub mod lcd1602;
pub mod i2c;
pub mod max7219;
