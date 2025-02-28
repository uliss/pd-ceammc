// use em
use std::{ffi::CString, thread::sleep, time::Duration};

use log::{debug, error};
use rotary_encoder_embedded::RotaryEncoder;
use rppal::gpio::Gpio;

use crate::{hw_msg_cb, hw_notify_cb};

use super::{hw_gpio_rotenc, hw_gpio_rotenc_data, Reply, Request};

impl hw_gpio_rotenc {
    pub fn new(
        dt: u8,
        clk: u8,
        btn: u8,
        init: f64,
        notify: hw_notify_cb,
        on_data: hw_gpio_rotenc_data,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("thread start");

            debug!("init Rotary Encoder with pins: dt={dt}, clk={clk}, btn={btn} and init value={init}");

            if dt == 0 || clk == 0 {
                debug!("invalid pins");
                return Ok(());
            }

            let gpio = Gpio::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            // Configure DT and CLK pins, typically pullup input
            let dt_pin = gpio
                .get(dt)
                .map_err(|err| {
                    error!("DT pin error: {err}");
                    err.to_string()
                })?
                .into_input_pullup();

            let clk_pin = gpio
                .get(clk)
                .map_err(|err| {
                    error!("CLK pin error: {err}");
                    err.to_string()
                })?
                .into_input_pullup();

            let mut rotary_encoder = RotaryEncoder::new(dt_pin, clk_pin).into_standard_mode();

            let mut value = init;
            let mut step = 1.0;

            // ...timer initialize at 900Hz to poll the rotary encoder
            loop {
                sleep(Duration::from_millis(1));
                let dir = rotary_encoder.update();

                match dir {
                    rotary_encoder_embedded::Direction::Clockwise => {
                        value += step;
                        Self::send_reply(&rep_tx, notify, Reply::Data(value, 1));
                    }
                    rotary_encoder_embedded::Direction::Anticlockwise => {
                        value -= step;
                        Self::send_reply(&rep_tx, notify, Reply::Data(value, -1));
                    }
                    _ => {}
                }

                match req_rx.try_recv() {
                    Ok(req) => match req {
                        Request::SetValue(val) => value = val,
                        Request::ResetValue => value = init,
                        Request::SetStep(val) => step = val,
                        Request::GetValue => {
                            Self::send_reply(&rep_tx, notify, Reply::Data(value, 0));
                        }
                    },
                    Err(err) => match err {
                        std::sync::mpsc::TryRecvError::Empty => {}
                        std::sync::mpsc::TryRecvError::Disconnected => {
                            break;
                        }
                    },
                }
            }

            debug!("thread done");
            Ok(())
        });

        Ok(hw_gpio_rotenc {
            tx: req_tx,
            rx: rep_rx,
            on_data,
            on_err,
        })
    }

    fn send_reply(tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb, rep: Reply) -> bool {
        if let Err(err) = tx.send(rep) {
            error!("send error: {err}");
            return false;
        }

        notify.notify();
        true
    }

    pub fn process_ptr(enc: *mut Self) -> bool {
        if enc.is_null() {
            return false;
        }

        let enc = unsafe { &*enc };
        while let Ok(rep) = enc.rx.try_recv() {
            match rep {
                Reply::Error(str) => enc.on_err.exec_raw(str.as_ptr()),
                Reply::Click => {}
                Reply::Data(value, dir) => {
                    (enc.on_data.cb)(enc.on_data.user, value, dir);
                }
            }
        }

        true
    }

    pub fn send_ptr(enc: *const Self, req: Request) -> bool {
        if enc.is_null() {
            error!("NULL encoder pointer");
            return false;
        }

        let enc = unsafe { &*enc };
        if let Err(err) = enc.tx.send(req) {
            let msg = format!("request send error: {err}");
            error!("{msg}");
            enc.on_err.exec(msg.as_str());
            return false;
        }

        true
    }
}
