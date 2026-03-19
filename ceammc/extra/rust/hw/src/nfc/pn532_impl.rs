use crate::{
    hw_msg_cb, hw_notify_cb,
    i2c::I2cAddress,
    nfc::{hw_nfc_pn532, hw_nfc_pn532_cb, pn532_timer::Timer, NfcWorker, Reply, Request},
    send_debug,
};
use log::{debug, error};
use pn532::requests::SAMMode;
use pn532::{i2c::I2CInterface, CountDown, Pn532};
use std::{ffi::CString, time::Duration};

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
            i2c.set_slave_address(0x24).map_err(|err| err.to_string())?;

            let bus = i2c.bus();
            let interface = I2CInterface { i2c };

            let mut timer = Timer::new();
            timer.start(Duration::from_millis(10));

            let mut pn532 = Pn532::<_, _>::new(interface, timer);
            // = match i2c_addr {
            // I2cAddress::Addr(_addr) => Pn532::new(interface, timer)),
            // I2cAddress::Default => Mpr121::new_default(i2c, &mut delay),
            // I2cAddress::Auto => Mpr121::new_default(i2c, &mut delay),
            // I2cAddress::Alt => return Err(format!("no alternative device address")),
            // _ => return Err(format!("invalid i2c address:")),
            // }?;
            // .map_err(|err| process_err(format!("{err:?}"), &tx, notify))?;

            // let t = CountDown::start(&mut self, count);

            send_debug(
                &tx,
                notify,
                format!("pn532 init with bus={bus} and addr={i2c_addr:?}").as_str(),
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
