use log::{debug, error};
use mpr121_hal::{mpr121::Mpr121, Channel, DebounceNumber, Mpr121Address};
use rppal::hal::Delay;
use std::ffi::CString;

use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::{i2c_impl::try_i2c_device, I2cAddress},
    mpr121::{hw_mpr121_reply_cb, hw_sensor_mpr121, Mpr212SensorWorker, Reply, Request},
    process_err, send_debug, send_reply,
};

fn to_channel(num: u8) -> Result<Channel, String> {
    match num {
        0 => Ok(Channel::Zero),
        1 => Ok(Channel::One),
        2 => Ok(Channel::Two),
        3 => Ok(Channel::Three),
        4 => Ok(Channel::Four),
        5 => Ok(Channel::Five),
        6 => Ok(Channel::Six),
        7 => Ok(Channel::Seven),
        8 => Ok(Channel::Eight),
        9 => Ok(Channel::Nine),
        10 => Ok(Channel::Ten),
        11 => Ok(Channel::Eleven),
        _ => Err(format!("invalid channel: {num}, allowed values in [0..11] range")),
    }
}

fn to_debounce(num: u8) -> Result<DebounceNumber, String> {
    match num {
        0 => Ok(DebounceNumber::Zero),
        1 => Ok(DebounceNumber::One),
        2 => Ok(DebounceNumber::Two),
        3 => Ok(DebounceNumber::Three),
        4 => Ok(DebounceNumber::Four),
        5 => Ok(DebounceNumber::Five),
        6 => Ok(DebounceNumber::Six),
        7 => Ok(DebounceNumber::Seven),
        _ => Err(format!("invalid debounce value: {num}, allowed values in [0..7] range")),
    }
}

