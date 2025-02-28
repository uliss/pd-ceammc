// use em
use std::{ffi::CString, thread::sleep, time::Duration};

use log::{debug, error};
use rotary_encoder_embedded::RotaryEncoder;
use rppal::{gpio::Gpio, system::DeviceInfo};

use crate::{hw_msg_cb, hw_notify_cb};

use super::hw_gpio_rotenc;

impl hw_gpio_rotenc {
    pub fn new(
        dt: u8,
        clk: u8,
        btn: u8,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (req_tx, req_rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), String> {
            debug!("thread start");

            let gpio = Gpio::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            let dev = DeviceInfo::new().map_err(|err| {
                error!("{err}");
                err.to_string()
            })?;

            // Configure DT and CLK pins, typically pullup input
            let dt_pin = gpio
                .get(dt)
                .map_err(|err| {
                    error!("DT pin error: {err}");
                    err.to_string()
                })?
                .into_input_pullup();

            // dt_pin.

            let clk_pin = gpio
                .get(clk)
                .map_err(|err| {
                    error!("CLK pin error: {err}");
                    err.to_string()
                })?
                .into_input_pullup();

            // Initialize the rotary encoder
            let mut rotary_encoder = RotaryEncoder::new(dt_pin, clk_pin).into_standard_mode();

            // Now you can update the state of the rotary encoder and get a direction value. Call this from an update routine, timer task or interrupt
            // let _dir =

            // gpio.
            // dt_pin.set_async_interrupt(trigger, debounce, callback);

            // ...timer initialize at 900Hz to poll the rotary encoder
            loop {
                // gpio.poll_interrupts(&[&dt_pin, &clk_pin], false, Some(Duration::from_millis(10)));

                sleep(Duration::from_millis(1));
                let dir = rotary_encoder.update();

                debug!("{dir:?}");

                match dir {
                    rotary_encoder_embedded::Direction::None => {}
                    rotary_encoder_embedded::Direction::Clockwise => {}
                    rotary_encoder_embedded::Direction::Anticlockwise => {}
                }

                match req_rx.try_recv() {
                    Ok(_) => {}
                    Err(err) => match err {
                        std::sync::mpsc::TryRecvError::Empty => {}
                        std::sync::mpsc::TryRecvError::Disconnected => {
                            break;
                        }
                    },
                }
            }

            debug!("thread done");
            Ok(())
        });

        Ok(hw_gpio_rotenc {
            tx: req_tx,
            rx: rep_rx,
            on_err,
            notify,
        })
    }
}
