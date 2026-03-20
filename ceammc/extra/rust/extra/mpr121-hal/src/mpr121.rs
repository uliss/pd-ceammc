#[cfg(feature = "sync")]
mod hal_imports {
    pub use embedded_hal::delay::DelayNs;
    pub use embedded_hal::i2c::I2c;
}

#[cfg(feature = "async")]
mod hal_imports {
    pub use embedded_hal_async::delay::DelayNs;
    pub use embedded_hal_async::i2c::I2c;
}

use crate::{registers::*, Channel, DebounceNumber};
use crate::{Mpr121Address, Mpr121Error};
use hal_imports::*;

/// This is the sensor itself and takes in an I2C Device or bus.
/// See the examples folder for more details.
/// The driver can work in either Async or Sync mode by specifying the the feature "async" or "sync".
pub struct Mpr121<I2C: I2c> {
    pub(crate) i2c: I2C,
    pub(crate) addr: Mpr121Address,
}

impl<I2C: I2c> Mpr121<I2C> {
    /// Default Threshold
    pub const DEFAULT_TOUCH_THRESHOLD: u8 = 12;
    /// Default Release
    pub const DEFAULT_RELEASE_THRESOLD: u8 = 6;
    /// The value to be written to soft reset register, to trigger a reset
    pub(crate) const SOFT_RESET_VALUE: u8 = 0x63;

    ///Creates the driver for the given I²C ports. Assumes that the I²C port is configured as master.
    ///
    /// If `use_auto_config` is set, the controller will use its auto configuration routine to setup
    /// charging parameters whenever it is transitioned from STOP to START mode.
    ///
    /// Note that we use the same default values as the Adafruit implementation, except for threshold values.
    /// Use [set_thresholds](Self::set_thresholds) to define those.
    ///
    /// In the event of an error, returns [Mpr121Error]
    #[maybe_async::maybe_async]
    pub async fn new(
        i2c: I2C,
        addr: Mpr121Address,
        delay: &mut impl DelayNs,
        use_auto_config: bool,
    ) -> Result<Self, Mpr121Error> {
        let mut dev = Mpr121 { i2c, addr };
        dev.reset_verify(delay).await?;

        // Check for overcurrent
        if dev.is_over_current_set().await? {
            return Err(Mpr121Error::OverCurrent);
        }

        // Put Device in Stop Mode
        dev.write_register(Register::Ecr, 0x0).await?;
        //Initialise the device to the similar settings as Adafruit
        dev.set_thresholds(
            Self::DEFAULT_TOUCH_THRESHOLD,
            Self::DEFAULT_RELEASE_THRESOLD,
        )
        .await?;
        dev.initialise_registers(use_auto_config).await?;

        Ok(dev)
    }

    #[maybe_async::maybe_async]
    async fn initialise_registers(&mut self, use_auto_config: bool) -> Result<(), Mpr121Error> {
        //Setup Filters MHD==MaximumHalfDelta, NHD=NoiseHalfDelta
        // Have a look at 5.5 in the data sheet for more information.

        self.write_register(Register::MaximumHalfDeltaRising, 0x01)
            .await?;
        self.write_register(Register::NoiseCountLimitRising, 0x01)
            .await?;
        self.write_register(Register::NoiseCountLimitRising, 0x0e)
            .await?;
        self.write_register(Register::FilterDelayCountLimitRising, 0x00)
            .await?;

        self.write_register(Register::MaximmHalfDeltaFalling, 0x01)
            .await?;
        self.write_register(Register::NoiseHalfDeltaFalling, 0x05)
            .await?;
        self.write_register(Register::NoiseCountLimitFalling, 0x01)
            .await?;
        self.write_register(Register::FilterDelayCountFalling, 0x00)
            .await?;

        self.write_register(Register::NoiseHalfDeltaTouched, 0x00)
            .await?;
        self.write_register(Register::NoiseCountLimitTouched, 0x00)
            .await?;
        self.write_register(Register::FilterDelayCountLimitTouched, 0x00)
            .await?;

        self.write_register(Register::Debounce, DebounceNumber::Zero.into())
            .await?;
        self.write_register(
            Register::GlobalChargeDischargeCurrentConfig,
            Register::GlobalChargeDischargeCurrentConfig.get_initial_value(),
        )
        .await?;
        self.write_register(Register::GlobalChargeDischargeTimeConfig, 0x20)
            .await?;

        if use_auto_config {
            self.write_register(Register::AutoConfig0, 0x0b).await?;
            self.write_register(Register::UpSideLimit, limits::UP_SIDE)
                .await?;
            self.write_register(Register::TargetLevel, limits::TARGET_LEVEL)
                .await?;
            self.write_register(Register::LowSideLimit, limits::LOW_SIDE)
                .await?;
        }
        //enable electrodes and return to start mode // See Datasheet 5.11
        let calibration_lock_bit = 0b1 << 7;
        let ecr_setting = calibration_lock_bit + Channel::NUM_CHANNELS;
        self.write_register(Register::Ecr, ecr_setting).await?;
        Ok(())
    }

