use std::ffi::CString;

use log::{debug, error};
use rppal::gpio::Gpio;

use crate::{hw_msg_cb, hw_notify_cb, infrared::irp::get_dfa, process_err};

use super::{hw_infrared, hw_infrared_key_cb, InfraredWorker, Reply, Request};

impl hw_infrared {
    pub fn new(
        pin: u8,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
        on_key: hw_infrared_key_cb,
    ) -> Result<Self, CString> {
        let (worker, rx, tx) = InfraredWorker::new(on_err);

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

                    if delta_usec > 50000 {
                        ir_tx.send(irp::InfraredData::Reset).unwrap_or_default();
                    }

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

            let mut opt_usec_tolerance = 100;
            let mut opt_max_gap = 30000;
            let mut opt_perc_tolerance = 30;

            let options = irp::Options {
                aeps: opt_usec_tolerance,
                eps: opt_perc_tolerance,
                max_gap: opt_max_gap,
                ..Default::default()
            };

            let mut dfa = get_dfa("NEC", &options)?;
            let mut decoder = irp::Decoder::new(options);

            'outer: loop {
                'chan_async: loop {
                    match ir_rx.try_recv() {
                        Ok(res) => {
                            decoder.dfa_input(res, &dfa, |_ev, vars| {
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
                        Ok(req) => {
                            debug!("{req:?}");

                            match req {
                                Request::SetToleranceUsec(usec) => opt_usec_tolerance = usec.into(),
                                Request::SetMaxGap(usec) => opt_max_gap = usec,
                                Request::SetTolerancePerc(perc) => opt_perc_tolerance = perc.into(),
                                Request::SetProtocol(proto) => {
                                    let options = irp::Options {
                                        aeps: opt_usec_tolerance,
                                        eps: opt_perc_tolerance,
                                        max_gap: opt_max_gap,
                                        ..Default::default()
                                    };

                                    dfa = get_dfa(proto.to_string_lossy().as_ref(), &options)
                                        .map_err(|err| process_err(err, &tx, notify))
                                        .unwrap_or_default();
                                    decoder = irp::Decoder::new(options);
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
                Reply::Error(err) => ir.worker.caller_error(&err),
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
