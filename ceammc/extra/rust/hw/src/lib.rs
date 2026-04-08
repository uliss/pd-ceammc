use std::{
    ffi::{CStr, CString},
    os::raw::c_char,
    thread::JoinHandle,
};

use ceammc_rs_msg::{msg_cb, msg_level, msg_notify};
use log::{debug, error, info};
pub mod gamepad;

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

#[must_use]
pub enum SendStatus {
    Full,
    Disconnected,
    Ok,
}

impl SendStatus {
    pub fn to_err(&self) -> Result<(), String> {
        match self {
            SendStatus::Disconnected => Err(format!("disconnected")),
            _ => Ok(()),
        }
    }
}

fn send_reply<R>(rep: R, tx: &std::sync::mpsc::SyncSender<R>, notify: msg_notify) -> SendStatus {
    if let Err(err) = tx.try_send(rep) {
        error!("reply send error: {err}");
        match err {
            std::sync::mpsc::TrySendError::Full(_) => SendStatus::Full,
            std::sync::mpsc::TrySendError::Disconnected(_) => SendStatus::Disconnected,
        }
    } else {
        notify.exec();
        SendStatus::Ok
    }
}

fn send_error<R>(tx: &std::sync::mpsc::SyncSender<R>, notify: msg_notify, msg: &str) -> SendStatus
where
    R: MakePdMessage<R>,
{
    error!("{msg}");
    send_reply(R::pd_error(CString::new(msg).unwrap_or_default()), tx, notify)
}

#[allow(dead_code)]
fn send_debug<R>(tx: &std::sync::mpsc::SyncSender<R>, notify: msg_notify, msg: &str) -> SendStatus
where
    R: MakePdMessage<R>,
{
    debug!("{msg}");
    send_reply(R::pd_debug(CString::new(msg).unwrap_or_default()), tx, notify)
}

#[allow(dead_code)]
fn send_info<R>(tx: &std::sync::mpsc::SyncSender<R>, notify: msg_notify, msg: &str) -> SendStatus
where
    R: MakePdMessage<R>,
{
    info!("{msg}");
    send_reply(R::pd_info(CString::new(msg).unwrap_or_default()), tx, notify)
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

#[derive(Debug)]
pub enum WorkerCommand<T> {
    Command(T),
    Quit,
}

pub struct HwThreadWorker<Request, Reply> {
    rx: std::sync::mpsc::Receiver<Reply>,
    tx: std::sync::mpsc::SyncSender<WorkerCommand<Request>>,
    on_msg: msg_cb,
    join_handle: Option<JoinHandle<()>>,
}

impl<Request, Reply> HwThreadWorker<Request, Reply>
where
    Request: Send,
    Reply: MakePdMessage<Reply>,
{
    pub fn new(
        on_msg: msg_cb,
        size: Option<usize>,
    ) -> (
        Self,
        std::sync::mpsc::Receiver<WorkerCommand<Request>>,
        std::sync::mpsc::SyncSender<Reply>,
    ) {
        let (req_tx, req_rx) = std::sync::mpsc::sync_channel(size.unwrap_or(32));
        let (rep_tx, rep_rx) = std::sync::mpsc::sync_channel(size.unwrap_or(32));

        (
            Self {
                rx: rep_rx,
                tx: req_tx,
                on_msg,
                join_handle: None,
            },
            req_rx,
            rep_tx,
        )
    }

    pub fn worker_error(&self, str: &str, tx: &std::sync::mpsc::SyncSender<Reply>, notify: msg_notify) -> SendStatus {
        error!("worker error {str}");
        send_error(tx, notify, &format!("worker error: {str}"))
    }

    pub fn quit(&mut self) {
        if let Some(jh) = self.join_handle.take() {
            if let Err(err) = self.tx.try_send(WorkerCommand::Quit) {
                log::error!("can't send quit: {err}");
            }
            if let Err(err) = jh.join() {
                log::error!("worker join error: {err:?}")
            }
        }
    }

    pub fn spawn<F>(&mut self, tx: std::sync::mpsc::SyncSender<Reply>, notify: msg_notify, fx: F)
    where
        F: FnOnce() -> Result<(), String>,
        F: Send + 'static,
        Reply: Send + 'static,
    {
        self.quit();

        let jh = std::thread::spawn(move || {
            debug!("worker thread start");

            if let Err(err) = fx() {
                if let Err(err2) = send_error(&tx, notify, &format!("worker error: {err}")).to_err() {
                    log::error!("{err}: {err2}")
                }
            }

            debug!("worker thread done")
        });
        self.join_handle = Some(jh);
    }

    // should be called only in the main caller thread!
    pub fn pd_message(&self, level: msg_level, msg: &CString) {
        self.on_msg.exec_cstr(msg, level);
    }

    pub fn send_request(&self, req: Request) -> bool {
        if let Err(err) = self.tx.try_send(WorkerCommand::Command(req)) {
            log::error!("{err}");
            self.on_msg.error_str("device is closed");
            false
        } else {
            true
        }
    }

    pub fn process_reply(&self, fx: &dyn Fn(Reply) -> ()) -> bool {
        while let Ok(rep) = self.rx.try_recv() {
            fx(rep)
        }

        true
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
pub mod pcf8574;
