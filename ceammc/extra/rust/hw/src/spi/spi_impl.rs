use std::ffi::CString;

use ceammc_rs_msg::{msg_cb, msg_notify};
use log::error;
use rppal::spi::Spi;

use crate::spi::{hw_spi, hw_spi_bus, hw_spi_cb, hw_spi_cs, Reply, Request};

pub fn i8_to_spi_bus(bus: i8) -> rppal::spi::Bus {
    match bus {
        0 => rppal::spi::Bus::Spi0,
        1 => rppal::spi::Bus::Spi1,
        2 => rppal::spi::Bus::Spi2,
        3 => rppal::spi::Bus::Spi3,
        4 => rppal::spi::Bus::Spi4,
        5 => rppal::spi::Bus::Spi5,
        6 => rppal::spi::Bus::Spi6,
        _ => rppal::spi::Bus::Spi0,
    }
}

pub fn i8_to_slave_select(cs: u8) -> Result<rppal::spi::SlaveSelect, String> {
    if cs < 16 {
        Ok(match cs {
            0 => rppal::spi::SlaveSelect::Ss0,
            1 => rppal::spi::SlaveSelect::Ss1,
            2 => rppal::spi::SlaveSelect::Ss2,
            3 => rppal::spi::SlaveSelect::Ss3,
            4 => rppal::spi::SlaveSelect::Ss4,
            5 => rppal::spi::SlaveSelect::Ss5,
            6 => rppal::spi::SlaveSelect::Ss6,
            7 => rppal::spi::SlaveSelect::Ss7,
            8 => rppal::spi::SlaveSelect::Ss8,
            9 => rppal::spi::SlaveSelect::Ss9,
            10 => rppal::spi::SlaveSelect::Ss10,
            11 => rppal::spi::SlaveSelect::Ss11,
            12 => rppal::spi::SlaveSelect::Ss12,
            13 => rppal::spi::SlaveSelect::Ss13,
            14 => rppal::spi::SlaveSelect::Ss14,
            15 => rppal::spi::SlaveSelect::Ss15,
            _ => rppal::spi::SlaveSelect::Ss0,
        })
    } else {
        Err(format!("invalid CS value: {cs}"))
    }
}

pub fn spi_new(bus: hw_spi_bus, cs: hw_spi_cs) -> Result<Spi, String> {
    rppal::spi::Spi::new(
        match bus {
            hw_spi_bus::NONE => return Err(String::new()),
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
        1_000_000,
        rppal::spi::Mode::Mode0,
    )
    .map_err(|err| {
        error!("SPI init error: {err}");
        err.to_string()
    })
}

impl hw_spi {
    pub fn new(
        bus: hw_spi_bus,
        cs: hw_spi_cs,
        notify: msg_notify,
        on_msg: msg_cb,
        on_data: hw_spi_cb,
    ) -> Result<Self, CString> {
        Ok(Self {
            obj: ceammc_rs_msg::Client::<Request, Reply>::start_worker(
                move |channel: &ceammc_rs_msg::ClientChannelBounded<Request, Reply>| {
                    let spi = spi_new(bus, cs)?;

                    loop {
                        if let Err(err) = channel.recv(&mut |req| -> Result<(), String> {
                            //
                            match req {
                                Request::LoopbackTest => {
                                    let tx_data = [0xAA, 0xBB, 0xCC, 0xDD];
                                    let mut rx_data = [0; 4];
                                    channel.send_debug(format!("sending data {:02X?}", tx_data))?;
                                    channel.send_debug(format!("MOSI shorted to MISO"))?;
                                    spi.transfer(&mut rx_data, &tx_data).map_err(|e| e.to_string())?;
                                    if tx_data == rx_data {
                                        channel.send_post(format!("MOSI and MISO are working correct"))?;
                                        channel.send_data(Reply::Loopback(true)).to_worker_result()?;
                                    } else {
                                        channel.send_debug(format!("received data: {:02X?}", rx_data))?;
                                        channel.send_error(format!("received data does not match sent data"))?;
                                        channel.send_data(Reply::Loopback(true)).to_worker_result()?;
                                    }
                                }
                                Request::Transfer { tx_data, rx_size } => {
                                    let mut rx_data = vec![0u8; rx_size];
                                    spi.transfer(&mut rx_data, &tx_data).map_err(|e| e.to_string())?;
                                    channel
                                        .send_data(Reply::ReceivedData(rx_data.to_vec()))
                                        .to_worker_result()?;
                                }
                            }

                            Ok(())
                        }) {
                            error!("{err}");
                            break;
                        }
                    }

                    Ok(())
                },
                32,
                notify,
                on_msg,
            ),
            cb: on_data,
        })
    }

    pub fn process_reply_ptr(spi: *mut hw_spi) -> bool {
        if spi.is_null() {
            error!("NULL pointer");
            return false;
        }

        let spi = unsafe { &*spi };
        spi.obj.recv(|rep| match rep {
            Reply::Loopback(result) => spi.cb.loopback(result),
            Reply::ReceivedData(items) => {
                spi.cb.received(&items);
            }
        });
        true
    }

    pub fn send_request(spi: *mut hw_spi, req: Request) -> bool {
        if spi.is_null() {
            error!("NULL pointer");
            return false;
        }

        let spi = unsafe { &*spi };
        spi.obj.send(req)
    }
}
