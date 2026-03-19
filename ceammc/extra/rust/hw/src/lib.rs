use std::{
    ffi::{CStr, CString},
    os::raw::{c_char, c_void},
};

use log::{debug, error, info};

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

#[derive(Debug, Clone, Copy)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub enum hw_msg_level {
    Error,
    Debug,
    Info,
}

#[repr(C)]
#[allow(non_camel_case_types)]
#[derive(Clone, Copy)]
/// error callback
pub struct hw_msg_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can be NULL
    cb: Option<extern "C" fn(user: *mut c_void, level: hw_msg_level, cb: *const c_char)>,
}

impl hw_msg_cb {
    pub fn exec(&self, level: hw_msg_level, msg: &str) {
        self.cb.map(|f| {
            let msg = CString::new(msg).unwrap_or_default();
            f(self.user, level, msg.as_ptr());
        });
    }

    pub fn error(&self, msg: &str) {
        self.exec(hw_msg_level::Error, msg)
    }

    pub fn debug(&self, msg: &str) {
        self.exec(hw_msg_level::Debug, msg)
    }

    pub fn info(&self, msg: &str) {
        self.exec(hw_msg_level::Info, msg)
    }

    pub fn exec_raw(&self, level: hw_msg_level, msg: *const c_char) {
        self.cb.map(|f| {
            if !msg.is_null() {
                f(self.user, level, msg);
            }
        });
    }

    pub fn error_raw(&self, msg: *const c_char) {
        self.exec_raw(hw_msg_level::Error, msg)
    }

    pub fn debug_raw(&self, msg: *const c_char) {
        self.exec_raw(hw_msg_level::Debug, msg)
    }

    pub fn info_raw(&self, msg: *const c_char) {
        self.exec_raw(hw_msg_level::Info, msg)
    }

    pub fn error_cstr(&self, msg: CString) {
        self.exec_raw(hw_msg_level::Error, msg.as_ptr())
    }

    pub fn debug_cstr(&self, msg: CString) {
        self.exec_raw(hw_msg_level::Debug, msg.as_ptr())
    }

    pub fn info_cstr(&self, msg: CString) {
        self.exec_raw(hw_msg_level::Info, msg.as_ptr())
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

pub trait MakePdMessage<Message> {
    fn pd_error(msg: CString) -> Message;
    fn pd_debug(msg: CString) -> Message;
    fn pd_info(msg: CString) -> Message;
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
    R: MakePdMessage<R>,
{
    error!("{msg}");
    send_reply(R::pd_error(CString::new(msg).unwrap_or_default()), tx, notify)
}

#[allow(dead_code)]
fn send_debug<R>(tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb, msg: &str) -> bool
where
    R: MakePdMessage<R>,
{
    debug!("{msg}");
    send_reply(R::pd_debug(CString::new(msg).unwrap_or_default()), tx, notify)
}

#[allow(dead_code)]
fn send_info<R>(tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb, msg: &str) -> bool
where
    R: MakePdMessage<R>,
{
    info!("{msg}");
    send_reply(R::pd_info(CString::new(msg).unwrap_or_default()), tx, notify)
}

fn process_err<E, R>(err: E, tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb) -> String
where
    E: std::fmt::Display,
    R: MakePdMessage<R>,
{
    let str = err.to_string();
    send_error(tx, notify, str.as_str());
    str
}

#[allow(dead_code)]
fn process_debug<D, R>(msg: D, tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb) -> String
where
    D: std::fmt::Display,
    R: MakePdMessage<R>,
{
    let str = msg.to_string();
    send_debug(tx, notify, str.as_str());
    str
}

#[allow(dead_code)]
fn process_info<D, R>(msg: D, tx: &std::sync::mpsc::Sender<R>, notify: hw_notify_cb) -> String
where
    D: std::fmt::Display,
    R: MakePdMessage<R>,
{
    let str = msg.to_string();
    send_debug(tx, notify, str.as_str());
    str
}

macro_rules! return_not_rpi {
    ($x:expr) => {
        #[cfg(not(target_os = "linux"))]
        {
            log::error!("OS != \"linux\" is not supported");
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
    on_msg: hw_msg_cb,
}

impl<Request, Reply> HwThreadWorker<Request, Reply>
where
    Request: Send,
    Reply: MakePdMessage<Reply>,
{
    pub fn new(on_msg: hw_msg_cb) -> (Self, std::sync::mpsc::Receiver<Request>, std::sync::mpsc::Sender<Reply>) {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        (
            Self {
                rx: rep_rx,
                tx: req_tx,
                on_msg,
            },
            req_rx,
            rep_tx,
        )
    }

    pub fn worker_error(&self, str: &str, tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb) {
        error!("worker error {str}");
        process_err(format!("worker error: {str}"), tx, notify);
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

    // should be called only in the main caller thread!
    pub fn pd_message(&self, level: hw_msg_level, msg: &CString) {
        self.on_msg.exec_raw(level, msg.as_ptr());
    }

    pub fn send_request(&self, req: Request) -> bool {
        if let Err(err) = self.tx.send(req) {
            self.on_msg.exec(hw_msg_level::Error, err.to_string().as_str());
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
        while let Ok(rep) = self.rx.try_recv() {
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

#[derive(Debug)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct hw_color_rgb8 {
    red: u8,
    green: u8,
    blue: u8,
}

impl hw_color_rgb8 {
    pub fn into8(&self) -> rgb::RGB8 {
        rgb::RGB8 {
            r: self.red,
            g: self.green,
            b: self.blue,
        }
    }
}

#[derive(Debug, Clone, Copy)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct hw_slice {
    first: i32,
    last: i32,
    step: u32,
}

#[derive(Debug, Clone, Copy)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct hw_bits {
    data: *const u8,
    offset: i32,
    size: u32,
}

#[derive(Debug, Clone, Copy)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct hw_indexes {
    data: *const i32,
    size: usize,
}

pub mod printers;

#[cfg(feature = "cups")]
pub mod printers_cups;

#[cfg(target_os = "windows")]
pub mod printers_win;

pub mod ads1115;
pub mod dht11;
pub mod display;
pub mod gpio;
pub mod hc_sr04;
pub mod i2c;
pub mod infrared;
pub mod lcd1602;
pub mod max7219;
pub mod mpr121;
pub mod nfc;
pub mod rotenc;
pub mod rpi_gyro;
pub mod rpi_pwm;
pub mod rpi_pwm_pca9685;
pub mod spi;
pub mod vl53l0x;
pub mod ws2812;