    /// This method will reset and verify that the correct device is on the bus, if there is a failed read/write in the process or
    /// if the device registers do not match what is expected. It is likely that the device is not connected. Due to the nature of this function
    /// it should only really be called once as it will reset any prexisting configurations applied
    #[maybe_async::maybe_async]
    async fn reset_verify(&mut self, delay: &mut impl DelayNs) -> Result<(), Mpr121Error> {
        self.reset().await?;
        delay.delay_us(100).await;
        // Verify that the default registers match up
        let register_1 = Register::GlobalChargeDischargeCurrentConfig;
        let read_register_1_config = self.read_reg8(register_1).await?;
        if read_register_1_config != register_1.get_initial_value() {
            return Err(Mpr121Error::WrongDevice {
                mismatched_register: register_1,
                expected: register_1.get_initial_value(),
                actual: read_register_1_config,
            });
        }
        let register_2 = Register::GlobalChargeDischargeTimeConfig;
        let read_register_2_config = self.read_reg8(register_2).await?;
        if read_register_2_config != register_2.get_initial_value() {
            return Err(Mpr121Error::WrongDevice {
                mismatched_register: register_2,
                expected: register_2.get_initial_value(),
                actual: read_register_2_config,
            });
        }
        Ok(())
    }

    /// Performs a software reset on the device, resetting the MPR121 Touch sensor back to default configuration
    #[maybe_async::maybe_async]
    pub async fn reset(&mut self) -> Result<(), Mpr121Error> {
        let result = self
            .write_register(Register::SoftReset, Self::SOFT_RESET_VALUE)
            .await;

        // Map any read/write errors to a failed reset error
        result.err().map(|err| match err {
                Mpr121Error::ReadError(reg) => Mpr121Error::ResetFailed {
                    was_read: true,
                    reg,
                },
                Mpr121Error::WriteError(reg) => Mpr121Error::ResetFailed {
                    was_read: false,
                    reg,
                },
                _ => {
                    unreachable!("There should only be a read or write error at this stage, perhaps a lower level API has changed?")
                }
            });

        Ok(())
    }
    /// Initializes the driver assuming the sensors address is the default one (0x5a).
    /// If this fails, consider searching for the driver.
    /// Or following the documentation on setting a driver address, and use [new](Self::new) to specify the address.
    ///
    /// Have a look at [new](Self::new) for further documentation.
    #[maybe_async::maybe_async]
    pub async fn new_default(i2c: I2C, delay: &mut impl DelayNs) -> Result<Self, Mpr121Error> {
        let result = Self::new(i2c, Mpr121Address::Default, delay, true).await?;
        Ok(result)
    }
    /// Returns true if over-current is detected by the device.
    /// In that case you probably have to check your circuit
    ///
    /// In the event of an error [Mpr121Error] is returned
    #[maybe_async::maybe_async]
    pub async fn is_over_current_set(&mut self) -> Result<bool, Mpr121Error> {
        const OVER_CURRENT_PROTECTION_FLAG_MASK: u8 = 0b1 << 7;
        let read = self.read_reg8(Register::TouchStatus8_11).await?;
        //If bit D7 is set, we have OVCF
        Ok((read & (OVER_CURRENT_PROTECTION_FLAG_MASK)) > 0)
    }

