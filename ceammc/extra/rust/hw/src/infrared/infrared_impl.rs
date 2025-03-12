use std::{
    ffi::CString,
    sync::{atomic::Ordering, Arc},
    time::Duration,
};

use log::{debug, error};
use rppal::gpio::Gpio;

use crate::{hw_msg_cb, hw_notify_cb, send_reply};

use super::{hw_infrared, InfraredWorker, Reply};

impl hw_infrared {
    pub fn new(pin: u8, notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
        let (worker, rx, tx) = InfraredWorker::new(on_err);

        worker.spawn(tx.clone(), notify, move || {
            let gpio = Gpio::new().map_err(|err| format!("GPIO init error: {err}"))?;

            let mut ir_pin = gpio
                .get(pin)
                .map_err(|err| format!("GPIO init error: {err}"))?
                .into_input_pullup();

            debug!("GPIO pin: {pin}");

            let mut prev_event = Duration::default();
            let mut packet = Vec::<i64>::new();

            'outer: loop {
                'inner: while let Ok(res) =
                    ir_pin.poll_interrupt(packet.is_empty(), Some(Duration::from_millis(50)))
                {
                    match res {
                        Some(event) => {
                            let delta = event.timestamp - prev_event;

                            match event.trigger {
                                rppal::gpio::Trigger::Disabled => {}
                                rppal::gpio::Trigger::RisingEdge => {
                                    packet.push(delta.as_micros() as i64);
                                }
                                rppal::gpio::Trigger::FallingEdge => {
                                    packet.push(-(delta.as_micros() as i64));
                                }
                                rppal::gpio::Trigger::Both => {}
                            }

                            prev_event = event.timestamp;
                        }
                        None => {
                            let x = packet
                                .iter()
                                .map(|x| x.to_string())
                                .collect::<Vec<_>>()
                                .join(" ");

                            debug!("{x}");

                            packet.clear();
                            break 'inner;
                        }
                    }
                }

                loop {
                    match rx.try_recv() {
                        Ok(req) => match req {
                            super::Request::Poll(_) => todo!(),
                        },
                        Err(err) => match err {
                            std::sync::mpsc::TryRecvError::Empty => {}
                            std::sync::mpsc::TryRecvError::Disconnected => break 'outer,
                        },
                    }
                }
            }

            // while let Ok(req) = rx.recv() {
            //     match req {
            //         super::Request::Poll(state) => {
            //             let _ = if state {
            //                 // let prev_rising = prev_rising.clone();
            //                 let tx = tx.clone();
            //                 let prev_event_time = prev_event_time.clone();

            //                 pin.set_async_interrupt(
            //                     rppal::gpio::Trigger::Both,
            //                     None,
            //                     move |event| {
            //                         let dur = event.timestamp - *prev_event_time.read().unwrap();

            //                         match event.trigger {
            //                             rppal::gpio::Trigger::Disabled => {}
            //                             rppal::gpio::Trigger::RisingEdge => {
            //                                 send_reply(
            //                                     Reply::Data(dur.as_millis() as i64),
            //                                     &tx,
            //                                     notify,
            //                                 );
            //                             }
            //                             rppal::gpio::Trigger::FallingEdge => {
            //                                 send_reply(
            //                                     Reply::Data(-(dur.as_millis() as i64)),
            //                                     &tx,
            //                                     notify,
            //                                 );
            //                             }
            //                             rppal::gpio::Trigger::Both => {}
            //                         }

            //                         *prev_event_time.write().unwrap() = event.timestamp;
            //                     },
            //                 )
            //             } else {
            //                 pin.clear_async_interrupt()
            //             }
            //             .map_err(|err| format!("GPIO init error: {err}"))?;
            //         }
            //     }
            // }

            Ok(())
        });

        Ok(Self { worker })
    }

    pub fn process_reply_ptr(ir: *const Self) -> bool {
        if ir.is_null() {
            error!("NULL IR pointer");
            false
        } else {
            let ir = unsafe { &*ir };
            ir.worker.process_reply(&|rep| match rep {
                Reply::Error(err) => ir.worker.caller_error(&err),
                Reply::Data(data) => {
                    debug!("data: {data}");
                }
            })
        }
    }
}
