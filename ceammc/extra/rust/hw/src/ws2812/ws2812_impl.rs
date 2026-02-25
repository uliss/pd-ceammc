use std::ffi::CString;

use log::{debug, error};
use rgb::RGB8;
use smart_leds_trait::SmartLedsWrite;
use ws2812_spi::prerendered::Ws2812;

use crate::{
    hw_msg_cb, hw_notify_cb,
    max7219::{hw_spi_bus, hw_spi_cs},
    ws2812::{led_fx, Reply},
    MakePdMessage,
};

use super::{hw_spi_ws2812, Request};

fn pos2index(idx: i32, len: usize) -> usize {
    if idx >= 0 {
        (idx as usize).min(len)
    } else {
        let idx = idx.abs() as usize;
        if idx < len {
            len - idx
        } else {
            0
        }
    }
}

impl hw_spi_ws2812 {
    pub fn new(
        bus: hw_spi_bus,
        cs: hw_spi_cs,
        size: usize,
        notify: hw_notify_cb,
        on_msg: hw_msg_cb,
        clear_on_exit: bool,
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
                    hw_spi_bus::NONE => return Err(CString::from_vec_with_nul(b"\0".to_vec()).unwrap()),
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
                3_840_000,
                rppal::spi::Mode::Mode0,
            )
            .map_err(|err| {
                error!("{err}");
                CString::new(err.to_string()).unwrap_or_default()
            })?;

            debug!("SPI init: {spi:?}");

            let mut prerendered_buf = [0; 1024];
            let mut ws = Ws2812::new(spi, &mut prerendered_buf);

            debug!("ws2182 init with size: {size}");

            let mut leds = Vec::with_capacity(size);
            leds.resize(size, RGB8::default());

            let mut brightness = 127;

            while let Ok(req) = rx.recv() {
                debug!("{req:?}");

                match req {
                    crate::ws2812::Request::SetPixelColor(idx, rgb) => match leds.get_mut(idx) {
                        Some(c) => *c = rgb,
                        None => Self::send_error(&rep_tx, notify, format!("invalid pixel index: {idx}").as_str()),
                    },
                    Request::SetBrightness(b) => {
                        brightness = b;
                    }
                    Request::Flush => {
                        if let Err(err) = ws.write(smart_leds::brightness(leds.iter().cloned(), brightness)) {
                            Self::send_error(&rep_tx, notify, format!("{err:?}").as_str());
                        }
                    }
                    Request::Rotate(delta) => {
                        let len = leds.len();
                        if delta > 0 {
                            leds.rotate_right((delta as usize).max(len));
                        } else {
                            leds.rotate_left((delta.abs() as usize).max(len));
                        }
                    }
                    Request::Clear => {
                        leds.fill(RGB8::default());
                    }
                    Request::Fill(rgb) => {
                        leds.fill(rgb);
                    }
                    Request::SetSliceColor(slice, rgb) => {
                        let a = pos2index(slice.first, leds.len());
                        let b = pos2index(slice.last, leds.len());

                        for idx in (a..=b).step_by(slice.step) {
                            leds.get_mut(idx).map(|c| {
                                *c = rgb;
                            });
                        }
                    }
                    Request::SetRangeColor(range, rgb) => {
                        let a = pos2index(range.first, leds.len());
                        let b = (a + range.length).min(leds.len());

                        for idx in a..b {
                            leds.get_mut(idx).map(|c| {
                                *c = rgb;
                            });
                        }
                    }
                    Request::ApplyEffect(range, fx, arg, flush) => match fx {
                        crate::ws2812::hw_led_fx::Rainbow => {
                            let a = pos2index(range.first, leds.len());
                            let b = (a + range.length).min(leds.len());

                            for (idx, c) in &mut leds[a..b].iter_mut().enumerate() {
                                *c = led_fx::rainbow(idx, b, arg);
                            }

                            if flush {
                                if let Err(err) =
                                    ws.write(smart_leds::brightness(leds[a..b].iter().cloned(), brightness))
                                {
                                    Self::send_error(&rep_tx, notify, format!("{err:?}").as_str());
                                }
                            }
                        }
                    },
                }
            }

            Ok(())
        });

        Ok(hw_spi_ws2812 {
            tx,
            rx: rep_rx,
            on_msg,
            notify,
            clear_on_exit,
        })
    }

    fn send(&self, req: Request) -> bool {
        if let Err(err) = self.tx.send(req) {
            error!("send error: {err}");
            return false;
        }

        self.notify.notify();
        true
    }

    pub fn send_ptr(ws: *const Self, req: Request) -> bool {
        if ws.is_null() {
            error!("NULL ws pointer");
            return false;
        }

        let ws = unsafe { &*ws };
        ws.send(req)
    }

    fn send_error(tx: &std::sync::mpsc::Sender<Reply>, notify: hw_notify_cb, err: &str) {
        error!("ws2812 write error: {err}");

        tx.send(Reply::pd_error(CString::new(err).unwrap_or_default()))
            .map(|_| {
                notify.notify();
            })
            .unwrap_or_else(|err| {
                error!("send error: {err}");
            });
    }

    pub fn process_ptr(ws: *mut Self) {
        if ws.is_null() {
            error!("NULL ws pointer");
            return;
        }

        let ws = unsafe { &*ws };
        while let Ok(rep) = ws.rx.try_recv() {
            match rep {
                Reply::Message(level, str) => {
                    ws.on_msg.exec(level, str.to_str().unwrap());
                }
            }
        }
    }
}

impl Drop for hw_spi_ws2812 {
    fn drop(&mut self) {
        if self.clear_on_exit {
            if self.send(Request::Clear) {
                self.send(Request::Flush);
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use crate::ws2812::ws2812_impl::pos2index;

    #[test]
    fn p2i() {
        assert_eq!(pos2index(0, 3), 0);
        assert_eq!(pos2index(1, 3), 1);
        assert_eq!(pos2index(2, 3), 2);
        assert_eq!(pos2index(3, 3), 3);
        assert_eq!(pos2index(3, 3), 3);
        assert_eq!(pos2index(-1, 3), 2);
        assert_eq!(pos2index(-2, 3), 1);
        assert_eq!(pos2index(-3, 3), 0);
        assert_eq!(pos2index(0, 1), 0);
        assert_eq!(pos2index(-4, 3), 0);

        assert_eq!(pos2index(0, 1), 0);
        assert_eq!(pos2index(1, 1), 1);
        assert_eq!(pos2index(-1, 1), 0);
        assert_eq!(pos2index(-2, 1), 0);

        assert_eq!(pos2index(0, 0), 0);
        assert_eq!(pos2index(1, 0), 0);
        assert_eq!(pos2index(-1, 0), 0);
    }
}
