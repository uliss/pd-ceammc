use std::collections::HashMap;
use std::time::Duration;
use std::{ffi::c_void, ffi::CString, ptr::null_mut};

use crate::{hw_msg_cb, hw_notify_cb};
use log::{debug, error};

#[cfg(target_os = "linux")]
use rppal::gpio::{self, Gpio, IoPin, Level};
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

#[repr(C)]
#[allow(non_camel_case_types)]
/// pin list callback
pub struct hw_gpio_pin_list_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// can not be NULL
    cb: extern "C" fn(*mut c_void, *const u8, usize),
}

#[allow(non_camel_case_types)]
/// gpio opaque type
pub struct hw_gpio {
    rx: tokio::sync::mpsc::Receiver<HwGpioReply>,
    tx: tokio::sync::mpsc::Sender<HwGpioRequest>,
    on_err: hw_msg_cb,
    on_dbg: hw_msg_cb,
    on_pin: hw_gpio_pin_cb,
    on_pin_list: hw_gpio_pin_list_cb,
}

impl hw_gpio {
    pub fn new(
        on_err: hw_msg_cb,
        on_dbg: hw_msg_cb,
        notify: hw_notify_cb,
        on_pin: hw_gpio_pin_cb,
        on_pin_list: hw_gpio_pin_list_cb,
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
                                        reply_debug(
                                            format!(
                                                "RPi model: {}, soc: {}",
                                                dev.model(),
                                                dev.soc()
                                            ),
                                            &notify,
                                            &reply_tx,
                                        )
                                        .await;
                                    }
                                    Err(err) => error!("{err}"),
                                }

                                let mut pins: HashMap<u8, IoPin> = HashMap::new();

                                loop {
                                    if let Some(req) = req_rx.recv().await {
                                        match process_request(
                                            req, &notify, &reply_tx, &gpio, &mut pins,
                                        )
                                        .await
                                        {
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
                    on_dbg,
                    on_pin,
                    on_pin_list,
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
    SetOutput(u8),
    SetInput(u8),
    ResetPin(u8),
    Read(u8),
    Write(u8, bool),
    Toggle(u8),
    SetPwmFreq(u8, f64, f64),
    SetPwm(u8, f64, f64),
    ClearPwm(u8),
    SetBias(u8, gpio::Bias),
    SetInterrupt(u8, u8, f64),
    ClearInterrupt(u8),
    ListPins,
}

#[derive(Clone)]
enum HwGpioReply {
    PinLevel(u8, bool),
    Error(CString),
    Debug(CString),
    Pins(Vec<u8>),
}

enum ProcessFlow {
    Continue,
    Quit,
}

#[cfg(target_os = "linux")]
fn get_pin(pin: u8, pins: &mut HashMap<u8, IoPin>) -> Result<&mut IoPin, String> {
    Ok(pins
        .get_mut(&pin)
        .ok_or(format!("pin not configured for I/O: [{pin}]"))?)
}

#[cfg(target_os = "linux")]
fn check_output(pin: &mut IoPin) -> Result<&mut IoPin, String> {
    if pin.mode() != gpio::Mode::Output {
        return Err(format!("pin [{}] not configured for output", pin.pin()));
    } else {
        Ok(pin)
    }
}

#[cfg(target_os = "linux")]
fn check_input(pin: &mut IoPin) -> Result<&mut IoPin, String> {
    if pin.mode() != gpio::Mode::Input {
        return Err(format!("pin [{}] not configured for input", pin.pin()));
    } else {
        Ok(pin)
    }
}

#[cfg(target_os = "linux")]
async fn process_request(
    req: HwGpioRequest,
    notify: &hw_notify_cb,
    reply_tx: &tokio::sync::mpsc::Sender<HwGpioReply>,
    gpio: &Gpio,
    pins: &mut HashMap<u8, IoPin>,
) -> Result<ProcessFlow, String> {
    match req {
        HwGpioRequest::Quit => {
            return Ok(ProcessFlow::Quit);
        }
        HwGpioRequest::Read(pin) => {
            let level = get_pin(pin, pins)?.read() == Level::High;
            reply(HwGpioReply::PinLevel(pin, level), notify, reply_tx).await;
        }
        HwGpioRequest::Write(pin, state) => {
            let io_pin = get_pin(pin, pins).and_then(|pin| check_output(pin))?;
            if state {
                io_pin.set_high();
            } else {
                io_pin.set_low();
            }
        }
        HwGpioRequest::Toggle(pin) => {
            get_pin(pin, pins)
                .and_then(|pin| check_output(pin))
                .and_then(|pin| Ok(pin.toggle()))?;
        }
        HwGpioRequest::SetPwmFreq(pin, freq, duty) => {
            get_pin(pin, pins)
                .and_then(|pin| check_output(pin))
                .and_then(|pin| pin.set_pwm_frequency(freq, duty).map_err(|e| e.to_string()))?;
        }
        HwGpioRequest::SetPwm(pin, period_ms, width_ms) => {
            get_pin(pin, pins)
                .and_then(|pin| check_output(pin))
                .and_then(|pin| {
                    pin.set_pwm(
                        Duration::from_secs_f64(period_ms * 0.001),
                        Duration::from_secs_f64(width_ms * 0.001),
                    )
                    .map_err(|e| e.to_string())
                })?;
        }
        HwGpioRequest::ClearPwm(pin) => {
            get_pin(pin, pins)
                .and_then(|pin| check_output(pin))
                .and_then(|pin| pin.clear_pwm().map_err(|e| e.to_string()))?;
        }
        HwGpioRequest::SetBias(pin, bias) => {
            get_pin(pin, pins)
                .and_then(|pin| check_input(pin))
                .and_then(|pin| Ok(pin.set_bias(bias)))?;
        }
        HwGpioRequest::SetInterrupt(_, _, _) => todo!(),
        HwGpioRequest::ClearInterrupt(_) => todo!(),
        HwGpioRequest::SetOutput(pin) => match pins.get_mut(&pin) {
            Some(pin) => {
                pin.set_mode(gpio::Mode::Output);
            }
            None => {
                let io_pin = gpio
                    .get(pin)
                    .map_err(|e| e.to_string())?
                    .into_io(gpio::Mode::Output);
                pins.insert(pin, io_pin);
            }
        },
        HwGpioRequest::SetInput(pin) => match pins.get_mut(&pin) {
            Some(pin) => {
                pin.set_mode(gpio::Mode::Input);
            }
            None => {
                let io_pin = gpio
                    .get(pin)
                    .map_err(|e| e.to_string())?
                    .into_io(gpio::Mode::Input);
                pins.insert(pin, io_pin);
            }
        },
        HwGpioRequest::ResetPin(pin) => {
            if !pins.contains_key(&pin) {
                return Err(format!("pin [{pin}] not configured"));
            } else {
                pins.remove(&pin);
            }
        }
        HwGpioRequest::ListPins => {
            let keys = pins.keys().into_iter().map(|k| *k).collect::<Vec<_>>();
            reply(HwGpioReply::Pins(keys), notify, reply_tx).await;
        }
    };

    Ok(ProcessFlow::Continue)
}

async fn reply(
    msg: HwGpioReply,
    notify: &hw_notify_cb,
    reply_tx: &tokio::sync::mpsc::Sender<HwGpioReply>,
) {
    match reply_tx.send(msg).await {
        Ok(_) => notify.notify(),
        Err(err) => error!("{err}"),
    }
}

async fn reply_error(
    msg: String,
    notify: &hw_notify_cb,
    reply_tx: &tokio::sync::mpsc::Sender<HwGpioReply>,
) {
    reply(
        HwGpioReply::Error(CString::new(msg).unwrap_or_default()),
        notify,
        reply_tx,
    )
    .await;
}

async fn reply_debug(
    msg: String,
    notify: &hw_notify_cb,
    reply_tx: &tokio::sync::mpsc::Sender<HwGpioReply>,
) {
    reply(
        HwGpioReply::Debug(CString::new(msg).unwrap_or_default()),
        notify,
        reply_tx,
    )
    .await;
}

/// create new gpio
/// @param on_err - on error callback for output error messages
/// @param on_dbg - on error callback for output error messages
/// @param notify - notification update callback
/// @param on_pin - called on pin value output
/// @param on_pin_list - called on pin list reply
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_new(
    on_err: hw_msg_cb,
    on_dbg: hw_msg_cb,
    notify: hw_notify_cb,
    on_pin: hw_gpio_pin_cb,
    on_pin_list: hw_gpio_pin_list_cb,
) -> *mut hw_gpio {
    match hw_gpio::new(on_err, on_dbg, notify, on_pin, on_pin_list) {
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
            HwGpioReply::Debug(msg) => {
                gp.on_dbg.exec_raw(msg.as_ptr());
            }
            HwGpioReply::Pins(items) => {
                (gp.on_pin_list.cb)(gp.on_pin_list.user, items.as_ptr(), items.len());
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
/// @param freq - freq in Hertz
/// @param duty_cycle - duty cycle in % [0-100] range
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_pwm_freq(
    gp: *mut hw_gpio,
    pin: u8,
    freq: f64,
    duty_cycle: f64,
) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
    gp.send(HwGpioRequest::SetPwmFreq(pin, freq, duty_cycle))
}

/// set software pwm on pin
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
/// @param period - period in msec
/// @param width - duty_cycle width in msec
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_set_pwm(
    gp: *mut hw_gpio,
    pin: u8,
    period: f64,
    width: f64,
) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
    gp.send(HwGpioRequest::SetPwm(pin, period, width))
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
    gp.send(HwGpioRequest::ClearPwm(pin))
}

/// reset pin to initial state
/// @param gpio - pointer to gpio struct
/// @param pin - pin number
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_reset_pin(gp: *mut hw_gpio, pin: u8) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
    gp.send(HwGpioRequest::ResetPin(pin))
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub enum hw_gpio_mode {
    Output,
    Input,
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
    match mode {
        hw_gpio_mode::Output => gp.send(HwGpioRequest::SetOutput(pin)),
        hw_gpio_mode::Input => gp.send(HwGpioRequest::SetInput(pin)),
    }
}

/// list used pins
/// @param gpio - pointer to gpio struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_list_pins(gp: *mut hw_gpio) -> bool {
    if gp.is_null() {
        log::error!("NULL gpio pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };
    gp.send(HwGpioRequest::ListPins)
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
