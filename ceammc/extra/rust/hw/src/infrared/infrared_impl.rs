use std::ffi::CString;

use log::{debug, error};
use rppal::gpio::Gpio;

use crate::{hw_msg_cb, hw_notify_cb, infrared::irp::get_decoder, process_err};

use super::{hw_infrared, hw_infrared_key_cb, InfraredWorker, Reply, Request};

impl hw_infrared {
    pub fn new(pin: u8, notify: hw_notify_cb, on_msg: hw_msg_cb, on_key: hw_infrared_key_cb) -> Result<Self, CString> {
        let (mut worker, rx, tx) = InfraredWorker::new(on_msg);

        worker.spawn(tx.clone(), notify, move || {
            let gpio = Gpio::new().map_err(|err| format!("GPIO init error: {err}"))?;

            let mut ir_pin = gpio
                .get(pin)
                .map_err(|err| format!("GPIO init error: {err}"))?
                .into_input_pulldown();

            debug!("GPIO pin: {pin}");

            // async move
            let (ir_tx, ir_rx) = std::sync::mpsc::channel();
            let mut prev_event_usec = 0u128;

            ir_pin
                .set_async_interrupt(rppal::gpio::Trigger::Both, None, move |event| {
                    let event_usec = event.timestamp.as_micros();
                    let delta_usec = event_usec.saturating_sub(prev_event_usec);

                    match event.trigger {
                        rppal::gpio::Trigger::RisingEdge => {
                            ir_tx
                                .send(irp::InfraredData::Flash(delta_usec as u32))
                                .unwrap_or_default();
                        }
                        rppal::gpio::Trigger::FallingEdge => {
                            ir_tx
                                .send(irp::InfraredData::Gap(delta_usec as u32))
                                .unwrap_or_default();
                        }
                        _ => {}
                    }

                    prev_event_usec = event_usec;
                })
                .map_err(|err| format!("GPIO init error: {err}"))?;

            let mut proto_name = "NEC".to_owned();
            let (mut dfa, mut dec) = get_decoder(proto_name)?;

            'outer: loop {
                'chan_async: loop {
                    match ir_rx.try_recv() {
                        Ok(res) => {
                            debug!("{res}");

                            dec.dfa_input(res, &dfa, |_ev, vars| {
                                let mut keys = vars.keys().collect::<Vec<_>>();
                                keys.sort();

                                for k in &keys {
                                    if let Some(v) = vars.get(*k) {
                                        let key = CString::new(k.as_str()).unwrap_or_default();
                                        if let Err(err) = tx.send(Reply::Key(key, *v)) {
                                            error!("send error: {err}");
                                            break;
                                        }
                                    }
                                }

                                notify.notify();
                            });
                        }
                        Err(err) => match err {
                            std::sync::mpsc::TryRecvError::Empty => break 'chan_async,
                            std::sync::mpsc::TryRecvError::Disconnected => break 'outer,
                        },
                    }
                }

                'req: loop {
                    match rx.try_recv() {
                        Ok(crate::WorkerCommand::Quit) => break 'outer,
                        Ok(crate::WorkerCommand::Command(req)) => {
                            debug!("{req:?}");

                            match req {
                                Request::SetProtocol(proto) => {
                                    proto_name = proto.to_string_lossy().to_string();

                                    if let Ok((a, b)) =
                                        get_decoder(proto_name).map_err(|err| process_err(err, &tx, notify))
                                    {
                                        dfa = a;
                                        dec = b;
                                    }
                                }
                            }
                        }
                        Err(err) => match err {
                            std::sync::mpsc::TryRecvError::Empty => break 'req,
                            std::sync::mpsc::TryRecvError::Disconnected => break 'outer,
                        },
                    }
                }
            }

            Ok(())
        });

        Ok(Self { worker, on_key })
    }

    pub fn process_reply_ptr(ir: *const Self) -> bool {
        if ir.is_null() {
            error!("NULL IR pointer");
            false
        } else {
            let ir = unsafe { &*ir };
            ir.worker.process_reply(&|rep| match rep {
                Reply::Message(level, err) => ir.worker.pd_message(level, &err),
                Reply::Key(key, value) => (ir.on_key.cb)(ir.on_key.user, key.as_ptr(), value),
            })
        }
    }

    pub fn send_request_ptr(ir: *const Self, req: Request) -> bool {
        if ir.is_null() {
            error!("NULL IR pointer");
            false
        } else {
            let ir = unsafe { &*ir };
            ir.worker.send_request(req)
        }
    }
}
