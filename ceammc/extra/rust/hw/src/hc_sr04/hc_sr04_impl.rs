use std::{ffi::CString, sync::{mpsc::TryRecvError, Arc, Condvar, Mutex}, time::Duration};

use log::{debug, error};
use rppal::gpio::{Event, Trigger};

use crate::{hc_sr04::{Reply, Request, HW_SR04_DEF_POLL_INTERVAL, HW_SR04_MAX_POLL_INTERVAL, HW_SR04_MIN_POLL_INTERVAL}, hw_msg_cb, hw_notify_cb};

use super::{hw_gpio_sr04, hw_sr04_cb};

impl hw_gpio_sr04 {
    pub fn new(
        trigger_pin: u8,
        echo_pin: u8,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
        on_data: hw_sr04_cb,
    ) -> Result<Self, CString> {
        let result = Arc::new((Mutex::new(None), Condvar::new()));
        let async_result = result.clone();
        let async_result2 = result.clone();

        let (tx, rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("thread start");

            let gpio = rppal::gpio::Gpio::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            debug!("GPIO: trigger_pin={trigger_pin}, echo=pin={echo_pin}");

            let mut trig_pin = gpio
                .get(trigger_pin)
                .map_err(|err| {
                    error!("trigger pin [{trigger_pin}]: {err}");
                    err.to_string()
                })?
                .into_output_low();

            trig_pin.set_reset_on_drop(false);

            std::thread::sleep(Duration::from_millis(50));
            debug!("gpio init done");

            let mut prev_event: Option<Event> = None;
            let mut echo_pin = gpio
                .get(echo_pin)
                .map_err(|err| {
                    error!("echo pin [{echo_pin}]: {err}");
                    err.to_string()
                })?
                .into_input_pulldown();

            echo_pin
                .set_async_interrupt(Trigger::Both, None, move |ev| match ev.trigger {
                    Trigger::RisingEdge => {
                        if prev_event.is_none()
                            || prev_event.is_some_and(|event| event.trigger == Trigger::FallingEdge)
                        {
                            prev_event.replace(ev);
                        } else {
                            debug!("unexpected up");
                        }
                    }
                    Trigger::FallingEdge => {
                        if prev_event.is_some_and(|event| event.trigger == Trigger::RisingEdge) {
                            let length = ev.timestamp.checked_sub(prev_event.unwrap().timestamp);
                            prev_event.replace(ev);

                            length
                                .map(|len| {
                                    // at 20 degrees celsius
                                    let sound_speed_cm_sec = 343.0 * 100.0;
                                    let distance_cm = sound_speed_cm_sec * 0.5 * len.as_secs_f64();

                                    let (m, cond) = async_result.as_ref();

                                    match m.lock() {
                                        Ok(mut mg) => {
                                            if len.as_millis() < 38 {
                                                mg.replace(Reply::Measure(distance_cm as f32));
                                            } else {
                                                mg.take();
                                            }
                                        }
                                        Err(err) => {
                                            error!("{err}");
                                        }
                                    }

                                    cond.notify_all();
                                })
                                .or_else(|| {
                                    error!("duration overflow");
                                    None
                                });
                        } else {
                            debug!("unexpected falling");
                        }
                    }
                    _ => {
                        debug!("event ???");
                    }
                })
                .map_err(|err| {
                    error!("{err}");
                    err.to_string()
                })?;

            let mut cycle_mode = false;
            let mut poll_interval_msec = HW_SR04_DEF_POLL_INTERVAL;
            loop {
                match rx.try_recv() {
                    Ok(req) => match req {
                        Request::Poll(state) => {
                            cycle_mode = state;
                        }
                        Request::OneShot => {
                            cycle_mode = false;
                            Self::trig_fire(
                                &mut trig_pin,
                                &async_result2,
                                notify,
                                HW_SR04_DEF_POLL_INTERVAL,
                            );
                        }
                        Request::SetPollTime(msec) => {
                            poll_interval_msec =
                                msec.clamp(HW_SR04_MIN_POLL_INTERVAL, HW_SR04_MAX_POLL_INTERVAL);
                            debug!("set poll interval: {poll_interval_msec}");
                        }
                    },
                    Err(err) => match err {
                        TryRecvError::Empty => {
                            std::thread::sleep(Duration::from_millis(55));
                        }
                        TryRecvError::Disconnected => break,
                    },
                };

                if cycle_mode {
                    Self::trig_fire(&mut trig_pin, &async_result2, notify, poll_interval_msec);
                }
            }

            debug!("thread done");
            Ok(())
        });

        // detach thread

        Ok(hw_gpio_sr04 {
            result,
            tx,
            on_err,
            on_data,
        })
    }

    fn trig_fire(
        pin: &mut rppal::gpio::OutputPin,
        var: &Arc<(Mutex<Option<Reply>>, std::sync::Condvar)>,
        notify: hw_notify_cb,
        poll_interval: u16,
    ) {
        // 10us impulse to start distance measure
        pin.set_high();
        std::thread::sleep(Duration::from_micros(10));
        pin.set_low();

        let (mtx, cond) = var.as_ref();

        let mut do_sync = false;

        match mtx.lock() {
            Ok(mg) => {
                // 50ms have passed or may be we have result
                match cond.wait_timeout(mg, Duration::from_millis(50)) {
                    Ok(res) => {
                        do_sync = true;
                        if res.1.timed_out() {
                            debug!("echo timeout");
                        }
                    }
                    Err(err) => {
                        error!("echo error: {err}");
                    }
                }
            }
            Err(err) => {
                error!("{err}");
            }
        }

        if do_sync {
            // debug!("measure done");
            notify.notify();
            std::thread::sleep(Duration::from_millis(poll_interval as u64));
        }
    }

    pub fn send(&self, req: Request) -> bool {
        if let Err(err) = self.tx.send(req) {
            error!("{err}");
            self.on_err.exec(err.to_string().as_str());
            false
        } else {
            true
        }
    }

    pub fn check_result(&self) {
        match self.result.0.try_lock() {
            Ok(res) => match res.as_ref() {
                Some(reply) => match reply {
                    Reply::Measure(res) => {
                        self.on_data.exec(*res, false);
                    }
                },
                None => {
                    self.on_data.exec(0.0, true);
                }
            },
            Err(err) => {
                self.on_err.exec(err.to_string().as_str());
            }
        }
    }
}
