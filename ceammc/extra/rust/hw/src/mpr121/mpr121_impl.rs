use log::{debug, error};
use mpr121_hal::{mpr121::Mpr121, Mpr121Address};
use rppal::hal::Delay;
use std::ffi::CString;

use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::I2cAddress,
    mpr121::{hw_mpr121_key_cb, hw_sensor_mpr121, Mpr212SensorWorker, Reply, Request},
    process_err, send_debug, send_reply,
};

impl hw_sensor_mpr121 {
    pub(crate) fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: hw_notify_cb,
        on_msg: hw_msg_cb,
        on_key: hw_mpr121_key_cb,
    ) -> Result<Self, CString> {
        let (worker, rx, tx) = Mpr212SensorWorker::new(on_msg);

        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let i2c = crate::i2c::i2c_impl::create_i2c_bus(i2c_bus, &tx, notify)?;
            debug!("i2c init: {i2c:?}");

            let bus = i2c.bus();
            let mut delay = Delay::new();
            let mut sensor = match i2c_addr {
                I2cAddress::Addr(_addr) => Mpr121::new(i2c, Mpr121Address::Default, &mut delay, true),
                I2cAddress::Default => Mpr121::new_default(i2c, &mut delay),
                I2cAddress::Auto => Mpr121::new_default(i2c, &mut delay),
                I2cAddress::Alt => return Err(format!("no alternative device address")),
                I2cAddress::Invalid(addr) => return Err(format!("invalid i2c address: {addr}")),
            }
            .map_err(|err| process_err(format!("{err:?}"), &tx, notify))?;

            send_debug(
                &tx,
                notify,
                format!("mpr121 init with bus={bus} and addr={i2c_addr:?}").as_str(),
            );

            while let Ok(req) = rx.recv() {
                match req {
                    Request::ReadAll => match sensor.get_touched() {
                        Ok(res) => {
                            debug!("all keys: {res:b}");
                            send_reply(Reply::AllKeys(res), &tx, notify);
                        }
                        Err(err) => {
                            process_err(format!("{err:?}"), &tx, notify);
                        }
                    },
                }
            }

            Ok(())
        });

        Ok(Self { worker, cb: on_key })
    }

    pub(crate) fn process_reply(mpr: *const Self) -> bool {
        if mpr.is_null() {
            error!("NULL Mpr121 pointer");
            false
        } else {
            let mpr: &hw_sensor_mpr121 = unsafe { &*mpr };

            mpr.worker.process_reply(&|rep| match rep {
                Reply::Message(level, msg) => {
                    mpr.worker.pd_message(level, &msg);
                }
                Reply::AllKeys(st) => {
                    mpr.cb.all_keys(st);
                }
                Reply::Key => todo!(),
            });

            true
        }
    }

    pub(crate) fn send_request(mpr: *const Self, req: Request) -> bool {
        if mpr.is_null() {
            error!("NULL mpr121 pointer");
            false
        } else {
            let mpr = unsafe { &*mpr };

            mpr.worker.send_request(req)
        }
    }
}
