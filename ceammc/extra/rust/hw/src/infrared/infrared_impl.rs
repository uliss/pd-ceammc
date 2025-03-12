use std::{ffi::CString, sync::Arc, time::Duration, vec};

use embedded_hal::digital::InputPin;
use log::{debug, error};
use palette::num::SaturatingSub;
use rppal::gpio::Gpio;

use crate::{hw_msg_cb, hw_notify_cb, infrared::irp::get_irp, send_reply};

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

            let mut opt_err_tolerance = 100;
            let mut opt_max_gap = 30000;
            let mut opt_perc_tolerance = 30;

            let mut prev_event_usec = 0u128;
            let mut edges = Vec::<irp::InfraredData>::new();

            ir_pin
                .set_async_interrupt(rppal::gpio::Trigger::Both, None, move |event| {
                    let event_usec = event.timestamp.as_micros();
                    let delta_usec = event_usec.saturating_sub(prev_event_usec);

                    if delta_usec >= 50_000 {
                        if !edges.is_empty() {
                            debug!("long event");
                            let irp = get_irp(crate::infrared::irp::Protocol::NEC);

                            let options = irp::Options {
                                aeps: opt_err_tolerance,
                                eps: opt_perc_tolerance,
                                max_gap: opt_max_gap,
                                ..Default::default()
                            };
                            let dfa = irp.compile(&options).expect("build dfa should succeed");
                            let mut decoder = irp::Decoder::new(options);

                            for ir in &edges {
                                decoder.dfa_input(*ir, &dfa, |event, vars| {
                                    debug!("event: {event} {vars:?}");
                                    // num_decoded += 1;

                                    // for (k, v) in &vars {
                                    //     send_reply(
                                    //         Reply::Reply(
                                    //             CString::new(k.as_str()).unwrap_or_default(),
                                    //             *v,
                                    //         ),
                                    //         &tx,
                                    //         notify,
                                    //     );
                                    // }            // let ir = infrared::Receiver::builder()
                                    //     .frequency(20_000)
                                    //     .nec()
                                    //     .pin(ir_pin)
                                    //     .build();
                                    // let ir = infrared::PeriodicPoll::with_pin(20_000, ir_pin);

                                    // infrared::PeriodicPoll::with_input(0, ir_pin);
                                    // let r1= infrared::Receiver::with_
                                    // infraredpin(40_000, ir_pin);

                                    // let r2: infrared::PeriodicPoll<infrared::protocol::Nec, InputPin> =
                                    //     infrared::PeriodicPoll::with_pin(40_000, ir_pin);

                                    // let mut r3: BufferInputReceiver<Rc6> = BufferInputReceiver::with_frequenzy(20_000);

                                    // let buf: &[u32] = &[20, 40, 20];
                                    // let cmd_iter = r3.iter(buf);
                                });
                            }

                            debug!("{edges:?}");

                            edges.clear();
                            return;
                        }
                    }

                    match event.trigger {
                        rppal::gpio::Trigger::RisingEdge => {
                            edges.push(irp::InfraredData::Flash(delta_usec as u32));
                        }
                        rppal::gpio::Trigger::FallingEdge => {
                            edges.push(irp::InfraredData::Gap(delta_usec as u32));
                        }
                        _ => {}
                    }

                    prev_event_usec = event_usec;
                })
                .map_err(|err| format!("GPIO init error: {err}"))?;

            'outer: loop {
                'req: loop {
                    match rx.try_recv() {
                        Ok(req) => {
                            debug!("{req:?}");

                            match req {
                                Request::SetToleranceUsec(usec) => opt_err_tolerance = usec.into(),
                                Request::SetMaxGap(usec) => opt_max_gap = usec,
                                Request::SetTolerancePerc(perc) => opt_perc_tolerance = perc.into(),
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
                Reply::Data(data) => {
                    debug!("data: {data}");
                }
                Reply::Reply(key, value) => (ir.on_key.cb)(ir.on_key.user, key.as_ptr(), value),
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

#[cfg(test)]
mod test {
    use irp::{InfraredData, Message, Vartable};

    use crate::infrared::irp::get_irp;

    #[test]
    fn test_decode() {
        let msg = "9092 -4582 
        540 -605  538 -582  537 -608  541 -603  512 -607  541 -603  513 -634  513 -606
        541 -1744 545 -1715 533 -1726 542 -1743 540 -1721 511 -1745 541 -1746 512 -1746

        511 -1747 538 -609  504 -1752 540 -605  538 -1720 538 -607  516 -628  513 -606
        539 -606  513 -1745 537 -607  514 -1743 537 -608  510 -1748 538 -1747 512 -1745 +564 -30732";
        let rawir = Message::parse(msg).expect("parse should succeed");
        assert_eq!(
            rawir.raw,
            vec![
                9092, 4582, //
                540, 605, 538, 582, 537, 608, 541, 603, 512, 607, 541, 603, 513, 634, 513, 606,
                541, 1744, 545, 1715, 533, 1726, 542, 1743, 540, 1721, 511, 1745, 541, 1746, 512,
                1746, 511, 1747, 538, 609, 504, 1752, 540, 605, 538, 1720, 538, 607, 516, 628, 513,
                606, 539, 606, 513, 1745, 537, 607, 514, 1743, 537, 608, 510, 1748, 538, 1747, 512,
                1745, 564, 30732
            ]
        );

        let options = irp::Options {
            aeps: 100,
            eps: 30,
            max_gap: 20000,
            ..Default::default()
        };

        let irp = get_irp(crate::infrared::irp::Protocol::NEC);
        let dfa = irp.compile(&options).expect("build dfa should succeed");

        // Create a decoder with 100 microsecond tolerance, 30% relative tolerance,
        // and 20000 microseconds maximum gap.
        let mut decoder = irp::Decoder::new(options);

        // Set some values for D, S, and F
        let mut vars = Vartable::new();
        vars.set(String::from("D"), 255);
        vars.set(String::from("S"), 0xff);
        vars.set(String::from("F"), 1);
        let message = irp.encode_raw(vars, 1).expect("encode should succeed");
        if let Some(carrier) = &message.carrier {
            println!("carrier: {}Hz", carrier);
        }
        println!("{}", message.print_rawir());
        for ir in InfraredData::from_rawir(msg).unwrap() {
            decoder.dfa_input(ir, &dfa, |event, vars| {
                println!("decoded: {} {:?}", event, vars);
            });
        }
    }
    //
}
