use std::{ffi::CString, time::Duration};

use log::{debug, error};
use palette::Srgb;
use rgb::RGB8;
use smart_led_effects::strip::{
    Bounce, Breathe, Collision, Cycle, Cylon, EffectIterator, Fire, Meteor, Rainbow, SnowSparkle, Strobe, Twinkle,
};
use smart_leds_trait::SmartLedsWrite;
use ws2812_spi::prerendered::Ws2812;

use crate::{
    hw_msg_cb, hw_notify_cb,
    max7219::{hw_spi_bus, hw_spi_cs},
    ws2812::Reply,
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
        let (tx, mut rx) = tokio::sync::mpsc::channel(16);
        let (rep_tx, rep_rx) = tokio::sync::mpsc::channel(16);

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

            let mut prerendered_buf = [0; 2024];
            let mut ws = Ws2812::new(spi, &mut prerendered_buf);

            debug!("ws2182 init with size: {size}");

            let mut leds = Vec::with_capacity(size);
            leds.resize(size, RGB8::default());

            let mut brightness = 127;

            let mut fx_rainbow = Rainbow::new(size, None);
            let mut fx_strobe = Strobe::new(size, None, Duration::from_millis(30), None);
            let mut fx_twinkle = Twinkle::new(size, None, None, None, None);
            let mut fx_bounce = Bounce::new(size, None, None, None, None, None);
            let mut fx_collision = Collision::new(size, None);
            let mut fx_breathe = Breathe::new(size, None, None);
            let mut fx_meteor = Meteor::new(size, None, None, None);
            let mut fx_fire = Fire::new(size, None, None);
            let mut fx_cycle = Cycle::new(size, None);
            let mut fx_cylon = Cylon::new(size, Srgb::new(200u8, 100u8, 50u8), None, None);
            let mut fx_snow_sparkle = SnowSparkle::new(size, None, None, None, None);

            let rt = tokio::runtime::Builder::new_current_thread()
                .build()
                .map_err(|err| CString::new(err.to_string()).unwrap_or_default())?;

            rt.block_on(async {
                debug!("tokio start");

                while let Some(req) = rx.recv().await {
                    debug!("{req:?}");

                    match req {
                        Request::SetPixelColor(rgb, idx) => match leds.get_mut(idx) {
                            Some(c) => {
                                *c = rgb.into8();
                            }
                            None => {
                                Self::send_error(&rep_tx, notify, format!("invalid pixel index: {idx}").as_str()).await
                            }
                        },
                        Request::SetBrightness(b) => {
                            brightness = b;
                        }
                        Request::Flush => {
                            if let Err(err) = ws.write(smart_leds::brightness(leds.iter().cloned(), brightness)) {
                                Self::send_error(&rep_tx, notify, format!("{err:?}").as_str()).await;
                            }
                        }
                        Request::Rotate(delta, _slice) => {
                            if delta > 0 {
                                leds.rotate_right((delta as usize).min(size));
                            } else {
                                leds.rotate_left((delta.abs() as usize).min(size));
                            }
                        }
                        Request::Clear => {
                            leds.fill(RGB8::default());
                        }
                        Request::FillSlice(color, slice) => {
                            let a = slice.map(|x| pos2index(x.first, size)).unwrap_or(0);
                            let b = slice.map(|x| pos2index(x.last, size)).unwrap_or(size);
                            let step = slice.map(|x| x.step).unwrap_or(1);

                            for idx in (a..=b).step_by(step as usize) {
                                leds.get_mut(idx).map(|c| {
                                    *c = color.into8();
                                });
                            }
                        }
                        Request::Quit => {
                            break;
                        }
                        Request::ApplyEffect(fx, slice) => {
                            let mut apply_fn = |fx: &mut dyn EffectIterator| -> Option<()> {
                                let new_data = fx.next()?;

                                match slice {
                                    Some(slice) => {
                                        let a = pos2index(slice.first, size);
                                        let b = pos2index(slice.last, size);
                                        let step = slice.step.max(1).try_into().ok()?;

                                        for idx in (a..=b).step_by(step) {
                                            let a = leds.get_mut(idx)?;
                                            let b = new_data.get(idx)?;
                                            a.r = b.red;
                                            a.g = b.green;
                                            a.b = b.blue;
                                        }
                                    }
                                    None => {
                                        for (a, b) in leds.iter_mut().zip(&new_data) {
                                            a.r = b.red;
                                            a.g = b.green;
                                            a.b = b.blue;
                                        }
                                    }
                                }

                                Some(())
                            };

                            match fx {
                                crate::ws2812::hw_led_fx::Bounce => apply_fn(&mut fx_bounce),
                                crate::ws2812::hw_led_fx::Breathe => apply_fn(&mut fx_breathe),
                                crate::ws2812::hw_led_fx::Collision => apply_fn(&mut fx_collision),
                                crate::ws2812::hw_led_fx::Cycle => apply_fn(&mut fx_cycle),
                                crate::ws2812::hw_led_fx::Cylon => apply_fn(&mut fx_cylon),
                                crate::ws2812::hw_led_fx::Fire => apply_fn(&mut fx_fire),
                                crate::ws2812::hw_led_fx::Meteor => apply_fn(&mut fx_meteor),
                                crate::ws2812::hw_led_fx::Rainbow => apply_fn(&mut fx_rainbow),
                                crate::ws2812::hw_led_fx::Strobe => apply_fn(&mut fx_strobe),
                                crate::ws2812::hw_led_fx::Twinkle => apply_fn(&mut fx_twinkle),
                                crate::ws2812::hw_led_fx::SnowSparkle => apply_fn(&mut fx_snow_sparkle),
                            };
                        }
                        Request::FillBits(color, offset, bitset) => {
                            let a = pos2index(offset, leds.len());
                            for i in 0..bitset.len() {
                                let led_idx = i + a;
                                if led_idx >= leds.len() {
                                    break;
                                }
                                if bitset[i] {
                                    leds[led_idx] = color.into8();
                                } else {
                                    leds[led_idx].b = 0;
                                    leds[led_idx].g = 0;
                                    leds[led_idx].r = 0;
                                }
                            }
                        }
                        Request::FillPixels(color, pixels) => {
                            for p in &pixels {
                                let idx = pos2index(*p, leds.len());
                                if idx >= leds.len() {
                                    break
                                }

                                leds[idx].r = color.red;
                                leds[idx].g = color.green;
                                leds[idx].b = color.blue;
                            }
                        },
                    }
                }

                let _ = rep_tx.try_send(Reply::Done);
                // no notify
                debug!("tokio done");
            });

            if clear_on_exit {
                leds.fill(RGB8::default());
                if let Err(err) = ws.write(leds.iter().cloned()) {
                    error!("clear_on_exit error: {err:?}");
                } else {
                    debug!("clear on exit ...");
                }
            }

            debug!("thread done");
            Ok(())
        });

        Ok(hw_spi_ws2812 {
            tx,
            rx: rep_rx,
            on_msg,
            notify,
        })
    }

    fn send(&self, req: Request) -> bool {
        if let Err(err) = self.tx.try_send(req) {
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

    async fn send_error(tx: &tokio::sync::mpsc::Sender<Reply>, notify: hw_notify_cb, err: &str) {
        error!("ws2812 write error: {err}");

        tx.try_send(Reply::pd_error(CString::new(err).unwrap_or_default()))
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

        let ws = unsafe { &mut *ws };
        while let Ok(rep) = ws.rx.try_recv() {
            match rep {
                Reply::Message(level, str) => {
                    ws.on_msg.exec(level, str.to_str().unwrap());
                }
                Reply::Done => {}
            }
        }
    }
}

impl Drop for hw_spi_ws2812 {
    fn drop(&mut self) {
        self.send(Request::Quit);
        loop {
            match self.rx.try_recv() {
                Ok(_) => {}
                Err(err) => match err {
                    tokio::sync::mpsc::error::TryRecvError::Empty => continue,
                    tokio::sync::mpsc::error::TryRecvError::Disconnected => break,
                },
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
