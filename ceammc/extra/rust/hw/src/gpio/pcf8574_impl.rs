use std::ffi::CString;

use ceammc_rs_msg::{msg_cb, msg_notify};
use log::{debug, error};
use pcf857x::PinFlag;
use pcf857x::{Pcf8574, SlaveAddr};
use rppal::gpio::Gpio;

use crate::gpio::hw_gpio_mode;
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

fn from_i2c_addr(addr: u8) -> Result<SlaveAddr, String> {
    let x = match addr {
        0x20 => SlaveAddr::Default,
        0x21 => SlaveAddr::Alternative(false, false, true),
        0x22 => SlaveAddr::Alternative(false, true, false),
        0x23 => SlaveAddr::Alternative(false, true, true),
        0x24 => SlaveAddr::Alternative(true, false, false),
        0x25 => SlaveAddr::Alternative(true, false, true),
        0x26 => SlaveAddr::Alternative(true, true, false),
        0x27 => SlaveAddr::Alternative(true, true, true),
        _ => return Err(format!("invalid i2c address: 0x{addr:02x}")),
    };
    Ok(x)
}

struct PinConfig {
    modes: [hw_gpio_mode; 8],
    raw_output: u8,
}

fn to_pin_flag(pin: u8) -> Option<PinFlag> {
    let flag = match pin {
        0 => PinFlag::P0,
        1 => PinFlag::P1,
        2 => PinFlag::P2,
        3 => PinFlag::P3,
        4 => PinFlag::P4,
        5 => PinFlag::P5,
        6 => PinFlag::P6,
        7 => PinFlag::P7,
        _ => return None,
    };
    Some(flag)
}

impl PinConfig {
    fn new() -> Self {
        Self {
            modes: [hw_gpio_mode::Output; 8],
            raw_output: 0,
        }
    }

    fn pin_input_flag(pin: u8, mode: &hw_gpio_mode) -> Option<PinFlag> {
        match mode {
            hw_gpio_mode::Input => to_pin_flag(pin),
            hw_gpio_mode::Output => None,
        }
    }

    fn pin_flags(&self) -> Option<PinFlag> {
        self.modes
            .iter()
            .enumerate()
            .map(|(i, pin)| Self::pin_input_flag(i as u8, pin))
            .reduce(|a, b| match (a, b) {
                (None, None) => None,
                (None, Some(b)) => Some(b),
                (Some(a), None) => Some(a),
                (Some(a), Some(b)) => Some(a | b),
            })
            .flatten()
    }

    fn input_mask(&self) -> u8 {
        let mut res = 0;
        for i in 0..self.modes.len() {
            if self.modes[i] == hw_gpio_mode::Input {
                res |= 0x1 << i;
            }
        }

        res
    }

    fn set_mode(&mut self, pin: u8, mode: hw_gpio_mode) -> bool {
        let pin: usize = pin.into();
        if pin < self.modes.len() {
            self.modes[pin] = mode;
            true
        } else {
            false
        }
    }

    fn write(&mut self, pin: u8, value: bool) -> Result<u8, String> {
        let pin: usize = pin.into();
        if pin >= self.modes.len() {
            return Err(format!("invalid pin: {pin}"));
        }

        if self.modes[pin] != hw_gpio_mode::Output {
            return Err(format!("pin [{pin}] is not configured for output"));
        }

        let pin_value = (0x1 << pin) | self.input_mask();
        if value {
            self.raw_output |= pin_value;
        } else {
            self.raw_output &= !(0x1 << pin) | self.input_mask();
        }

        Ok(self.raw_output)
    }
}

fn read_pin(pin: u8, all_pins_state: u8) -> bool {
    ((0x1 << pin) & all_pins_state) > 0
}