impl hw_sensor_mpr121 {
    pub(crate) fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        irq_pin: *const u8,
        notify: hw_notify_cb,
        on_msg: hw_msg_cb,
        on_reply: hw_mpr121_reply_cb,
    ) -> Result<Self, CString> {
        let (mut worker, rx, tx) = Mpr212SensorWorker::new(on_msg);
        let irq_pin = if irq_pin.is_null() {
            None
        } else {
            Some(unsafe { *irq_pin })
        };

        let gpio_tx = worker.tx.clone();
        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let mut i2c = crate::i2c::i2c_impl::create_i2c_bus(i2c_bus, &tx, notify)?;
            debug!("i2c init: {i2c:?}, irq: {irq_pin:?}");

            let bus = i2c.bus();
            let mut delay = Delay::new();
            let addr: u16 = match i2c_addr {
                I2cAddress::Addr(addr) => match addr {
                    0x5A => addr,
                    0x5B => addr,
                    0x5C => addr,
                    0x5D => addr,
                    _ => return Err(format!("invalid i2c address: {addr}")),
                },
                I2cAddress::Default => Mpr121Address::Default as u8,
                I2cAddress::Auto => Mpr121Address::Default as u8,
                I2cAddress::Alt => Mpr121Address::Vdd as u8,
                I2cAddress::Invalid(addr) => return Err(format!("invalid i2c address: {addr}")),
            }
            .into();

            try_i2c_device(&mut i2c, addr, crate::i2c::i2c_impl::DetectMethod::QuickWrite)?;

            let pin = if irq_pin.is_some() {
                let mut pin = rppal::gpio::Gpio::new()
                    .map_err(|err| err.to_string())?
                    .get(irq_pin.unwrap_or_default())
                    .map_err(|err| err.to_string())?
                    .into_input_pulldown();
                pin.set_reset_on_drop(true);
                pin.set_async_interrupt(rppal::gpio::Trigger::FallingEdge, None, move |_event| {
                    if let Err(err) = gpio_tx.send(crate::WorkerCommand::Command(Request::ReadAll)) {
                        log::error!("irq send error: {err}");
                    };
                })
                .map_err(|err| err.to_string())?;
                log::debug!("IRQ pin: {}", pin.pin());
                Some(pin)
            } else {
                None
            };

            let mut sensor = match i2c_addr {
                I2cAddress::Addr(addr) => Mpr121::new(
                    i2c,
                    match addr {
                        0x5A => Mpr121Address::Default,
                        0x5B => Mpr121Address::Vdd,
                        0x5C => Mpr121Address::Sda,
                        0x5D => Mpr121Address::Scl,
                        _ => return Err(format!("invalid i2c address: {addr}")),
                    },
                    &mut delay,
                    true,
                ),
                I2cAddress::Default => Mpr121::new_default(i2c, &mut delay),
                I2cAddress::Auto => Mpr121::new_default(i2c, &mut delay),
                I2cAddress::Alt => Mpr121::new(i2c, Mpr121Address::Vdd, &mut delay, true),
                I2cAddress::Invalid(addr) => return Err(format!("invalid i2c address: {addr}")),
            }
            .map_err(|err| process_err(format!("{err:?}"), &tx, notify))?;

            send_debug(
                &tx,
                notify,
                format!("mpr121 init with bus={bus} and addr=0x{addr:02x}").as_str(),
            );

            let mut key_state: u16 = 0;

            while let Ok(crate::WorkerCommand::Command(req)) = rx.recv() {
                debug!("request: {req:?}");
                match req {
                    Request::ReadAll => match sensor.get_touched() {
                        Ok(keys) => {
                            send_reply(
                                Reply::AllTouches {
                                    touched: keys,
                                    previous: key_state,
                                    over_current: sensor.is_over_current_set().unwrap_or(false),
                                },
                                &tx,
                                notify,
                            );
                            key_state = keys;
                        }
                        Err(err) => {
                            process_err(format!("{err:?}"), &tx, notify);
                        }
                    },
                    Request::Reset => {
                        if let Err(err) = sensor.reset() {
                            process_err(format!("{err:?}"), &tx, notify);
                        }
                    }
                    Request::SetThresholds(on, off) => {
                        if let Err(err) = sensor.set_thresholds(on, off) {
                            process_err(format!("{err:?}"), &tx, notify);
                        }
                    }
                    Request::SetDebounce(on, off) => {
                        if let Err(err) = sensor.set_debounce(to_debounce(on)?, to_debounce(off)?) {
                            process_err(format!("{err:?}"), &tx, notify);
                        }
                    }
                    Request::GetFiltered(channel) => match sensor.get_filtered(to_channel(channel)?) {
                        Ok(value) => {
                            send_reply(Reply::Filtered { value, channel }, &tx, notify);
                        }
                        Err(err) => {
                            process_err(format!("{err:?}"), &tx, notify);
                        }
                    },
                    Request::GetBaseline(channel) => match sensor.get_baseline(to_channel(channel)?) {
                        Ok(value) => {
                            send_reply(Reply::Baseline { value, channel }, &tx, notify);
                        }
                        Err(err) => {
                            process_err(format!("{err:?}"), &tx, notify);
                        }
                    },
                }
            }

            log::info!("worker cycle done");

            if let Some(mut pin) = pin {
                if let Err(err) = pin.clear_async_interrupt() {
                    log::error!("async pin: {err}");
                }
                drop(pin);
                log::debug!("drop PIN");
            }

            Ok(())
        });

        Ok(Self { worker, cb: on_reply })
    }

    pub(crate) fn process_reply(mpr: *const Self) -> bool {
        if mpr.is_null() {
            error!("NULL Mpr121 pointer");
            false
        } else {
            let mpr: &hw_sensor_mpr121 = unsafe { &*mpr };

            mpr.worker.process_reply(&|rep| match rep {
                Reply::Message(level, msg) => {
                    mpr.worker.pd_message(level, &msg);
                }
                Reply::AllTouches {
                    touched,
                    previous,
                    over_current,
                } => {
                    mpr.cb.all_touches(touched, previous, over_current);
                }
                Reply::Filtered { value, channel } => {
                    mpr.cb.filtered(channel, value);
                }
                Reply::Baseline { value, channel } => {
                    mpr.cb.baseline(channel, value);
                }
            });

            true
        }
    }

    pub(crate) fn send_request(mpr: *const Self, req: Request) -> bool {
        if mpr.is_null() {
            error!("NULL mpr121 pointer");
            false
        } else {
            let mpr = unsafe { &*mpr };

            mpr.worker.send_request(req)
        }
    }

    pub(crate) fn free(mut self) {
        self.worker.quit();
    }
}
