use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::I2cAddress,
    nfc::{hw_nfc_pn532, hw_nfc_pn532_cb, pn532_timer::Timer, NfcWorker, Reply, Request},
    send_debug,
};
use log::{debug, error};
use pn532::{i2c::I2CInterface, Pn532};
use pn532::{requests::SAMMode, Interface};
use rppal::i2c::I2c;
use std::{ffi::CString, time::Duration};

enum DetectMethod {
    QuickWrite,
    ReceiveByte,
    ReadByte,
    WriteRead,
}

fn try_device(i2c: &mut I2c, i2c_addr: u16, method: DetectMethod) -> Result<(), String> {
    i2c.set_slave_address(i2c_addr).map_err(|err| err.to_string())?;
    // i2c.set_timeout(10).map_err(|err| err.to_string())?;

    match match method {
        DetectMethod::QuickWrite => i2c.write(&[]).map(|_| ()),
        DetectMethod::ReceiveByte => i2c.smbus_receive_byte().map(|_| ()),
        DetectMethod::ReadByte => {
            let mut buf = [0u8; 1];
            i2c.read(&mut buf).map(|_| ())
        }
        DetectMethod::WriteRead => {
            let mut buf = [0u8; 1];
            i2c.write_read(&[0x00], &mut buf).map(|_| ())
        }
    } {
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

            try_device(&mut i2c, i2c_addr, DetectMethod::QuickWrite)?;
            i2c.set_timeout(10).map_err(|err| err.to_string())?;

            let bus = i2c.bus();
            let interface = I2CInterface { i2c };

            let mut pn532 = Pn532::<_, _>::new(interface, Timer::new());

            send_debug(
                &tx,
                notify,
                format!("pn532 init with bus={bus} and addr=0x{i2c_addr:02x}").as_str(),
            );

            while let Ok(req) = rx.recv() {
                match req {
                    Request::ReadAll => {
                        // let firmware = pn532
                        //     .process(&pn532::Request::GET_FIRMWARE_VERSION, 4, Duration::from_millis(50))
                        //     .map_err(|err| format!("{err:?}"))?;
                        // log::info!("firmware: {firmware:?}");

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
                    }
                }
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
