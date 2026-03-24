use std::{ffi::CString, path::Path};

use log::{debug, error};
use rppal::i2c::I2c;

use crate::{hw_msg_cb, hw_notify_cb, process_err, MakePdMessage};

use super::hw_i2c;

pub enum DetectMethod {
    QuickWrite,
    ReceiveByte,
    ReadByte,
    WriteRead,
}

pub fn try_i2c_device(i2c: &mut I2c, i2c_addr: u16, method: DetectMethod) -> Result<(), String> {
    i2c.set_slave_address(i2c_addr).map_err(|err| err.to_string())?;

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
            let msg = format!("device 0x{:02x} is not responding: check connection!", i2c_addr);
            log::error!("{msg}");
            Err(msg)
        }
    }
}

fn check_kernel_i2c_modules() -> Result<(), String> {
    const PROC_MODULES: &str = "/proc/modules";
    const ETC_MODULES: &str = "/etc/modules";
    const I2C_MODULE: &str = "i2c_dev";
    const I2C_MODULE_CONF: &str = "i2c-dev";
    let modules_content =
        std::fs::read_to_string(PROC_MODULES).map_err(|err| format!("can't read {PROC_MODULES}: {err}"))?;

    for line in modules_content.lines() {
        if line.trim().starts_with(I2C_MODULE) {
            return Ok(());
        }
    }

    if Path::new(ETC_MODULES).exists() {
        let modules_conf =
            std::fs::read_to_string("/etc/modules").map_err(|err| format!("can't read {ETC_MODULES}: {err}"))?;

        for line in modules_conf.lines() {
            if line.trim().starts_with(I2C_MODULE_CONF) {
                return Err(format!("kernel module {I2C_MODULE} is set for autoload, but is not loaded. You can load it with command: sudo modprobe {I2C_MODULE}"));
            }
        }

        return Err(format!(
            "kernel module {I2C_MODULE} is NOT set for autoload, add {I2C_MODULE_CONF} to {ETC_MODULES}"
        ));
    }

    Err(format!(
        "kernel module {I2C_MODULE} is not loaded and module autoload settings not found"
    ))
}

fn check_kernel_i2c_config() -> Result<(), String> {
    const BOOT_CONFIG: &str = "/boot/firmware/config.txt";
    const DT_PARAM0: &str = "dtparam=i2c_arm=on";
    const DT_PARAM1: &str = "dtparam=i2c_arm=true";

    if Path::new(BOOT_CONFIG).exists() {
        let config = std::fs::read_to_string(BOOT_CONFIG).map_err(|err| format!("can't read {BOOT_CONFIG}: {err}"))?;

        let i2c_enabled = config
            .lines()
            .any(|line| line.trim().starts_with(DT_PARAM0) || line.trim().starts_with(DT_PARAM1));

        if !i2c_enabled {
            return Err(format!(
                "I2C is not enabled in {BOOT_CONFIG}. Add {DT_PARAM0} into {BOOT_CONFIG}"
            ));
        }
        return Ok(());
    }

    Err(format!("{BOOT_CONFIG} is not found"))
}

fn check_i2c_device(bus: u8) -> Result<(), String> {
    let i2c_dev = format!("/dev/i2c-{bus}");
    if !Path::new(&i2c_dev).exists() {
        return Err(format!("i2c device is not found: {i2c_dev}"));
    }
    Ok(())
}

fn check_i2c_permissions() -> Result<(), String> {
    const I2C_GROUP: &str = "i2c";
    let user = users::get_user_by_uid(users::get_current_uid()).ok_or(format!("can't get current user"))?;

    if let Some(groups) = users::get_user_groups(user.name(), user.primary_group_id()) {
        if !groups.iter().any(|g| g.name() == I2C_GROUP) {
            Err(format!(
                "user {0} is NOT in the group {I2C_GROUP}. Add: sudo usermod -a -G {I2C_GROUP} {0}",
                user.name().display()
            ))
        } else {
            Ok(())
        }
    } else {
        Err(format!("can't get user groups"))
    }
}

#[allow(non_snake_case)]
pub fn create_i2c_bus<Reply>(bus: i8, tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb) -> Result<I2c, String>
where
    Reply: MakePdMessage<Reply>,
{
    match bus {
        crate::i2c::HW_I2C_DEFAULT_BUS => Ok(I2c::new().map_err(|err| process_err(err, tx, notify))?),
        bus if bus >= 0 && bus < crate::i2c::HW_I2C_MAX_BUS => match I2c::with_bus(bus as u8) {
            Ok(i2c) => Ok(i2c),
            Err(err) => {
                check_kernel_i2c_modules()?;
                check_kernel_i2c_config()?;
                check_i2c_device(bus as u8)?;
                check_i2c_permissions()?;
                Err(process_err(err, tx, notify))
            }
        },
        _ => {
            return Err(process_err(format!("invalid I2C bus: {bus}"), tx, notify));
        }
    }
}

impl hw_i2c {
    pub fn new(addr: u8, _notify: hw_notify_cb, on_msg: hw_msg_cb) -> Result<Self, CString> {
        let (tx, rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("worker thread start");

            let mut i2c = I2c::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            i2c.set_slave_address(addr as u16).map_err(|e| e.to_string())?;

            debug!("i2c init done");

            while let Ok(req) = rx.recv() {
                match req {
                    crate::i2c::Request::ScanDevices => {
                        Self::scan_devices(&mut i2c)?;
                    }
                }
            }

            debug!("worker thread done");

            Ok(())
        });

        Ok(hw_i2c {
            _tx: tx,
            _on_err: on_msg,
        })
    }

    fn scan_devices(i2c: &mut I2c) -> Result<(), String> {
        for i in 0..127 {
            i2c.set_slave_address(i).map_err(|e| e.to_string())?;
            // i2c.read(buffer)
        }

        todo!()
    }
}
