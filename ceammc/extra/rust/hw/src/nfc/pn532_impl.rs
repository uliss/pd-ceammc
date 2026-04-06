use crate::{
    hw_msg_cb,
    i2c::i2c_impl::{try_i2c_device, DetectMethod},
    nfc::{hw_nfc_pn532, hw_nfc_pn532_cb, pn532_timer::Timer, NfcWorker, Reply, Request},
    send_debug,
};
use ceammc_rs_msg::msg_notify;
use log::{debug, error};
use pn532::requests::SAMMode;
use pn532::{i2c::I2CInterface, Pn532};
use std::{ffi::CString, time::Duration};

const PN532_I2C_ADDR: u16 = 0x24;

impl hw_nfc_pn532 {
    pub(crate) fn new(
        i2c_bus: i8,
        notify: msg_notify,
        on_msg: hw_msg_cb,
        on_key: hw_nfc_pn532_cb,
    ) -> Result<Self, CString> {
        let (mut worker, rx, tx) = NfcWorker::new(on_msg, None);

        worker.spawn(tx.clone(), notify, move || -> Result<(), String> {
            let mut i2c = crate::i2c::i2c_impl::create_i2c_bus(i2c_bus)?;
            debug!("i2c init: {i2c:?}");

            try_i2c_device(&mut i2c, PN532_I2C_ADDR, DetectMethod::QuickWrite)?;
            i2c.set_timeout(10).map_err(|err| err.to_string())?;

            let bus = i2c.bus();
            let interface = I2CInterface { i2c };

            let mut pn532 = Pn532::<_, _>::new(interface, Timer::new());

            send_debug(
                &tx,
                notify,
                format!("pn532 init with i2c_bus={bus} and i2c_addr=0x{PN532_I2C_ADDR:02x}").as_str(),
            )
            .to_err()?;

            while let Ok(crate::WorkerCommand::Command(req)) = rx.recv() {
                match req {
                    Request::ReadAll => {
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
