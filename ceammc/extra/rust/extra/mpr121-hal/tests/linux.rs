//! Linux / MacOS Specific Tests, responsible for creating the I2C Device thriugh the FT232H Breakout Board. These tests run in sync mode
#[cfg(test)]
pub mod linux {
    pub mod tests;
}
#[cfg(feature = "sync")]
mod i2c_driver {
    use embedded_hal::delay::DelayNs;
    use ftdi::Device;
    use ftdi_embedded_hal::Delay;
    use ftdi_embedded_hal::{self as hal, I2c};
    use std::error::Error;

    pub fn setup_i2c() -> Result<I2c<Device>, Box<dyn Error>> {
        const BAUDRATE: u32 = 400_000;
        // Change these for your device
        const DEVICE_VID: u16 = 0x0403;
        const DEVICE_PID: u16 = 0x6014;

        let device = ftdi::find_by_vid_pid(DEVICE_VID, DEVICE_PID)
            .interface(ftdi::Interface::A)
            .open()?;
        // Next initialise the HAL with the device and the Baudrate
        let hal = match hal::FtHal::init_freq(device, BAUDRATE) {
            Ok(hal) => hal,
            Err(err) => {
                eprintln!("Failed to initialise HAL: {}", err);
                return Err(Box::new(err));
            }
        };
        // Finally initialise the I2C with the HAL
        let i2c = match hal.i2c() {
            Ok(i2c) => i2c,
            Err(err) => {
                eprintln!("Failed to initialise I2C: {}", err);
                return Err(Box::new(err));
            }
        };
        Ok(i2c)
    }
    pub fn setup_delay() -> impl DelayNs {
        Delay::new()
    }
}
