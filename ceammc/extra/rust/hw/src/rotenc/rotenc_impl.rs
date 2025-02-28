use std::{ffi::CString, thread::sleep, time::Duration};

use log::{debug, error};
use rotary_encoder_embedded::RotaryEncoder;
use rppal::gpio::{Gpio, Trigger};

use crate::{hw_msg_cb, hw_notify_cb};

use super::{hw_gpio_rotenc, hw_gpio_rotenc_click, hw_gpio_rotenc_data, Reply, Request};

impl hw_gpio_rotenc {
    pub fn new(
        dt: u8,
        clk: u8,
        btn: u8,
        init: f64,
        step: f64,
        min_value: f64,
        max_value: f64,
        notify: hw_notify_cb,
        on_data: hw_gpio_rotenc_data,
        on_click: hw_gpio_rotenc_click,
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

            let mut btn_pin = None;
            if btn > 0 {
                let mut pin = gpio
                    .get(btn)
                    .map_err(|err| {
                        error!("BTN pin error: {err}");
                        err.to_string()
                    })?
                    .into_input_pullup();

                let rep_rx2 = rep_tx.clone();

                pin.set_async_interrupt(
                    rppal::gpio::Trigger::Both,
                    Some(Duration::from_millis(10)),
                    move |ev| match ev.trigger {
                        Trigger::RisingEdge => {
                            Self::send_reply(&rep_rx2, notify, Reply::Click(false));
                        }
                        Trigger::FallingEdge => {
                            Self::send_reply(&rep_rx2, notify, Reply::Click(true));
                        }
                        _ => {}
                    },
                )
                .map_err(|err| {
                    error!("button pin init error: {err}");
                    err.to_string()
                })?;

                btn_pin.replace(pin);
            }

            let mut rotary_encoder = RotaryEncoder::new(dt_pin, clk_pin).into_standard_mode();

            let mut enc_value = init;
            let mut enc_step = step;
            let mut enc_min = min_value;
            let mut enc_max = max_value;

            const DIR_NONE: i8 = 0;
            const DIR_INC: i8 = 1;
            const DIR_DEC: i8 = -1;

            // ...timer initialize at 900Hz to poll the rotary encoder
            loop {
                sleep(Duration::from_millis(1));
                let dir = rotary_encoder.update();

                match dir {
                    rotary_encoder_embedded::Direction::Clockwise => {
                        enc_value += enc_step;
                        enc_value = enc_value.clamp(enc_min, enc_max);
                        Self::send_reply(&rep_tx, notify, Reply::Data(enc_value, DIR_INC));
                    }
                    rotary_encoder_embedded::Direction::Anticlockwise => {
                        enc_value -= enc_step;
                        enc_value = enc_value.clamp(enc_min, enc_max);
                        Self::send_reply(&rep_tx, notify, Reply::Data(enc_value, DIR_DEC));
                    }
                    _ => {}
                }

                match req_rx.try_recv() {
                    Ok(req) => {
                        debug!("{req:?}");

                        match req {
                            Request::SetValue(val) => enc_value = val,
                            Request::ResetValue => enc_value = init,
                            Request::SetStep(val) => enc_step = val,
                            Request::GetValue => {
                                Self::send_reply(&rep_tx, notify, Reply::Data(enc_value, DIR_NONE));
                            }
                            Request::SetMin(min) => enc_min = min,
                            Request::SetMax(max) => enc_max = max,
                        }
                    }
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
            on_click,
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
                Reply::Click(state) => {
                    (enc.on_click.cb)(enc.on_click.user, state);
                }
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
