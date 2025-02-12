use crate::hw_msg_cb;
use crate::hw_notify_cb;
use log::{debug, error};
use rppal::system::DeviceInfo;

use std::collections::HashMap;
use std::ffi::c_int;
use std::ffi::CString;
use std::time::Duration;

use rppal::gpio::{self, Gpio};
use tokio::sync::mpsc::error::TryRecvError;

use super::hw_gpio_pin_cb;
use super::hw_gpio_pin_list_cb;
use super::hw_gpio_poll_cb;

/// gpio opaque type
pub struct hw_gpio {
    rx: tokio::sync::mpsc::Receiver<HwGpioReply>,
    tx: tokio::sync::mpsc::Sender<HwGpioRequest>,
    pub on_err: hw_msg_cb,
    pub on_dbg: hw_msg_cb,
    on_pin: hw_gpio_pin_cb,
    on_pin_list: hw_gpio_pin_list_cb,
}

impl hw_gpio {
    pub fn try_recv(&mut self) -> Result<HwGpioReply, TryRecvError> {
        self.rx.try_recv()
    }

    pub fn exec_pin(&self, pin: u8, level: bool) {
        (self.on_pin.cb)(self.on_pin.user, pin, level);
    }

    pub fn exec_pin_list(&self, items: &Vec<u8>)
    {
        (self.on_pin_list.cb)(self.on_pin_list.user, items.as_ptr(), items.len());
    }

