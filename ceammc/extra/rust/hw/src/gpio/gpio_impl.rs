use crate::gpio::HW_GPIO_IMPULSE_LENGTH_MIN_MSEC;
use crate::hw_msg_cb;
use crate::hw_notify_cb;
use crate::send_debug;
use crate::send_error;
use crate::send_reply;
use log::{debug, error};
use rppal::system::DeviceInfo;

use std::collections::HashMap;
use std::ffi::c_int;
use std::ffi::CString;
use std::time::Duration;

use rppal::gpio::{self, Gpio};

use super::hw_gpio;
use super::hw_gpio_bias;
use super::hw_gpio_pin_cb;
use super::hw_gpio_pin_list_cb;
use super::hw_gpio_poll_cb;
use super::hw_gpio_trigger;
use super::GpioThreadWorker;
use super::Reply;
use super::Request;
use super::HW_GPIO_IMPULSE_LENGTH_MAX_MSEC;

impl hw_gpio {
    pub fn exec_pin(&self, pin: u8, level: bool) {
        (self.on_pin.cb)(self.on_pin.user, pin, level);
    }

    pub fn exec_pin_list(&self, items: &Vec<u8>) {
        (self.on_pin_list.cb)(self.on_pin_list.user, items.as_ptr(), items.len());
    }

    pub fn new(
        on_msg: hw_msg_cb,
        notify: hw_notify_cb,
        on_pin: hw_gpio_pin_cb,
        on_pin_list: hw_gpio_pin_list_cb,
        on_pin_poll: hw_gpio_poll_cb,
    ) -> Result<hw_gpio, CString> {
        let (mut worker, rx, tx) = GpioThreadWorker::new(on_msg, None);

        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let gpio = Gpio::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            let dev = DeviceInfo::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            send_debug(&tx, notify, &format!("RPi model: {}, soc: {}", dev.model(), dev.soc())).to_err()?;

            let mut pins: HashMap<u8, GpioPin> = HashMap::new();

            while let Ok(crate::WorkerCommand::Command(req)) = rx.recv() {
                if let Err(err) = process_request(req, &notify, on_pin_poll, &tx, &gpio, &mut pins) {
                    send_error(&tx, notify, &err.to_string()).to_err()?;
                }
            }

            Ok(())
        });

        Ok(hw_gpio {
            worker,
            on_pin,
            on_pin_list,
        })
    }

    pub fn send_request_ptr(gp: *mut hw_gpio, req: Request) -> bool {
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.worker.send_request(req)
    }

    pub fn process_reply_ptr(gp: *mut hw_gpio) -> bool {
        if gp.is_null() {
            log::error!("NULL gpio pointer");
            return false;
        }

        let gp = unsafe { &mut *gp };
        gp.worker.process_reply(&|reply| match reply {
            Reply::PinLevel(pin, level) => {
                gp.exec_pin(pin, level);
                debug!("pin [{pin}] = {level}");
            }
            Reply::Message(level, msg) => {
                gp.worker.pd_message(level, &msg);
            }
            Reply::Pins(items) => {
                gp.exec_pin_list(&items);
            }
        })
    }
}

enum GpioPin {
    Input(gpio::InputPin),
    Output(gpio::OutputPin),
}

fn get_output_pin(pin: u8, pins: &mut HashMap<u8, GpioPin>) -> Result<&mut gpio::OutputPin, String> {
    let x = pins
        .get_mut(&pin)
        .ok_or(format!("pin is not configured for I/O: [{pin}]"))?;

    match x {
        GpioPin::Input(_) => Err(format!("pin [{pin}] is not configured for output")),
        GpioPin::Output(output_pin) => Ok(output_pin),
    }
}

