use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::I2cAddress,
    nfc::{hw_nfc_pn532, hw_nfc_pn532_cb, pn532_timer::Timer, NfcWorker, Reply, Request},
    process_err, send_debug,
};
use log::{debug, error};
use pn532::requests::SAMMode;
use pn532::{i2c::I2CInterface, Pn532};
use rppal::i2c::I2c;
use std::{ffi::CString, time::Duration};

fn try_device(i2c: &mut I2c, i2c_addr: u16) -> Result<(), String> {
    i2c.set_slave_address(i2c_addr).map_err(|err| err.to_string())?;
    i2c.set_timeout(10).map_err(|err| err.to_string())?;
    let mut read_buf = [0u8; 1];
    match i2c.write_read(&[0x00], &mut read_buf) {
        Ok(_) => Ok(()),
        Err(_) => {
            let msg = format!("device is not response: 0x{:02x}", i2c_addr);
            log::error!("{msg}");
            Err(msg)
        }
    }
}

impl hw_nfc_pn532 {
    pub(crate) fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        notify: hw_notify_cb,
        on_msg: hw_msg_cb,
        on_key: hw_nfc_pn532_cb,
    ) -> Result<Self, CString> {
        let (worker, rx, tx) = NfcWorker::new(on_msg);

        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let mut i2c = crate::i2c::i2c_impl::create_i2c_bus(i2c_bus, &tx, notify)?;
            debug!("i2c init: {i2c:?}");

            let i2c_addr: u16 = match i2c_addr {
                I2cAddress::Addr(addr) => addr.into(),
                I2cAddress::Default => 0x24,
                I2cAddress::Auto => 0x24,
                I2cAddress::Alt => return Err(format!("no alternative device address")),
                _ => return Err(format!("invalid i2c address: {i2c_addr:?}")),
            };

            try_device(&mut i2c, i2c_addr)?;

            let bus = i2c.bus();
            let interface = I2CInterface { i2c };
            let mut pn532 = Pn532::<_, _>::new(interface, Timer::new());

            send_debug(
                &tx,
                notify,
                format!("pn532 init with bus={bus} and addr=0x{i2c_addr:02x}").as_str(),
            );

            let firmware = pn532
                .process(&pn532::Request::GET_FIRMWARE_VERSION, 4, Duration::from_millis(50))
                .map_err(|err| format!("{err:?}"))?;
            log::info!("firmware: {firmware:?}");

            match pn532.process(
                &pn532::Request::sam_configuration(SAMMode::Normal, false),
                0,
                Duration::from_millis(50),
            ) {
                Ok(_) => println!("✅ PN532 готов"),
                Err(err) => {
                    println!("❌ Ошибка: {:?}", err);
                    return Err(format!("{err:?}"));
                }
            }

            while let Ok(_req) = rx.recv() {
                // match req {
                //     Request::ReadAll => match pn532.get_touched() {
                //         Ok(res) => {
                //             debug!("all keys: {res:b}");
                //             send_reply(Reply::AllKeys(res), &tx, notify);
                //         }
                //         Err(err) => {
                //             process_err(format!("{err:?}"), &tx, notify);
                //         }
                //     },
                // }
            }

            Ok(())
        });

        Ok(Self { worker, cb: on_key })
    }

    pub(crate) fn process_reply(nfc: *const Self) -> bool {
        if nfc.is_null() {
            error!("NULL NFC PN532 pointer");
            false
        } else {
            let nfc: &hw_nfc_pn532 = unsafe { &*nfc };

            nfc.worker.process_reply(&|rep| match rep {
                Reply::Message(level, msg) => {
                    nfc.worker.pd_message(level, &msg);
                }
                Reply::AllKeys(st) => {
                    nfc.cb.all_keys(st);
                }
            });

            true
        }
    }

    pub(crate) fn send_request(nfc: *const Self, req: Request) -> bool {
        if nfc.is_null() {
            error!("NULL NFC PN532 pointer");
            false
        } else {
            let nfc = unsafe { &*nfc };

            nfc.worker.send_request(req)
        }
    }
}
