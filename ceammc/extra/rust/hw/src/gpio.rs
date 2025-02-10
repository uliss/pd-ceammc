use std::{
    ffi::CString,
    ptr::null_mut,
    sync::{Arc, Mutex},
    time::Duration,
};

use crate::hw_error_cb;
use lazy_static::lazy_static;
use log::{debug, error};
use tokio::time::sleep;

#[cfg(target_os = "linux")]
use rppal::{
    self,
    gpio::{self, Gpio, OutputPin},
};

#[allow(non_camel_case_types)]
/// gpio opaque type
pub struct hw_gpio {
    gpio: Arc<HwGpio>,
    on_err: hw_error_cb,
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
    pub fn new(on_err: hw_error_cb) -> Result<hw_gpio, CString> {
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

            false
        } else {
            true
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

pub enum HwGpioRequest {
    Quit,
    Read(u8),
    Write(u8, bool),
    Toggle(u8),
    SetPwmFreq(u8, f64, f64),
    SetPwm(u8, f64, f64),
    ClearPwm(u8),
    SetResetOnDrop(u8, bool),
    SetBias(u8, u8),
    SetInterrupt(u8, u8, f64),
    ClearInterrupt(u8),
}

#[derive(Clone)]
enum HwGpioReply {
    PinLevel(u8, bool),
}

#[cfg(target_os = "linux")]
fn gpio_output_pin(gpio: &Gpio, pin: u8) -> Result<OutputPin, CString> {
    gpio.get(pin)
        .map_err(|err| CString::new(err.to_string()).unwrap_or_default())
        .map(|x| x.into_output())
}

#[cfg(target_os = "linux")]
fn gpio_read_pin(gpio: &Gpio, pin: u8) -> Result<bool, CString> {
    gpio.get(pin)
        .map_err(|err| CString::new(err.to_string()).unwrap_or_default())
        .map(|x| match x.read() {
            gpio::Level::Low => false,
            gpio::Level::High => true,
        })
}

#[cfg(target_os = "linux")]
fn gpio_write_pin(gpio: &Gpio, pin: u8, state: bool) -> Result<(), CString> {
    gpio_output_pin(gpio, pin).map(|mut x| {
        x.write(match state {
            true => gpio::Level::High,
            false => gpio::Level::Low,
        })
    })
}

#[cfg(target_os = "linux")]
fn gpio_toggle_pin(gpio: &Gpio, pin: u8) -> Result<(), CString> {
    gpio_output_pin(gpio, pin).map(|mut x| x.toggle())
}

impl HwGpio {
    pub fn new() -> Result<Arc<HwGpio>, CString> {
        match tokio::runtime::Runtime::new() {
            Ok(rt) => {
                debug!("creating tokio runtime ...");

                let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<HwGpioRequest>(16);
                let (reply_tx, mut reply_rx) = tokio::sync::mpsc::channel::<HwGpioReply>(16);

                std::thread::spawn(move || {
                    debug!("[worker thread] starting ...");

                    let _x: Result<(), CString> = rt.block_on(async move {
                        debug!("[worker thread] starting runloop ...");

                        #[cfg(target_os = "linux")]
                        match &Gpio::new() {
                            Ok(gpio) => loop {
                                if let Some(req) = req_rx.recv().await {
                                    match req {
                                        HwGpioRequest::Quit => {
                                            debug!("[worker thread] quit");
                                            return Ok(());
                                        }
                                        HwGpioRequest::Read(pin) => {
                                            let level = gpio_read_pin(gpio, pin)?;
                                            reply_tx
                                                .send(HwGpioReply::PinLevel(pin, level))
                                                .await
                                                .map_err(|err| {
                                                    CString::new(err.to_string())
                                                        .unwrap_or_default()
                                                })?;
                                        }
                                        HwGpioRequest::Write(pin, state) => {
                                            gpio_write_pin(gpio, pin, state)?
                                        }
                                        HwGpioRequest::Toggle(pin) => gpio_toggle_pin(gpio, pin)?,
                                        HwGpioRequest::SetPwmFreq(_, _, _) => todo!(),
                                        HwGpioRequest::SetPwm(_, _, _) => todo!(),
                                        HwGpioRequest::ClearPwm(_) => todo!(),
                                        HwGpioRequest::SetResetOnDrop(_, _) => todo!(),
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

                        #[cfg(not(target_os = "linux"))]
                        Ok(())
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
pub extern "C" fn ceammc_hw_gpio_new(on_err: hw_error_cb) -> *mut hw_gpio {
    match hw_gpio::new(on_err) {
        Ok(gpio) => Box::into_raw(Box::new(gpio)),
        Err(err) => {
            error!("{}", err.to_str().unwrap_or_default());
            null_mut()
        }
    }
}

/// create new gpio
/// @param gpio - pointer to gpio struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_free(gpio: *mut hw_gpio) {
    if !gpio.is_null() {
        drop(unsafe { Box::from_raw(gpio) })
    }
}

/// process events
/// @param gp - pointer to gpio struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_process_events(gp: *mut hw_gpio) {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return;
    }

    let gp = unsafe { &mut *gp };

    while let Ok(reply) = Arc::get_mut(&mut gp.gpio).unwrap().rx.try_recv() {
        match reply {
            HwGpioReply::PinLevel(_, _) => todo!(),
            // Ok(data) => match data {

            // },
            // Err(err) => gp.on_err.exec_cstr(&err),
        }
    }
}

/// set pin level
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param level - pin level (=0: low, >0: high)
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_pin(gp: *mut hw_gpio, pin: u8, level: bool) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };

    gp.send(HwGpioRequest::Write(pin, level))
}

/// toggle pin level
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_toggle_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
    gp.send(HwGpioRequest::Toggle(pin))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn gpio_init() {
        let g1 = Gpio::new();
        assert!(g1.is_ok());
        let g1 = g1.unwrap();
        assert!(g1.get(0).is_ok());

        let g2 = Gpio::new();
        assert!(g2.is_ok());
        let g2 = g2.unwrap();
        assert!(g2.get(0).is_ok());
    }
}
