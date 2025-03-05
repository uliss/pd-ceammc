use std::{ffi::CString, time::Duration};

use log::{debug, error};
use rppal::pwm::Pwm;

use crate::{
    hw_msg_cb, hw_notify_cb,
    rpi_pwm::{Reply, Request},
};

use super::hw_rpi_pwm;

impl hw_rpi_pwm {
    pub fn new(channel: u8, _notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
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

            let pwm = Pwm::new(channel).map_err(|err| {
                error!("{err}");
                CString::new(err.to_string()).unwrap_or_default()
            })?;

            let pi = wiringpi::setup_gpio();
            pi.pwm_pin();

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
