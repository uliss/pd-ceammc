use std::{ffi::CString, time::Duration};

use log::{debug, error};
use rppal::{gpio::Gpio, pwm::Pwm, system::DeviceInfo};

use crate::{
    hw_msg_cb, hw_notify_cb,
    rpi_pwm::{Reply, Request}, str_to_cstr,
};

use super::hw_rpi_pwm;

fn send_reply(tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb, rep: Reply) -> bool {
    if let Err(err) = tx.send(rep) {
        error!("send error: {err}");
        false
    } else {
        notify.notify();
        true
    }
}

fn send_error<T>(tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb, msg: T) -> CString
where
    T: Into<Vec<u8>>,
{
    let cstr = str_to_cstr(msg);
    error!("{}", cstr.to_str().unwrap());
    send_reply(tx, notify, Reply::Error(cstr.clone()));
    cstr
}

impl hw_rpi_pwm {
    pub fn new(channel: u8, notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
        let channel = match channel {
            0 => rppal::pwm::Channel::Pwm0,
            1 => rppal::pwm::Channel::Pwm1,
            2 => rppal::pwm::Channel::Pwm2,
            3 => rppal::pwm::Channel::Pwm3,
            _ => {
                return Err(
                    CString::new(format!("invalid channel value: {channel}")).unwrap_or_default()
                );
            }
        };

        let (tx, rx) = std::sync::mpsc::channel();
        let (rep_tx, _rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), CString> {
            debug!("thread start");

            let dev_info = DeviceInfo::new()
                .map_err(|err| send_error(&rep_tx, notify, err.to_string().as_str()))?;

            let (pwm_pin, pin_mode) = match dev_info.model() {
                rppal::system::Model::RaspberryPi5 => match channel {
                    rppal::pwm::Channel::Pwm0 => (12u8, rppal::gpio::Mode::Alt0),
                    rppal::pwm::Channel::Pwm1 => (13, rppal::gpio::Mode::Alt0),
                    rppal::pwm::Channel::Pwm2 => (18, rppal::gpio::Mode::Alt3),
                    rppal::pwm::Channel::Pwm3 => (19, rppal::gpio::Mode::Alt3),
                },
                _ => match channel {
                    rppal::pwm::Channel::Pwm0 => (12u8, rppal::gpio::Mode::Alt0),
                    rppal::pwm::Channel::Pwm1 => (13, rppal::gpio::Mode::Alt0),
                    _ => {
                        return Err(send_error(
                            &rep_tx,
                            notify,
                            format!("unsupported PWM channel: {channel:?}"),
                        ));
                    }
                },
            };

            debug!("using pin: {pwm_pin} at mode: {pin_mode:?}");

            let gpio = Gpio::new().map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;
            let mut pwm_pin = gpio
                .get(pwm_pin)
                .map_err(|err| send_error(&rep_tx, notify, err.to_string()))?
                .into_io(pin_mode);

            pwm_pin.set_reset_on_drop(true);

            let pwm =
                Pwm::new(channel).map_err(|err| send_error(&rep_tx, notify, err.to_string()))?;

            debug!("init pwm done: {pwm:?}");

            while let Ok(req) = rx.recv() {
                debug!("{req:?}");

                let proc = |req: Request| -> Result<(), rppal::pwm::Error> {
                    match req {
                        crate::rpi_pwm::Request::Enable(state) => {
                            if state {
                                pwm.enable()?;
                            } else {
                                pwm.disable()?;
                            }
                        }
                        crate::rpi_pwm::Request::SetFreq(freq, duty) => {
                            pwm.set_frequency(freq, duty)?;
                        }
                        Request::SetPeriod(msec) => {
                            pwm.set_period(Duration::from_secs_f64(msec.abs() * 0.001))?
                        }
                        Request::SetPolarity(p) => pwm.set_polarity(match p {
                            crate::rpi_pwm::hw_rpi_pwm_polarity::NORMAL => {
                                rppal::pwm::Polarity::Normal
                            }
                            crate::rpi_pwm::hw_rpi_pwm_polarity::INVERSE => {
                                rppal::pwm::Polarity::Inverse
                            }
                        })?,
                        Request::SetPulseWidth(msec) => {
                            pwm.set_pulse_width(Duration::from_secs_f64(msec.abs() * 0.001))?
                        }
                        Request::SetDutyCycle(duty) => pwm.set_duty_cycle(duty)?,
                    }

                    debug!(
                        "enabled: {}, freq={}Hz, period={}ms, duty={}% pulse_width={}ms polarity={}",
                        pwm.is_enabled()?,
                        pwm.frequency()?,
                        pwm.period()?.as_secs_f32() * 1000.0,
                        pwm.duty_cycle()? * 100.0,
                        pwm.pulse_width()?.as_millis(),
                        pwm.polarity()?
                    );

                    Ok(())
                };

                if let Err(err) = proc(req) {
                    error!("{err}");
                    if let Err(err) = rep_tx.send(Reply::Error(
                        CString::new(err.to_string()).unwrap_or_default(),
                    )) {
                        error!("send error: {err}");
                    }
                }
            }

            debug!("thread done");
            Ok(())
        });

        Ok(hw_rpi_pwm {
            tx,
            // rx: rep_rx,
            on_err,
        })
    }

    pub fn send_ptr(pwm: *const Self, req: Request) -> bool {
        if pwm.is_null() {
            error!("NULL pwm pointer");
            return false;
        }

        let pwm = unsafe { &*pwm };
        if let Err(err) = pwm.tx.send(req) {
            pwm.on_err.exec(format!("request error: {err}").as_str());
            return false;
        }

        true
    }
}
