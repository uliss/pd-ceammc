use std::{
    ffi::CString,
    ptr::null_mut,
    sync::{Arc, Mutex},
    time::Duration,
};

use lazy_static::lazy_static;
use log::{debug, error};
use tokio::time::sleep;

#[allow(non_camel_case_types)]
/// gpio opaque type
pub struct hw_gpio {
    gpio: Arc<HwGpio>, // on_err: gamepad_err_cb,
                       // on_event: gamepad_event_cb,
                       // on_devinfo: gamepad_listdev_cb,
                       // req_tx: GamepadTx,
                       // reply_rx: GamepadRx,
}

struct SharedGpio {
    data: Option<Arc<HwGpio>>,
}

impl SharedGpio {
    fn new() -> Self {
        SharedGpio { data: None }
    }

    fn reset(&mut self) {
        debug!("reset");
        self.data = None;
    }

    fn check_init(&mut self) -> Result<(), CString> {
        if self.data.is_none() {
            match &HwGpio::new() {
                Ok(gpio) => {
                    debug!("new");
                    self.data = Some(gpio.clone());
                    Ok(())
                }
                Err(err) => return Err(err.clone()),
            }
        } else {
            Ok(())
        }
    }

    fn make(&mut self) -> Result<Arc<HwGpio>, CString> {
        match self.check_init() {
            Ok(_) => Ok(self.data.as_ref().unwrap().clone()),
            Err(err) => Err(err),
        }
    }
}

lazy_static! {
    static ref GPIO: Mutex<SharedGpio> = Mutex::new(SharedGpio::new());
}

impl hw_gpio {
    pub fn new() -> Result<hw_gpio, CString> {
        match GPIO.lock().as_mut() {
            Ok(x) => {
                debug!("+1");
                x.make().map(|x| hw_gpio { gpio: x })
            },
            Err(err) => Err(CString::new(err.to_string()).unwrap_or_default()),
        }
    }
}

impl Drop for hw_gpio {
    fn drop(&mut self) {
        debug!("-1");
        if Arc::strong_count(&self.gpio) <= 2 {
            GPIO.lock().unwrap().reset();
        }
    }
}

struct HwGpio {
    rx: tokio::sync::mpsc::Receiver<HwGpioReply>,
    tx: tokio::sync::mpsc::Sender<HwGpioRequest>,
}



enum HwGpioRequest {
    Quit,
    Read,
    SetHigh(u8),
    SetLow(u8),
    Toggle(u8),
    SetPwmFreq(u8, f64, f64),
    SetPwm(u8, f64, f64),
    ClearPwm(u8),
    SetResetOnDrop(u8, bool),
    IsHigh(u8),
    IsLow(u8),
    SetBias(u8, u8),
    SetInterrupt(u8, u8, f64),
    ClearInterrupt(u8)
}

enum HwGpioReply {}

impl HwGpio {
    pub fn new() -> Result<Arc<HwGpio>, CString> {
        match tokio::runtime::Runtime::new() {
            Ok(rt) => {
                debug!("creating tokio runtime ...");

                let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<HwGpioRequest>(16);
                let (reply_tx, reply_rx) = tokio::sync::mpsc::channel::<HwGpioReply>(64);

                std::thread::spawn(move || {
                    debug!("[worker thread] starting ...");

                    let x = rt.block_on(async move {
                        debug!("[worker thread] starting runloop ...");

                        loop {
                            if let Ok(req) = req_rx.try_recv() {
                                match req {
                                    HwGpioRequest::Quit => {
                                        debug!("[worker thread] quit");
                                        return Ok::<(), ()>(());
                                    }
                                    HwGpioRequest::Read => todo!(),
                                    HwGpioRequest::SetHigh(pin) => todo!(),
                                    HwGpioRequest::SetLow(pin) => todo!(),
                                    HwGpioRequest::Toggle(_) => todo!(),
                                    HwGpioRequest::SetPwmFreq(_, _, _) => todo!(),
                                    HwGpioRequest::SetPwm(_, _, _) => todo!(),
                                    HwGpioRequest::ClearPwm(_) => todo!(),
                                    HwGpioRequest::SetResetOnDrop(_, _) => todo!(),
                                    HwGpioRequest::IsHigh(_) => todo!(),
                                    HwGpioRequest::IsLow(_) => todo!(),
                                    HwGpioRequest::SetBias(_, _) => todo!(),
                                    HwGpioRequest::SetInterrupt(_, _, _) => todo!(),
                                    HwGpioRequest::ClearInterrupt(_) => todo!(),
                                }
                            }

                            sleep(Duration::from_millis(100)).await;
                        }
                    });

                    debug!("[worker thread] exit");
                });

                Ok(Arc::new(HwGpio {
                    rx: reply_rx,
                    tx: req_tx,
                }))
            }
            Err(err) => Err(CString::new(err.to_string()).unwrap_or_default()),
        }
    }
}

impl Drop for HwGpio {
    fn drop(&mut self) {
        debug!("exit");
        let _ = self.tx.blocking_send(HwGpioRequest::Quit);
    }
}

/// create new gpio
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_new(// on_err: gamepad_err_cb,
) -> *mut hw_gpio {
    match hw_gpio::new() {
        Ok(gpio) => Box::into_raw(Box::new(gpio)),
        Err(err) => {
            error!("{}", err.to_str().unwrap_or_default());
            null_mut()
        }
    }
}

/// create new gpio
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_free(
    // on_err: gamepad_err_cb,
    gpio: *mut hw_gpio,
) {
    if !gpio.is_null() {
        drop(unsafe { Box::from_raw(gpio) })
    }
}
