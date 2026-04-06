use std::{ffi::CString, thread::sleep, time::Duration};

use ceammc_rs_msg::{msg_cb, msg_notify};
use log::{debug, error};
use rotary_encoder_embedded::RotaryEncoder;
use rppal::gpio::{Gpio, Trigger};

use crate::{send_debug, send_reply};

use super::{hw_gpio_rotenc, hw_gpio_rotenc_click, hw_gpio_rotenc_data, Reply, Request, RotEncoderWorker};

impl hw_gpio_rotenc {
    pub fn new(
        dt: u8,
        clk: u8,
        btn: Option<u8>,
        init: f64,
        step: f64,
        min_value: f64,
        max_value: f64,
        notify: msg_notify,
        on_data: hw_gpio_rotenc_data,
        on_click: hw_gpio_rotenc_click,
        on_msg: msg_cb,
    ) -> Result<Self, CString> {
        let (mut worker, rx, tx) = RotEncoderWorker::new(on_msg, None);

        worker.spawn(tx.clone(), notify, move || {
            debug!("try to init Rotary Encoder with pins: dt={dt}, clk={clk}, btn={btn:?} and init value={init}");

            if dt == 0 || clk == 0 {
                debug!("invalid pins");
                return Ok(());
            }

            let gpio = Gpio::new().map_err(|err| err.to_string())?;

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
            if let Some(btn) = btn {
                let mut pin = gpio
                    .get(btn)
                    .map_err(|err| {
                        error!("BTN pin error: {err}");
                        err.to_string()
                    })?
                    .into_input_pullup();

                let tx2 = tx.clone();

                pin.set_async_interrupt(
                    rppal::gpio::Trigger::Both,
                    Some(Duration::from_millis(10)),
                    move |ev| match ev.trigger {
                        Trigger::RisingEdge => {
                            let _ = send_reply(Reply::Click(false), &tx2, notify);
                        }
                        Trigger::FallingEdge => {
                            let _ = send_reply(Reply::Click(true), &tx2, notify);
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

            send_debug(
                &tx,
                notify,
                format!("Rotary Encoder init done with pins: dt={dt}, clk={clk}, btn={btn:?} and init value={init}")
                    .as_str(),
            )
            .to_err()?;

            // ...timer initialize at 900Hz to poll the rotary encoder
            loop {
                sleep(Duration::from_millis(1));
                let dir = rotary_encoder.update();

                match dir {
                    rotary_encoder_embedded::Direction::Clockwise => {
                        enc_value += enc_step;
                        enc_value = enc_value.clamp(enc_min, enc_max);
                        send_reply(Reply::Data(enc_value, DIR_INC), &tx, notify).to_err()?;
                    }
                    rotary_encoder_embedded::Direction::Anticlockwise => {
                        enc_value -= enc_step;
                        enc_value = enc_value.clamp(enc_min, enc_max);
                        send_reply(Reply::Data(enc_value, DIR_DEC), &tx, notify).to_err()?;
                    }
                    _ => {}
                }

                match rx.try_recv() {
                    Ok(crate::WorkerCommand::Quit) => break,
                    Ok(crate::WorkerCommand::Command(req)) => {
                        debug!("{req:?}");

                        match req {
                            Request::SetValue(val) => enc_value = val,
                            Request::ResetValue => enc_value = init,
                            Request::SetStep(val) => enc_step = val,
                            Request::GetValue => {
                                send_reply(Reply::Data(enc_value, DIR_NONE), &tx, notify).to_err()?;
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
            //
            Ok(())
        });

        Ok(Self {
            worker,
            on_data,
            on_click,
        })
    }

    pub fn process_reply_ptr(enc: *mut Self) -> bool {
        if enc.is_null() {
            error!("NULL encoder pointer");
            return false;
        }

        let enc = unsafe { &*enc };
        enc.worker.process_reply(&|rep| match rep {
            Reply::Message(level, msg) => enc.worker.pd_message(level, &msg),
            Reply::Click(state) => {
                (enc.on_click.cb)(enc.on_click.user, state);
            }
            Reply::Data(value, dir) => {
                (enc.on_data.cb)(enc.on_data.user, value, dir);
            }
        })
    }

    pub fn send_request_ptr(enc: *const Self, req: Request) -> bool {
        if enc.is_null() {
            error!("NULL encoder pointer");
            return false;
        }

        let enc = unsafe { &*enc };
        enc.worker.send_request(req)
    }
}