    pub fn new(
        on_err: hw_msg_cb,
        on_dbg: hw_msg_cb,
        notify: hw_notify_cb,
        on_pin: hw_gpio_pin_cb,
        on_pin_list: hw_gpio_pin_list_cb,
        on_pin_poll: hw_gpio_poll_cb,
    ) -> Result<hw_gpio, CString> {
        match tokio::runtime::Builder::new_multi_thread()
            .worker_threads(1)
            .thread_name("pp-gpio-worker")
            .build()
        {
            Ok(rt) => {
                debug!("creating tokio runtime ...");

                let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<HwGpioRequest>(16);
                let (reply_tx, reply_rx) = tokio::sync::mpsc::channel::<HwGpioReply>(16);

                std::thread::spawn(move || {
                    debug!("[worker thread] starting ...");

                    let _x: Result<(), CString> = rt.block_on(async move {
                        debug!("[worker thread] starting runloop ...");

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

                                let mut pins: HashMap<u8, GpioPin> = HashMap::new();

                                loop {
                                    if let Some(req) = req_rx.recv().await {
                                        match process_request(
                                            req,
                                            &notify,
                                            on_pin_poll,
                                            &reply_tx,
                                            &gpio,
                                            &mut pins,
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
    SetInterrupt(u8, gpio::Trigger, Option<Duration>),
    ClearInterrupt(u8),
    ListPins,
}

#[derive(Clone)]
pub enum HwGpioReply {
    PinLevel(u8, bool),
    Error(CString),
    Debug(CString),
    Pins(Vec<u8>),
}

pub enum ProcessFlow {
    Continue,
    Quit,
}

enum GpioPin {
    Input(gpio::InputPin),
    Output(gpio::OutputPin),
}

fn get_output_pin(
    pin: u8,
    pins: &mut HashMap<u8, GpioPin>,
) -> Result<&mut gpio::OutputPin, String> {
    let x = pins
        .get_mut(&pin)
        .ok_or(format!("pin not configured for I/O: [{pin}]"))?;

    match x {
        GpioPin::Input(_) => Err(format!("pin [{pin}] not configured for output")),
        GpioPin::Output(output_pin) => Ok(output_pin),
    }
}

fn get_input_pin(pin: u8, pins: &mut HashMap<u8, GpioPin>) -> Result<&mut gpio::InputPin, String> {
    let x = pins
        .get_mut(&pin)
        .ok_or(format!("pin not configured for I/O: [{pin}]"))?;

    match x {
        GpioPin::Input(input_pin) => Ok(input_pin),
        GpioPin::Output(_) => Err(format!("pin [{pin}] not configured for input")),
    }
}

impl hw_gpio_poll_cb {
    fn exec(&self, pin: u8, trig: gpio::Trigger) {
        let data: i32 = pin as i32;
        let trig: i32 = match trig {
            gpio::Trigger::Disabled => 0,
            gpio::Trigger::RisingEdge => 1,
            gpio::Trigger::FallingEdge => 2,
            gpio::Trigger::Both => 3,
        };
        let data = (data | (trig << 8)) as c_int;

        (self.cb)(self.id, data);
        debug!("poll [{pin}]: {trig}");
    }
}

async fn process_request(
    req: HwGpioRequest,
    notify: &hw_notify_cb,
    poll_notify: hw_gpio_poll_cb,
    reply_tx: &tokio::sync::mpsc::Sender<HwGpioReply>,
    gpio: &Gpio,
    pins: &mut HashMap<u8, GpioPin>,
) -> Result<ProcessFlow, String> {
    match req {
        HwGpioRequest::Quit => {
            return Ok(ProcessFlow::Quit);
        }
        HwGpioRequest::Read(pin) => {
            let level = match pins.get(&pin) {
                Some(x) => match x {
                    GpioPin::Input(input_pin) => input_pin.is_high(),
                    GpioPin::Output(output_pin) => output_pin.is_set_high(),
                },
                None => return Err(format!("pin [{pin}] not configured")),
            };

            reply(HwGpioReply::PinLevel(pin, level), notify, reply_tx).await;
        }
        HwGpioRequest::Write(pin, state) => {
            let io_pin = get_output_pin(pin, pins)?;
            if state {
                io_pin.set_high();
            } else {
                io_pin.set_low();
            }
        }
        HwGpioRequest::Toggle(pin) => {
            get_output_pin(pin, pins).and_then(|pin| Ok(pin.toggle()))?;
        }
        HwGpioRequest::SetPwmFreq(pin, freq, duty) => {
            get_output_pin(pin, pins)
                .and_then(|pin| pin.set_pwm_frequency(freq, duty).map_err(|e| e.to_string()))?;
        }
        HwGpioRequest::SetPwm(pin, period_ms, width_ms) => {
            get_output_pin(pin, pins).and_then(|pin| {
                pin.set_pwm(
                    Duration::from_secs_f64(period_ms * 0.001),
                    Duration::from_secs_f64(width_ms * 0.001),
                )
                .map_err(|e| e.to_string())
            })?;
        }
        HwGpioRequest::ClearPwm(pin) => {
            get_output_pin(pin, pins).and_then(|pin| pin.clear_pwm().map_err(|e| e.to_string()))?;
        }
        HwGpioRequest::SetBias(pin, bias) => {
            get_input_pin(pin, pins).and_then(|pin| Ok(pin.set_bias(bias)))?;
        }
        HwGpioRequest::SetInterrupt(pin, trigger, debounce) => {
            get_input_pin(pin, pins).and_then(|x| {
                x.set_async_interrupt(trigger, debounce, move |ev| {
                    poll_notify.exec(pin, ev.trigger);
                })
                .map_err(|e| e.to_string())
            })?;
        }
        HwGpioRequest::ClearInterrupt(pin) => {
            get_input_pin(pin, pins)
                .and_then(|pin| pin.clear_async_interrupt().map_err(|e| e.to_string()))?;
        }
        HwGpioRequest::SetOutput(pin) => {
            if pins.contains_key(&pin) {
                match pins.get(&pin) {
                    Some(x) => match x {
                        GpioPin::Input(_) => {
                            pins.remove(&pin);
                        }
                        _ => return Ok(ProcessFlow::Continue),
                    },
                    None => {}
                }
            }

            let out_pin = gpio.get(pin).map_err(|e| e.to_string())?.into_output();
            pins.insert(pin, GpioPin::Output(out_pin));
        }
        HwGpioRequest::SetInput(pin) => {
            if pins.contains_key(&pin) {
                match pins.get(&pin) {
                    Some(x) => match x {
                        GpioPin::Output(_) => {
                            pins.remove(&pin);
                        }
                        _ => return Ok(ProcessFlow::Continue),
                    },
                    None => {}
                }
            }

            let in_pin = gpio
                .get(pin)
                .map_err(|e| e.to_string())?
                .into_input_pulldown();
            pins.insert(pin, GpioPin::Input(in_pin));
        }
        HwGpioRequest::ResetPin(pin) => {
            if !pins.contains_key(&pin) {
                return Err(format!("pin [{pin}] not configured"));
            } else {
                pins.remove(&pin);
            }
        }
        HwGpioRequest::ListPins => {
            let keys = pins.keys().into_iter().map(|k| *k).collect::<Vec<_>>();
            reply(HwGpioReply::Pins(keys), &notify, reply_tx).await;
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
