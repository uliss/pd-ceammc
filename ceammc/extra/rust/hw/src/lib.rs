use std::{
    ffi::{CStr, CString},
    os::raw::{c_char, c_void},
};

use log::{debug, error};

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

pub fn ptr_to_cstr(s: *const c_char) -> CString {
    if s.is_null() {
        CString::new("").unwrap_or_default()
    } else {
        unsafe { CStr::from_ptr(s) }.to_owned()
    }
}

pub trait MakePdError<Error> {
    fn pd_err(msg: CString) -> Error;
}

fn send_reply<R>(rep: R, tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb) -> bool {
    if let Err(err) = tx.send(rep) {
        error!("reply send error: {err}");
        false
    } else {
        notify.notify();
        true
    }
}

fn send_error<R>(tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb, msg: &str) -> bool
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

pub struct HwThreadWorker<Request, Reply> {
    rx: std::sync::mpsc::Receiver<Reply>,
    tx: std::sync::mpsc::Sender<Request>,
    on_err: hw_msg_cb,
}

impl<Request, Reply> HwThreadWorker<Request, Reply>
where
    Request: Send,
    Reply: MakePdError<Reply>,
{
    pub fn new(
        on_err: hw_msg_cb,
    ) -> (
        Self,
        std::sync::mpsc::Receiver<Request>,
        std::sync::mpsc::Sender<Reply>,
    ) {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        (
            Self {
                rx: rep_rx,
                tx: req_tx,
                on_err,
            },
            req_rx,
            rep_tx,
        )
    }

    pub fn worker_error(&self, str: &str) {
        error!("worker error {str}");
    }

    pub fn spawn<F>(&self, tx: std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb, fx: F)
    where
        F: FnOnce() -> Result<(), String>,
        F: Send + 'static,
        Reply: Send + 'static,
    {
        std::thread::spawn(move || {
            debug!("worker thread start");

            if let Err(err) = fx() {
                process_err(format!("worker error: {err}"), &tx, notify);
            }

            debug!("worker thread done");
        });
    }

    pub fn caller_error(&self, msg: &CString) {
        self.on_err.exec_raw(msg.as_ptr());
    }

    pub fn send_request(&self, req: Request) -> bool {
        if let Err(err) = self.tx.send(req) {
            self.on_err.exec(err.to_string().as_str());
            false
        } else {
            true
        }
    }

    pub fn send_request_ptr(x: *const Self, req: Request) -> bool {
        if x.is_null() {
            error!("NULL pointer");
            false
        } else {
            let x = unsafe { &*x };

            x.send_request(req)
        }
    }

    pub fn process_reply(&self, fx: &dyn Fn(Reply) -> ()) -> bool {
        if let Ok(rep) = self.rx.try_recv() {
            fx(rep)
        }

        true
    }

    pub fn process_reply_ptr(x: *const Self, fx: &dyn Fn(Reply) -> ()) -> bool {
        if x.is_null() {
            error!("NULL pointer");
            false
        } else {
            let x = unsafe { &*x };

            x.process_reply(fx)
        }
    }
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
