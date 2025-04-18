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