fn get_input_pin(pin: u8, pins: &mut HashMap<u8, GpioPin>) -> Result<&mut gpio::InputPin, String> {
    let x = pins
        .get_mut(&pin)
        .ok_or(format!("pin is not configured for I/O: [{pin}]"))?;

    match x {
        GpioPin::Input(input_pin) => Ok(input_pin),
        GpioPin::Output(_) => Err(format!("pin [{pin}] is not configured for input")),
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

fn process_request(
    req: Request,
    notify: &hw_notify_cb,
    poll_notify: hw_gpio_poll_cb,
    reply_tx: &std::sync::mpsc::SyncSender<Reply>,
    gpio: &Gpio,
    pins: &mut HashMap<u8, GpioPin>,
) -> Result<(), String> {
    match req {
        Request::Read(pin) => {
            let level = match pins.get(&pin) {
                Some(x) => match x {
                    GpioPin::Input(input_pin) => input_pin.is_high(),
                    GpioPin::Output(output_pin) => output_pin.is_set_high(),
                },
                None => return Err(format!("pin [{pin}] is not configured")),
            };

            send_reply(Reply::PinLevel(pin, level), reply_tx, *notify).to_err()?
        }
        Request::Write(pin, state) => {
            let io_pin = get_output_pin(pin, pins)?;
            if state {
                io_pin.set_high();
            } else {
                io_pin.set_low();
            }
        }
        Request::Toggle(pin) => {
            get_output_pin(pin, pins).and_then(|pin| Ok(pin.toggle()))?;
        }
        Request::SetPwmFreq(pin, freq, duty) => {
            get_output_pin(pin, pins).and_then(|pin| pin.set_pwm_frequency(freq, duty).map_err(|e| e.to_string()))?;
        }
        Request::SetPwm(pin, period_ms, width_ms) => {
            get_output_pin(pin, pins).and_then(|pin| {
                pin.set_pwm(
                    Duration::from_secs_f64(period_ms * 0.001),
                    Duration::from_secs_f64(width_ms * 0.001),
                )
                .map_err(|e| e.to_string())
            })?;
        }
        Request::ClearPwm(pin) => {
            get_output_pin(pin, pins).and_then(|pin| pin.clear_pwm().map_err(|e| e.to_string()))?;
        }
        Request::SetBias(pin, bias) => {
            get_input_pin(pin, pins).and_then(|pin| {
                Ok(pin.set_bias(match bias {
                    hw_gpio_bias::None => gpio::Bias::Off,
                    hw_gpio_bias::PullUp => gpio::Bias::PullUp,
                    hw_gpio_bias::PullDown => gpio::Bias::PullDown,
                }))
            })?;
        }
        Request::SetInterrupt(pin, trigger, debounce) => {
            get_input_pin(pin, pins).and_then(|x| {
                x.set_async_interrupt(
                    match trigger {
                        hw_gpio_trigger::None => gpio::Trigger::Disabled,
                        hw_gpio_trigger::RisingEdge => gpio::Trigger::RisingEdge,
                        hw_gpio_trigger::FallingEdge => gpio::Trigger::FallingEdge,
                        hw_gpio_trigger::Both => gpio::Trigger::Both,
                    },
                    debounce,
                    move |ev| {
                        poll_notify.exec(pin, ev.trigger);
                    },
                )
                .map_err(|e| e.to_string())
            })?;
        }
        Request::ClearInterrupt(pin) => {
            get_input_pin(pin, pins).and_then(|pin| pin.clear_async_interrupt().map_err(|e| e.to_string()))?;
        }
        Request::SetOutput(pin) => {
            if pins.contains_key(&pin) {
                match pins.get(&pin) {
                    Some(x) => match x {
                        GpioPin::Input(_) => {
                            pins.remove(&pin);
                        }
                        _ => return Ok(()),
                    },
                    None => {}
                }
            }

            let out_pin = gpio.get(pin).map_err(|e| e.to_string())?.into_output();
            pins.insert(pin, GpioPin::Output(out_pin));
        }
        Request::SetInput(pin) => {
            if pins.contains_key(&pin) {
                match pins.get(&pin) {
                    Some(x) => match x {
                        GpioPin::Output(_) => {
                            pins.remove(&pin);
                        }
                        _ => return Ok(()),
                    },
                    None => {}
                }
            }

            let in_pin = gpio.get(pin).map_err(|e| e.to_string())?.into_input_pulldown();
            pins.insert(pin, GpioPin::Input(in_pin));
        }
        Request::ResetPin(pin) => {
            if pins.contains_key(&pin) {
                pins.remove(&pin);
            }
        }
        Request::ResetPins(reset_pins) => {
            for pin in reset_pins {
                if pins.contains_key(&pin) {
                    pins.remove(&pin);
                }
            }
        }
        Request::ListPins => {
            let keys = pins.keys().into_iter().map(|k| *k).collect::<Vec<_>>();
            send_reply(Reply::Pins(keys), reply_tx, *notify).to_err()?
        }
        Request::Impulse(pin, length_ms) => {
            if length_ms < HW_GPIO_IMPULSE_LENGTH_MIN_MSEC || length_ms > HW_GPIO_IMPULSE_LENGTH_MAX_MSEC {
                return Err(format!("invalid impulse length: {length_ms}, should be in [{HW_GPIO_IMPULSE_LENGTH_MIN_MSEC} ... {HW_GPIO_IMPULSE_LENGTH_MAX_MSEC}] range"));
            }

            let pin = get_output_pin(pin, pins)?;
            pin.set_high();
            std::thread::sleep(Duration::from_micros(
                (length_ms.clamp(HW_GPIO_IMPULSE_LENGTH_MIN_MSEC, HW_GPIO_IMPULSE_LENGTH_MAX_MSEC) * 1000.0).round()
                    as u64,
            ));
            pin.set_low();
        }
    };

    Ok(())
}
