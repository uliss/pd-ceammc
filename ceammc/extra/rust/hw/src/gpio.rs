use std::{ffi::c_void, ffi::CString, ptr::null_mut};

use crate::{hw_error_cb, hw_notify_cb};
use log::{debug, error};

#[cfg(target_os = "linux")]
use rppal::gpio::{Gpio, Level, OutputPin};
#[cfg(target_os = "linux")]
use rppal::system::DeviceInfo;

#[repr(C)]
#[allow(non_camel_case_types)]
/// pin value callback
pub struct hw_gpio_pin_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can not be NULL
    cb: extern "C" fn(*mut c_void, u8, bool),
}

#[allow(non_camel_case_types)]
/// gpio opaque type
pub struct hw_gpio {
    rx: tokio::sync::mpsc::Receiver<HwGpioReply>,
    tx: tokio::sync::mpsc::Sender<HwGpioRequest>,
    on_err: hw_error_cb,
    on_pin: hw_gpio_pin_cb,
}

impl hw_gpio {
    pub fn new(
        on_err: hw_error_cb,
        notify: hw_notify_cb,
        on_pin: hw_gpio_pin_cb,
    ) -> Result<hw_gpio, CString> {
        match tokio::runtime::Runtime::new() {
            Ok(rt) => {
                debug!("creating tokio runtime ...");

                let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<HwGpioRequest>(16);
                let (reply_tx, reply_rx) = tokio::sync::mpsc::channel::<HwGpioReply>(16);

                std::thread::spawn(move || {
                    debug!("[worker thread] starting ...");

                    let _x: Result<(), CString> = rt.block_on(async move {
                        debug!("[worker thread] starting runloop ...");

                        #[cfg(target_os = "linux")]
                        match &Gpio::new() {
                            Ok(gpio) => {
                                //
                                match DeviceInfo::new() {
                                    Ok(dev) => {
                                        debug!("RPi model: {}, soc: {}", dev.model(), dev.soc());
                                    }
                                    Err(err) => error!("{err}"),
                                }

                                loop {
                                    if let Some(req) = req_rx.recv().await {
                                        match process_request(req, gpio, &notify, &reply_tx).await {
                                            Ok(flow) => match flow {
                                                ProcessFlow::Continue => {}
                                                ProcessFlow::Quit => {
                                                    debug!("[worker thread] quit");
                                                    return Ok(());
                                                }
                                            },
                                            Err(err) => {
                                                error!("{err}");
                                                reply_error(err, &notify, &reply_tx).await;
                                            }
                                        }
                                    }
                                }
                            }
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

                Ok(hw_gpio {
                    rx: reply_rx,
                    tx: req_tx,
                    on_err,
                    on_pin,
                })
            }
            Err(err) => Err(CString::new(err.to_string()).unwrap_or_default()),
        }
    }

    pub fn send(&self, value: HwGpioRequest) -> bool {
        if let Err(err) = self.tx.try_send(value) {
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
        debug!("exit");
        if let Err(err) = self.tx.blocking_send(HwGpioRequest::Quit) {
            error!("{err}");
        }
    }
}

pub enum HwGpioRequest {
    Quit,
    Read(u8),
    Write(u8, bool),
    Toggle(u8),
    SetPwmFreq(u8, f64, f64),
    SetPwm(u8, f64, f64),
    ClearPwm(u8),
    SetBias(u8, u8),
    SetInterrupt(u8, u8, f64),
    ClearInterrupt(u8),
}

#[derive(Clone)]
enum HwGpioReply {
    PinLevel(u8, bool),
    Error(CString),
}

#[cfg(target_os = "linux")]
fn gpio_output_pin(gpio: &Gpio, pin: u8) -> Result<OutputPin, String> {
    gpio.get(pin).map_err(|err| err.to_string()).map(|x| {
        let mut pin = x.into_output();
        pin.set_reset_on_drop(false);
        pin
    })
}

#[cfg(target_os = "linux")]
fn gpio_read_pin(gpio: &Gpio, pin: u8) -> Result<bool, String> {
    gpio.get(pin)
        .map_err(|err| err.to_string())
        .map(|x| match x.read() {
            Level::Low => false,
            Level::High => true,
        })
}

#[cfg(target_os = "linux")]
fn gpio_write_pin(gpio: &Gpio, pin: u8, state: bool) -> Result<(), String> {
    gpio_output_pin(gpio, pin).map(|mut x| {
        x.write(match state {
            true => Level::High,
            false => Level::Low,
        });
    })
}

#[cfg(target_os = "linux")]
fn gpio_toggle_pin(gpio: &Gpio, pin: u8) -> Result<(), String> {
    gpio_output_pin(gpio, pin).map(|mut x| {
        x.toggle();
    })
}

#[cfg(target_os = "linux")]
fn gpio_clear_pwm(gpio: &Gpio, pin: u8) -> Result<(), String> {
    gpio_output_pin(gpio, pin).and_then(|mut pin| pin.clear_pwm().map_err(|e| e.to_string()))
}

#[cfg(target_os = "linux")]
fn gpio_set_pwm_freq(gpio: &Gpio, pin: u8, freq: f64, duty: f64) -> Result<(), String> {
    gpio_output_pin(gpio, pin)
        .and_then(|mut pin| pin.set_pwm_frequency(freq, duty).map_err(|e| e.to_string()))
}

#[cfg(target_os = "linux")]
fn gpio_set_pwm(gpio: &Gpio, pin: u8, period: f64, width: f64) -> Result<(), String> {
    use std::time::Duration;

    gpio_output_pin(gpio, pin).and_then(|mut pin| {
        pin.set_pwm(
            Duration::from_secs_f64(period * 0.001),
            Duration::from_secs_f64(width * 0.001),
        )
        .map_err(|e| e.to_string())
    })
}

enum ProcessFlow {
    Continue,
    Quit,
}

async fn process_request(
    req: HwGpioRequest,
    gpio: &Gpio,
    notify: &hw_notify_cb,
    reply_tx: &tokio::sync::mpsc::Sender<HwGpioReply>,
) -> Result<ProcessFlow, String> {
    match req {
        HwGpioRequest::Quit => {
            return Ok(ProcessFlow::Quit);
        }
        HwGpioRequest::Read(pin) => {
            let level = gpio_read_pin(gpio, pin)?;
            reply_tx
                .send(HwGpioReply::PinLevel(pin, level))
                .await
                .map_err(|err| err.to_string())?;
            notify.notify();
        }
        HwGpioRequest::Write(pin, state) => {
            gpio_write_pin(gpio, pin, state)?;
        }
        HwGpioRequest::Toggle(pin) => gpio_toggle_pin(gpio, pin)?,
        HwGpioRequest::SetPwmFreq(pin, freq, duty) => gpio_set_pwm_freq(gpio, pin, freq, duty)?,
        HwGpioRequest::SetPwm(pin, period_ms, width_ms) => {
            gpio_set_pwm(gpio, pin, period_ms, width_ms)?
        }
        HwGpioRequest::ClearPwm(pin) => gpio_clear_pwm(gpio, pin)?,
        HwGpioRequest::SetBias(_, _) => todo!(),
        HwGpioRequest::SetInterrupt(_, _, _) => todo!(),
        HwGpioRequest::ClearInterrupt(_) => todo!(),
    };

    Ok(ProcessFlow::Continue)
}

async fn reply_error(
    msg: String,
    notify: &hw_notify_cb,
    reply_tx: &tokio::sync::mpsc::Sender<HwGpioReply>,
) {
    match reply_tx
        .send(HwGpioReply::Error(CString::new(msg).unwrap_or_default()))
        .await
    {
        Ok(_) => notify.notify(),
        Err(err) => error!("{err}"),
    }
}

/// create new gpio
/// @param on_err - on error callback for output error messages
/// @param notify - notification update callback
/// @param on_pin_value - called on pin value output
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_new(
    on_err: hw_error_cb,
    notify: hw_notify_cb,
    on_pin: hw_gpio_pin_cb,
) -> *mut hw_gpio {
    match hw_gpio::new(on_err, notify, on_pin) {
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

    while let Ok(reply) = gp.rx.try_recv() {
        match reply {
            HwGpioReply::PinLevel(pin, level) => {
                (gp.on_pin.cb)(gp.on_pin.user, pin, level);
                debug!("pin [{pin}] = {level}");
            }
            HwGpioReply::Error(msg) => {
                gp.on_err.exec_raw(msg.as_ptr());
            }
        }
    }
}

/// write pin value
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param level - pin level (=0: low, >0: high)
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_write_pin(gp: *mut hw_gpio, pin: u8, level: bool) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };

    gp.send(HwGpioRequest::Write(pin, level))
}

/// read pin request
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_read_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };

    gp.send(HwGpioRequest::Read(pin))
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

/// set software pwm freq on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param freq - freq in Herz
/// @param duty_cycle - duty cycle in % [0-100] range
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_pwm_freq(gp: *mut hw_gpio, pin: u8, freq: f64, duty_cycle: f64) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
    gp.send(HwGpioRequest::SetPwmFreq(pin, freq, duty_cycle));
}

/// set software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param period - period in msec
/// @param width - duty_cycle width in msec
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_pwm(gp: *mut hw_gpio, pin: u8, period: f64, width: f64) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
    gp.send(HwGpioRequest::SetPwm(pin, period, width));
}

/// clear software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_clear_pwm(gp: *mut hw_gpio, pin: u8) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
    gp.send(HwGpioRequest::ClearPwm(pin));
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub enum hw_gpio_mode {
    Output, Input
}

/// set pin mode
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param mode - pin mode
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_mode(gp: *mut hw_gpio, pin: u8, mode: hw_gpio_mode) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
}

#[cfg(target_os = "linux")]
#[cfg(test)]
mod tests {
    use rppal::system::DeviceInfo;

    use super::*;

    #[test]
    fn gpio_info() {
        println!("{}", DeviceInfo::new().unwrap().model());
    }

    #[test]
    fn gpio_init() {
        let g1 = Gpio::new().unwrap();
        assert!(g1.get(0).is_ok());
        let mut p1 = g1.get(0).unwrap().into_output();
        p1.set_high();
        assert!(p1.is_set_high());

        let g2 = Gpio::new().unwrap();
        let mut p2 = g2.get(1).unwrap().into_output();
        p2.set_low();
        assert!(p2.is_set_low());
    }
}
