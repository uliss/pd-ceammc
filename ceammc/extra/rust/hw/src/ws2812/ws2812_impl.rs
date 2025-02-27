use std::ffi::CString;

use log::{debug, error};
use rgb::RGB8;
use smart_leds::Brightness;
use smart_leds_trait::SmartLedsWrite;
use ws2812_spi::Ws2812;

use crate::{
    hw_msg_cb, hw_notify_cb,
    max7219::{hw_spi_bus, hw_spi_cs},
};

use super::{hw_spi_ws2812, Request};

impl hw_spi_ws2812 {
    pub fn new(
        bus: hw_spi_bus,
        cs: hw_spi_cs,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
    ) -> Result<Self, CString> {
        let (tx, rx) = std::sync::mpsc::channel();
        let (rep_tx, rep_rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || -> Result<(), CString> {
            debug!("thread start");

            // const MOSI_PIN: u8 = 10; // [DATA] BCM GPIO 10 (physical pin 19)
            // const SCLK_PIN: u8 = 11; // [CLK]  BCM GPIO 11 (physical pin 23)
            // const CS_PIN: u8 = 8; //    [CS]   SS:   Ss0 BCM GPIO 8 (physical pin 24)

            let spi = rppal::spi::Spi::new(
                match bus {
                    hw_spi_bus::SPI0 => rppal::spi::Bus::Spi0,
                    hw_spi_bus::SPI1 => rppal::spi::Bus::Spi1,
                    hw_spi_bus::SPI2 => rppal::spi::Bus::Spi2,
                    hw_spi_bus::SPI3 => rppal::spi::Bus::Spi3,
                    hw_spi_bus::SPI4 => rppal::spi::Bus::Spi4,
                    hw_spi_bus::SPI5 => rppal::spi::Bus::Spi5,
                    hw_spi_bus::SPI6 => rppal::spi::Bus::Spi6,
                },
                match cs {
                    hw_spi_cs::CS0 => rppal::spi::SlaveSelect::Ss0,
                    hw_spi_cs::CS1 => rppal::spi::SlaveSelect::Ss1,
                    hw_spi_cs::CS2 => rppal::spi::SlaveSelect::Ss2,
                    hw_spi_cs::CS3 => rppal::spi::SlaveSelect::Ss3,
                },
                2_500_000,
                rppal::spi::Mode::Mode0,
            )
            .map_err(|err| {
                error!("{err}");
                CString::new(err.to_string()).unwrap_or_default()
            })?;

            debug!("SPI init: {spi:?}");

            let mut ws = Ws2812::new(spi);

            debug!("ws2182 init");

            let mut brightness = 127;

            while let Ok(req) = rx.recv() {
                debug!("{req:?}");

                match req {
                    crate::ws2812::Request::ColorRGB(r, g, b) => {
                        // setup some data to write
                        let mut data = [RGB8::default(); 3];
                        data[0] = [0xFF_u8, 0_u8, 0_u8].into(); // Full RED
                        data[1] = [0_u8, 0xFF_u8, 0_u8].into(); // Full GREEN
                        data[2] = [0_u8, 0_u8, 0xFF_u8].into(); // Full BLUE

                        if let Err(err) =
                            ws.write(smart_leds::brightness(data.iter().cloned(), brightness))
                        {
                            error!("write error: {err}");
                        }
                    }
                    Request::SetBrightness(b) => {
                        brightness = b;
                    },
                }
            }

            Ok(())
        });

        Ok(hw_spi_ws2812 {
            tx,
            rx: rep_rx,
            on_err,
            notify,
        })
    }

    pub fn send_ptr(ws: *const Self, req: Request) -> bool {
        if ws.is_null() {
            error!("NULL ws pointer");
            return false;
        }

        let ws = unsafe { &*ws };

        if let Err(err) = ws.tx.send(req) {
            error!("send error: {err}");
            return false;
        }

        ws.notify.notify();
        true
    }
}