impl hw_pcf8574 {
    pub fn new(
        i2c_bus: i8,
        i2c_addr: I2cAddress,
        pin_interrupt: Option<u8>,
        notify: msg_notify,
        on_msg: msg_cb,
        on_data: hw_pcf8574_cb,
    ) -> Result<Self, CString> {
        let worker = ceammc_rs_msg::Client::<Request, Reply>::start_worker(
            move |channel| {
                let mut pin_config = PinConfig::new();

                let mut i2c = create_i2c_bus(i2c_bus)?;
                let addr = match i2c_addr {
                    I2cAddress::Default => SlaveAddr::Default,
                    I2cAddress::Alt => SlaveAddr::Alternative(true, false, false),
                    I2cAddress::Auto => SlaveAddr::Default,
                    I2cAddress::Invalid(_) => Err(format!("invalid i2c address"))?,
                    I2cAddress::Addr(val) => from_i2c_addr(val)?,
                };

                let i2c_addr = to_i2c_addr(&addr);
                try_i2c_device(&mut i2c, i2c_addr, crate::i2c::i2c_impl::DetectMethod::ReceiveByte)?;

                let mut device = Pcf8574::new(i2c, addr);
                channel.send_debug(format!(
                    "connected to i2c pcf8574 device with addr: 0x{i2c_addr:02x} ({addr:?})"
                ))?;

                let mut gpio_pin = None;
                if let Some(pin) = pin_interrupt {
                    channel.send_debug(format!("using GPIO interrupt pin: {pin}"))?;
                    let gpio = Gpio::new().map_err(|err| err.to_string())?;
                    let pin = gpio.get(pin).map_err(|err| err.to_string())?.into_input_pullup();
                    gpio_pin = Some(pin);
                }

                if let Some(mut pin) = gpio_pin {
                    pin.set_async_interrupt(rppal::gpio::Trigger::FallingEdge, None, |_| {})
                        .map_err(|err| err.to_string())?;
                }

                if let Err(err) = channel.recv_loop(&mut |req| {
                    match req {
                        Request::WriteAllPins(pins) => {
                            device.set(pins).map_err(|err| format!("{err:?}"))?;
                        }
                        Request::ConfigPin(pin, mode) => {
                            if !pin_config.set_mode(pin, mode) {
                                channel.send_error(format!("invalid pin: {pin}"))?
                            }
                        }
                        Request::ReadPin(pin) => {
                            if let Some(mask) = to_pin_flag(pin) {
                                let state = read_pin(pin, device.get(mask).map_err(|err| format!("{err:?}"))?);
                                channel.send_data(Reply::ReadPin { pin, state }).to_worker_result()?;
                            } else {
                                channel.send_error(format!("pin [{pin}] is not configured for read"))?;
                            }
                        }
                        Request::ReadAllPins => {
                            if let Some(mask) = pin_config.pin_flags() {
                                let state = device.get(mask).map_err(|err| format!("{err:?}"))?;
                                channel
                                    .send_data(Reply::ReadAll {
                                        mask: pin_config.input_mask(),
                                        state,
                                    })
                                    .to_worker_result()?;
                            } else {
                                channel.send_error(format!("no pins are configured for read"))?;
                            }
                        }
                        Request::WritePin { pin, value } => {
                            let bits = pin_config.write(pin, value)?;
                            device.set(bits).map_err(|err| format!("{err:?}"))?;
                            debug!("write: {bits:08b}");
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
            Reply::ReadAll { mask, state } => dev.cb.read_all(mask, state),
            Reply::ReadPin { pin, state } => dev.cb.read_pin(pin, state),
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

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_pin_config() {
        let mut cfg = PinConfig::new();
        assert_eq!(cfg.raw_output, 0b0000_0000);
        assert_eq!(cfg.input_mask(), 0b0000_0000);
        cfg.set_mode(0, hw_gpio_mode::Output);
        assert_eq!(cfg.raw_output, 0b0000_0000);
        assert_eq!(cfg.input_mask(), 0b0000_0000);
        cfg.set_mode(0, hw_gpio_mode::Input);
        assert_eq!(cfg.raw_output, 0b0000_0000);
        assert_eq!(cfg.input_mask(), 0b0000_0001);
        assert!(cfg.write(0, true).is_err());
        assert_eq!(cfg.write(1, true), Ok(0b0000_0011));
        assert_eq!(cfg.write(2, true), Ok(0b0000_0111));
        assert_eq!(cfg.write(2, false), Ok(0b0000_0011));
        assert_eq!(cfg.write(1, false), Ok(0b0000_0001));
        assert_eq!(cfg.pin_flags(), Some(PinFlag::P0));
        cfg.set_mode(7, hw_gpio_mode::Input);
        assert_eq!(cfg.write(1, true), Ok(0b1000_0011));
        assert_eq!(cfg.pin_flags(), Some(PinFlag::P0 | PinFlag::P7));
    }
}
