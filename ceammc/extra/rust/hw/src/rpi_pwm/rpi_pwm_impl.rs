use std::{ffi::CString, time::Duration};

use log::{debug, error};
use rppal::{gpio::Gpio, pwm::Pwm, system::DeviceInfo};

use crate::{
    hw_msg_cb, hw_notify_cb,
    rpi_pwm::{Reply, Request},
    send_error, str_to_cstr,
};

use super::{hw_rpi_pwm, PwmWorker};

fn msec_to_pwm_time(msec: f64) -> Duration {
    Duration::from_secs_f64(0.000001 * msec.max(0.0))
}

impl hw_rpi_pwm {
    pub fn new(channel: i8, notify: hw_notify_cb, on_msg: hw_msg_cb) -> Result<Self, CString> {
        let channel = match channel {
            0 => rppal::pwm::Channel::Pwm0,
            1 => rppal::pwm::Channel::Pwm1,
            2 => rppal::pwm::Channel::Pwm2,
            3 => rppal::pwm::Channel::Pwm3,
            _ => {
                return Err(str_to_cstr(format!("invalid channel value: {channel}")));
            }
        };

        let (mut worker, rx, tx) = PwmWorker::new(on_msg, None);

        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let dev_info = DeviceInfo::new().map_err(|err| err.to_string())?;

            let (pwm_pin, pin_mode) = match dev_info.model() {
                rppal::system::Model::RaspberryPi5 => match channel {
                    rppal::pwm::Channel::Pwm0 => (12u8, rppal::gpio::Mode::Alt0),
                    rppal::pwm::Channel::Pwm1 => (13, rppal::gpio::Mode::Alt0),
                    rppal::pwm::Channel::Pwm2 => (18, rppal::gpio::Mode::Alt5),
                    rppal::pwm::Channel::Pwm3 => (19, rppal::gpio::Mode::Alt5),
                },
                _ => match channel {
                    rppal::pwm::Channel::Pwm0 => (12u8, rppal::gpio::Mode::Alt0),
                    rppal::pwm::Channel::Pwm1 => (13, rppal::gpio::Mode::Alt0),
                    _ => {
                        return Err(format!("unsupported PWM channel: {channel:?}"));
                    }
                },
            };

            debug!("using pin: {pwm_pin} at mode: {pin_mode:?}");

            let gpio = Gpio::new().map_err(|err| err.to_string())?;
            let mut pwm_pin = gpio.get(pwm_pin).map_err(|err| err.to_string())?.into_io(pin_mode);

            pwm_pin.set_reset_on_drop(true);

            let pwm = Pwm::new(channel).map_err(|err| err.to_string())?;

            debug!("init pwm done: {pwm:?}");

            while let Ok(crate::WorkerCommand::Command(req)) = rx.recv() {
                debug!("{req:?}");

                match req {
                    crate::rpi_pwm::Request::Enable(state) => {
                        if state { pwm.enable() } else { pwm.disable() }
                            .or_else(|err| send_error(&tx, notify, &err.to_string()).to_err())?;
                    }
                    crate::rpi_pwm::Request::SetFreq(freq, duty) => {
                        // this depends from PWM clock divider
                        // we assume divider equal 384
                        // gpio pwmc 384
                        pwm.set_frequency(1000.0 * freq, duty)
                            .or_else(|err| send_error(&tx, notify, &err.to_string()).to_err())?;
                    }
                    Request::SetPeriod(msec) => pwm
                        .set_period(msec_to_pwm_time(msec))
                        .or_else(|err| send_error(&tx, notify, &err.to_string()).to_err())?,
                    Request::SetPolarity(p) => pwm
                        .set_polarity(match p {
                            crate::rpi_pwm::hw_rpi_pwm_polarity::NORMAL => rppal::pwm::Polarity::Normal,
                            crate::rpi_pwm::hw_rpi_pwm_polarity::INVERSE => rppal::pwm::Polarity::Inverse,
                        })
                        .or_else(|err| send_error(&tx, notify, &err.to_string()).to_err())?,
                    Request::SetPulseWidth(msec) => {
                        pwm.set_pulse_width(msec_to_pwm_time(msec))
                            .or_else(|err| send_error(&tx, notify, &err.to_string()).to_err())?;
                    }
                    Request::SetDutyCycle(duty) => pwm
                        .set_duty_cycle(duty)
                        .or_else(|err| send_error(&tx, notify, &err.to_string()).to_err())?,
                    Request::SetPwm(period_ms, width_ms) => {
                        let period_ms = period_ms.max(0.0);
                        let width_ms = width_ms.clamp(0.0, period_ms);
                        pwm.set_pulse_width(Duration::from_secs(0))
                            .or_else(|err| send_error(&tx, notify, &err.to_string()).to_err())?;
                        pwm.set_period(msec_to_pwm_time(period_ms))
                            .or_else(|err| send_error(&tx, notify, &err.to_string()).to_err())?;
                        pwm.set_pulse_width(msec_to_pwm_time(width_ms))
                            .or_else(|err| send_error(&tx, notify, &err.to_string()).to_err())?;
                    }
                }
            }

            debug!("thread done");
            Ok(())
        });

        Ok(Self { worker })
    }

    pub fn send_request_ptr(pwm: *const Self, req: Request) -> bool {
        if pwm.is_null() {
            error!("NULL pwm pointer");
            return false;
        }

        let pwm = unsafe { &*pwm };
        pwm.worker.send_request(req)
    }

    pub fn process_reply_ptr(pwm: *const Self) -> bool {
        if pwm.is_null() {
            error!("NULL pwm pointer");
            return false;
        }

        let pwm = unsafe { &*pwm };
        pwm.worker.process_reply(&|rep| match rep {
            Reply::Message(level, msg) => {
                pwm.worker.pd_message(level, &msg);
            }
        });

        true
    }
}