    /// Set the touch and release threshold for all channels. Usually the touch threshold is a little bigger than the release
    /// threshold. This creates some debounce characteristics. The correct thresholds depend on the application.
    ///
    /// Have a look at [note AN3892](https://www.nxp.com/docs/en/application-note/AN3892.pdf) of the mpr121 guidelines.
    /// In the event of an error [Mpr121Error] is returned
    #[maybe_async::maybe_async]
    pub async fn set_thresholds(&mut self, touch: u8, release: u8) -> Result<(), Mpr121Error> {
        for i in 0..Channel::NUM_CHANNELS {
            //Note ignoring false set thresholds
            self.write_register(
                Register::get_threshold_register(
                    Channel::try_from(i).expect("Channel Iteration Should not fail"),
                ),
                touch,
            )
            .await?;
            self.write_register(
                Register::get_release_register(
                    Channel::try_from(i).expect("Channel Iteration should not fail"),
                ),
                release,
            )
            .await?;
        }
        Ok(())
    }

    /// Sets the count for both touch and release. See 5.7 of the [Mpr121 Data Sheet](https://www.nxp.com/docs/en/data-sheet/MPR121.pdf).
    ///
    /// In the event of an error [Mpr121Error] is returned
    #[maybe_async::maybe_async]
    pub async fn set_debounce(
        &mut self,
        trigger_debounce: DebounceNumber,
        release_debounce: DebounceNumber,
    ) -> Result<(), Mpr121Error> {
        let bits = (u8::from(release_debounce) << 4) | (u8::from(trigger_debounce));
        self.write_register(Register::Debounce, bits).await?;
        Ok(())
    }

    /// Reads the filtered data from touch channels. Noise gets filtered out by the
    /// chip. See 5.3 in the data sheet.
    ///
    /// Note that the resulting value is only 10bit wide.
    ///
    /// Otherwise [Mpr121Error] is returned
    #[maybe_async::maybe_async]
    pub async fn get_filtered(&mut self, channel: Channel) -> Result<u16, Mpr121Error> {
        let register = Register::get_filtered_data_msb(channel);
        let result = self.read_reg16(register).await?;
        Ok(result)
    }

    /// Reads the baseline data for the channel. Note that this has only a resolution of 8bit.
    ///
    /// Otherwise [Mpr121Error] is returned
    #[maybe_async::maybe_async]
    pub async fn get_baseline(&mut self, channel: Channel) -> Result<u8, Mpr121Error> {
        //NOTE: the original reads a 8bit value and left shifts 2bit.
        //      While the shift is correct the data sheet mentions:
        //
        //      Although internally the baseline value is 10-bit,
        //      users can only access the 8 MSB of the 10-bit baseline value through the
        //      baseline value registers. The read out from the baseline register must
        //      be left shift two bits before comparing it with the 10-bit
        //      electrode data.
        //
        //      reading only 8bit and shifting 2bit effectively reduces the resolution to
        //      6bit, since we loose the 2MSB.
        //
        //      Therefore we read 16bit, mask out the top 6, and then shift
        let register = Register::get_baseline(channel);
        let mut value = self.read_reg16(register).await?;
        value &= 0b00000011_11111100;
        let cast = (value << 2)
            .try_into() // If this fails to convert into u8
            .map_err(|_| Mpr121Error::DataConversionError(register))?;
        Ok(cast)
    }

    /// Reads the *touched* state of all channels. Returns a u16 where each bit 0..12 indicates whether the
    /// pin is touched or not. Use bit shifting / masking to generate a mask, or, if only one sensor's value is
    /// needed, use [get_touch_state](Self::get_sensor_touch).
    ///
    /// Otherwise [Mpr121Error] is returned
    #[maybe_async::maybe_async]
    pub async fn get_touched(&mut self) -> Result<u16, Mpr121Error> {
        //mask upper four bits returns the rest
        let unmasked = self.read_reg16(Register::TouchStatus0_7).await?;
        Ok(unmasked & 0x0fff)
    }

    ///Returns the touch state of the given sensor.
    ///
    /// Otherwise [Mpr121Error] is returned
    #[maybe_async::maybe_async]
    pub async fn get_sensor_touch(&mut self, channel: Channel) -> Result<bool, Mpr121Error> {
        let result = self.get_touched().await?;
        Ok(result & channel.get_bit_mask() > 0)
    }
}
