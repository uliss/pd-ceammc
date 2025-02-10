use std::{
    ffi::{c_char, c_void, CString},
    ptr::null_mut,
    sync::{Arc, Mutex},
    time::Duration,
};

use lazy_static::lazy_static;
use log::{debug, error};
use rppal::{
    self,
    gpio::{self, Gpio, OutputPin},
};
use tokio::time::sleep;

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct gpio_err_cb {
    /// pointer to user data
    user: *mut c_void,
    cb: Option<extern "C" fn(*mut c_void, *const c_char)>,
}

impl gpio_err_cb {
    fn exec(&self, msg: &str) {
        self.cb.map(|f| {
            f(self.user, CString::new(msg).unwrap_or_default().as_ptr());
        });
    }
}

#[allow(non_camel_case_types)]
/// gpio opaque type
pub struct hw_gpio {
    gpio: Arc<HwGpio>,
    on_err: gpio_err_cb,
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
    pub fn new(on_err: gpio_err_cb) -> Result<hw_gpio, CString> {
        match GPIO.lock().as_mut() {
            Ok(x) => {
                debug!("+1");
                x.make().map(|x| hw_gpio { gpio: x, on_err })
            }
            Err(err) => Err(CString::new(err.to_string()).unwrap_or_default()),
        }
    }

    pub fn send(&self, value: HwGpioRequest) -> bool {
        if let Err(err) = self.gpio.tx.try_send(value) {
            self.on_err
                .exec(format!("[owner] send error: {err}").as_str());
            return false;
        }

        todo!()
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
    _rx: tokio::sync::mpsc::Receiver<HwGpioReply>,
    tx: tokio::sync::mpsc::Sender<HwGpioRequest>,
}

pub enum HwGpioRequest {
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
    ClearInterrupt(u8),
}

enum HwGpioReply {}

fn gpio_output_pin(gpio: &Gpio, pin: u8) -> Result<OutputPin, CString> {
    gpio.get(pin)
        .map_err(|err| CString::new(err.to_string()).unwrap_or_default())
        .map(|x| x.into_output())
}

fn gpio_set_pin(gpio: &Gpio, pin: u8, state: bool) -> Result<(), CString> {
    gpio_output_pin(gpio, pin).map(|mut x| {
        x.write(match state {
            true => gpio::Level::High,
            false => gpio::Level::Low,
        })
    })
}

fn gpio_toggle_pin(gpio: &Gpio, pin: u8) -> Result<(), CString> {
    gpio_output_pin(gpio, pin).map(|mut x| x.toggle())
}

impl HwGpio {
    pub fn new() -> Result<Arc<HwGpio>, CString> {
        match tokio::runtime::Runtime::new() {
            Ok(rt) => {
                debug!("creating tokio runtime ...");

                let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<HwGpioRequest>(16);
                let (_reply_tx, reply_rx) = tokio::sync::mpsc::channel::<HwGpioReply>(64);

                std::thread::spawn(move || {
                    debug!("[worker thread] starting ...");

                    let _x: Result<(), CString> = rt.block_on(async move {
                        debug!("[worker thread] starting runloop ...");

                        match &Gpio::new() {
                            Ok(gpio) => loop {
                                if let Ok(req) = req_rx.try_recv() {
                                    match req {
                                        HwGpioRequest::Quit => {
                                            debug!("[worker thread] quit");
                                            return Ok(());
                                        }
                                        HwGpioRequest::Read => todo!(),
                                        HwGpioRequest::SetHigh(pin) => {
                                            gpio_set_pin(gpio, pin, true)?
                                        }
                                        HwGpioRequest::SetLow(pin) => {
                                            gpio_set_pin(gpio, pin, false)?
                                        }
                                        HwGpioRequest::Toggle(pin) => gpio_toggle_pin(gpio, pin)?,
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
                            },
                            Err(err) => {
                                error!("can't init GPIO: {err}");
                                return Err(CString::new(err.to_string()).unwrap_or_default());
                            }
                        };
                    });

                    debug!("[worker thread] exit");
                });

                Ok(Arc::new(HwGpio {
                    _rx: reply_rx,
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
pub extern "C" fn ceammc_hw_gpio_new(on_err: gpio_err_cb) -> *mut hw_gpio {
    match hw_gpio::new(on_err) {
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

/// set pin level
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_pin(gp: *mut hw_gpio, pin: u8, level: bool) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };

    if level {
        gp.send(HwGpioRequest::SetHigh(pin))
    } else {
        gp.send(HwGpioRequest::SetLow(pin))
    }
}

/// toggle pin level
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_toggle_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
    gp.send(HwGpioRequest::Toggle(pin))
}
