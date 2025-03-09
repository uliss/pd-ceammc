use std::{
    ffi::CString,
    os::raw::{c_char, c_void},
};

use log::error;

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

pub fn str_to_cstr<T>(s: T) -> CString
where
    T: Into<Vec<u8>>,
{
    CString::new(s).unwrap_or_default()
}

trait MakePdError<Error> {
    fn pd_err(msg: CString) -> Error;
}

fn send_reply<R>(rep: R, tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb)
{
    if let Err(err) = tx.send(rep) {
        error!("reply send error: {err}");
    } else {
        notify.notify();
    }
}

fn send_error<R>(tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb, msg: &str)
where
    R: MakePdError<R>,
{
    error!("{msg}");
    send_reply(R::pd_err(CString::new(msg).unwrap_or_default()), tx, notify)
}

fn process_err<E, R>(err: E, tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb) -> String
where
    E: std::fmt::Display,
    R: MakePdError<R>,
{
    let str = err.to_string();
    send_error(tx, notify, str.as_str());
    str
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

pub mod dht11;
pub mod display;
pub mod gpio;
pub mod hc_sr04;
pub mod i2c;
pub mod lcd1602;
pub mod max7219;
pub mod rotenc;
pub mod rpi_pwm;
pub mod rpi_pwm_pca9685;
pub mod vl53l0x;
pub mod ws2812;
