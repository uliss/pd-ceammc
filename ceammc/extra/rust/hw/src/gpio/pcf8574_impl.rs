use std::ffi::CString;

use ceammc_rs_msg::{msg_cb, msg_notify};
use log::error;
use pcf857x::PinFlag;
use pcf857x::{Pcf8574, SlaveAddr};

use crate::i2c::i2c_impl::try_i2c_device;
use crate::{
    i2c::{i2c_impl::create_i2c_bus, I2cAddress},
    pcf8574::{hw_pcf8574, hw_pcf8574_cb, Reply, Request},
};

fn to_i2c_addr(addr: &SlaveAddr) -> u16 {
    match *addr {
        SlaveAddr::Default => 0x20,
        SlaveAddr::Alternative(false, false, false) => 0x20,
        SlaveAddr::Alternative(false, false, true) => 0x21,
        SlaveAddr::Alternative(false, true, false) => 0x22,
        SlaveAddr::Alternative(false, true, true) => 0x23,
        SlaveAddr::Alternative(true, false, false) => 0x24,
        SlaveAddr::Alternative(true, false, true) => 0x25,
        SlaveAddr::Alternative(true, true, false) => 0x26,
        SlaveAddr::Alternative(true, true, true) => 0x27,
    }
}

impl hw_pcf8574 {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: msg_notify,
        on_msg: msg_cb,
        on_data: hw_pcf8574_cb,
    ) -> Result<Self, CString> {
        let worker = ceammc_rs_msg::Client::<Request, Reply>::start_worker(
            move |channel| {
                let mut i2c = create_i2c_bus(i2c_bus)?;
                let addr = match i2c_addr {
                    I2cAddress::Default => SlaveAddr::Default,
                    I2cAddress::Alt => SlaveAddr::Alternative(true, false, false),
                    I2cAddress::Auto => SlaveAddr::Default,
                    I2cAddress::Invalid(_) => Err(format!("invalid i2c address"))?,
                    I2cAddress::Addr(val) => match val {
                        0x20 => SlaveAddr::Default,
                        0x21 => SlaveAddr::Alternative(false, false, true),
                        0x22 => SlaveAddr::Alternative(false, true, false),
                        0x23 => SlaveAddr::Alternative(false, true, true),
                        0x24 => SlaveAddr::Alternative(true, false, false),
                        0x25 => SlaveAddr::Alternative(true, false, true),
                        0x26 => SlaveAddr::Alternative(true, true, false),
                        0x27 => SlaveAddr::Alternative(true, true, true),
                        _ => Err(format!("invalid i2c address: 0x{val:02x}"))?,
                    },
                };

                try_i2c_device(
                    &mut i2c,
                    to_i2c_addr(&addr),
                    crate::i2c::i2c_impl::DetectMethod::ReceiveByte,
                )?;

                let mut device = Pcf8574::new(i2c, addr);
                channel.send_debug(format!("connected to i2c pcf8574 device with addr: {addr:?} ..."))?;

                if let Err(err) = channel.recv_loop(&mut |req| {
                    match req {
                        Request::SetPins(pins) => {
                            device.set(pins).map_err(|err| format!("{err:?}"))?;
                        }
                        Request::GetPins { mask } => {
                            let mut flags = vec![];
                            match mask {
                                x if x & 0b0000_0001 > 0 => flags.push(PinFlag::P0),
                                x if x & 0b0000_0010 > 0 => flags.push(PinFlag::P1),
                                x if x & 0b0000_0100 > 0 => flags.push(PinFlag::P2),
                                x if x & 0b0000_1000 > 0 => flags.push(PinFlag::P3),
                                x if x & 0b0001_0000 > 0 => flags.push(PinFlag::P4),
                                x if x & 0b0010_0000 > 0 => flags.push(PinFlag::P5),
                                x if x & 0b0100_0000 > 0 => flags.push(PinFlag::P6),
                                x if x & 0b1000_0000 > 0 => flags.push(PinFlag::P7),
                                _ => {}
                            }

                            if let Some(pin_mask) = flags.iter().copied().reduce(|x, y| x | y) {
                                let state = device.get(pin_mask).map_err(|err| format!("{err:?}"))?;
                                channel.send_data(Reply::InputPins { mask, state }).to_worker_result()?;
                            }
                        }
                    }
                    Ok(())
                }) {
                    channel.send_error(err)?;
                }
                Ok(())
            },
            32,
            notify,
            on_msg,
        );

        Ok(Self { worker, cb: on_data })
    }

    pub fn process_reply_ptr(dev: *mut Self) -> bool {
        if dev.is_null() {
            error!("NULL device pointer");
            return false;
        }

        let dev = unsafe { &*dev };
        dev.worker.recv_loop(|rep| match rep {
            Reply::InputPins { mask, state } => {
                dev.cb.input_pins(mask, state);
            }
        });
        true
    }

    pub fn send_request_ptr(dev: *mut Self, req: Request) -> bool {
        if dev.is_null() {
            error!("NULL device pointer");
            return false;
        }

        let dev = unsafe { &*dev };
        dev.worker.send(req)
    }
}
