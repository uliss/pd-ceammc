use std::{ffi::CString, time::Duration};

use log::{debug, error};
use rgb::bytemuck::Contiguous;
use rppal::gpio::Gpio;

use crate::{hw_msg_cb, hw_notify_cb, infrared::irp::get_irp};

use super::{hw_infrared, InfraredWorker, Reply, Request};

impl hw_infrared {
    pub fn new(pin: u8, notify: hw_notify_cb, on_err: hw_msg_cb) -> Result<Self, CString> {
        let (worker, rx, tx) = InfraredWorker::new(on_err);

        worker.spawn(tx.clone(), notify, move || {
            let gpio = Gpio::new().map_err(|err| format!("GPIO init error: {err}"))?;

            let mut ir_pin = gpio
                .get(pin)
                .map_err(|err| format!("GPIO init error: {err}"))?
                .into_input_pulldown();

            ir_pin
                .set_interrupt(rppal::gpio::Trigger::Both, None)
                .map_err(|err| format!("GPIO init error: {err}"))?;

            debug!("GPIO pin: {pin}");

            let mut prev_event = Duration::default();
            let mut packet = Vec::<i64>::new();
            let mut new_packet = true;

            let mut opt_err_tolerance = 100;

            'outer: loop {
                // debug!("listen for packet...");

                'inner: while let Ok(res) =
                    ir_pin.poll_interrupt(new_packet, Some(Duration::from_millis(30)))
                {
                    match res {
                        Some(event) => {
                            let delta = event.timestamp - prev_event;
                            new_packet = false;

                            match event.trigger {
                                rppal::gpio::Trigger::Disabled => {}
                                rppal::gpio::Trigger::RisingEdge => {
                                    if event.seqno > 1 {
                                        packet.push(delta.as_micros() as i64);
                                    }
                                }
                                rppal::gpio::Trigger::FallingEdge => {
                                    if event.seqno > 1 {
                                        packet.push(-(delta.as_micros() as i64));
                                    }
                                }
                                rppal::gpio::Trigger::Both => {}
                            }

                            prev_event = event.timestamp;
                        }
                        None => {
                            if !packet.is_empty() {
                                packet.push(-30000);

                                let x = packet
                                    .iter()
                                    .map(|x| x.to_string())
                                    .collect::<Vec<_>>()
                                    .join(" ");

                                debug!("{x}");

                                let options = irp::Options {
                                    aeps: opt_err_tolerance,
                                    eps: 30,
                                    max_gap: 20000,
                                    ..Default::default()
                                };

                                let irp = get_irp(crate::infrared::irp::Protocol::NEC);
                                let dfa = irp.compile(&options).expect("build dfa should succeed");

                                // Create a decoder with 100 microsecond tolerance, 30% relative tolerance,
                                // and 20000 microseconds maximum gap.
                                let mut decoder = irp::Decoder::new(options);

                                let data = packet.iter().map(|x| {
                                    if *x >= 0 {
                                        irp::InfraredData::Flash(*x as u32)
                                    } else {
                                        irp::InfraredData::Gap(x.abs() as u32)
                                    }
                                });

                                for ir in data {
                                    decoder.dfa_input(ir, &dfa, |event, vars| {
                                        println!("decoded: {event} {vars:?}");
                                    });
                                }

                                packet.clear();
                            }

                            new_packet = true;
                            break 'inner;
                        }
                    }
                }

                'req: loop {
                    match rx.try_recv() {
                        Ok(req) => match req {
                            crate::infrared::Request::Poll(_) => todo!(),
                            crate::infrared::Request::SetTolerance(usec) => {
                                opt_err_tolerance = usec.into();
                            }
                        },
                        Err(err) => match err {
                            std::sync::mpsc::TryRecvError::Empty => break 'req,
                            std::sync::mpsc::TryRecvError::Disconnected => break 'outer,
                        },
                    }
                }
            }

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

        //     let irp = Irp::parse(
        //         r#"
        //    {38.4k,564}<1,-1|1,-3>(16,-8,D:8,S:8,F:8,~F:8,E:8,~E:8,1,^108m)[D:0..255,S:0..255=255-D,F:0..255,E:0..255]"#,
        //     )
        //     .expect("parse should succeed");

        let dfa = irp.compile(&options).expect("build dfa should succeed");

        // Create a decoder with 100 microsecond tolerance, 30% relative tolerance,
        // and 20000 microseconds maximum gap.
        let mut decoder = irp::Decoder::new(options);

        // Set some values for D, S, and F
        let mut vars = Vartable::new();
        vars.set(String::from("D"), 255);
        vars.set(String::from("S"), 0xff);
        vars.set(String::from("F"), 1);
        // vars.set(String::from("E"), 23);
        // // encode message with 0 repeats
        let message = irp.encode_raw(vars, 1).expect("encode should succeed");
        if let Some(carrier) = &message.carrier {
            println!("carrier: {}Hz", carrier);
        }
        // if let Some(duty_cycle) = &message.duty_cycle {
        //     println!("duty cycle: {}%", duty_cycle);
        // }
        println!("{}", message.print_rawir());

        // let valid = "+9024 -4512
        //     +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -1692
        //     +564 -564  +564 -564  +564 -1692 +564 -564  +564 -1692 +564 -1692 +564 -564  +564 -564

        //     +564 -1692 +564 -564  +564 -564  +564 -564  +564 -564  +564 -564  +564 -564  +564 -564
        //     +564 -564  +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -1692 +564 -1692

        //     +564 -1692 +564 -1692 +564 -1692 +564 -564  +564 -1692 +564 -564  +564 -564  +564 -564
        //     +564 -564  +564 -564  +564 -564  +564 -1692 +564 -564  +564 -1692 +564 -1692 +564 -1692
        //     +564 -9300";

        // println!("test");
        for ir in InfraredData::from_rawir(msg).unwrap() {
            decoder.dfa_input(ir, &dfa, |event, vars| {
                println!("decoded: {} {:?}", event, vars);
            });
        }
    }
    //
}
