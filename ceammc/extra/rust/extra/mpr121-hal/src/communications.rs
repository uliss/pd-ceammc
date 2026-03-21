#[cfg(feature = "sync")]
use embedded_hal::i2c::I2c;
#[cfg(feature = "async")]
use embedded_hal_async::i2c::I2c;

use crate::{mpr121::Mpr121, registers::*, Mpr121Error};

impl<I2C: I2c> Mpr121<I2C> {
    #[maybe_async::maybe_async]
    //Write implementation. Returns an error if a read or write operation failed. The error contains the failing register.
    pub(crate) async fn write_register(
        &mut self,
        reg: Register,
        value: u8,
    ) -> Result<(), Mpr121Error> {
        let addr_val: u8 = self.addr.into();
        //Check in which mode we are by reading ECR.
        let ecr_state = self.read_reg8(Register::Ecr).await?;

        // Detect if sensor is already stopped, See Datasheet 5.11
        let ecr_stop_mode_bit_mask: u8 = 0b00111111;
        let stopped = (ecr_state & ecr_stop_mode_bit_mask) == 0; // At least one of the electrodes is on if the bitmasked register is not 0

        if reg.require_stop() && !stopped {
            //set to stop
            self.i2c
                .write(
                    addr_val,
                    &[
                        Register::Ecr.into(),
                        Register::get_initial_value(&Register::Ecr),
                    ],
                )
                .await
                .map_err(|_| Mpr121Error::WriteError(Register::Ecr))?;
        }

        //actual write
        self.i2c
            .write(addr_val, &[reg.into(), value])
            .await
            .map_err(|_| Mpr121Error::WriteError(reg))?;

        //reset to old ecr state
        if reg.require_stop() && !stopped {
            self.i2c
                .write(addr_val, &[Register::Ecr.into(), ecr_state])
                .await
                .map_err(|_| Mpr121Error::WriteError(Register::Ecr))?;
        }

        Ok(())
    }

    #[maybe_async::maybe_async]
    //Reads the value, returns Err, if reading failed.
    pub(crate) async fn read_reg8(&mut self, reg: Register) -> Result<u8, Mpr121Error> {
        let mut val = [0u8];
        self.i2c
            .write_read(self.addr.into(), &[reg.into()], &mut val)
            .await
            .map_err(|_| Mpr121Error::ReadError(reg))?;
        Ok(val[0])
    }

    #[maybe_async::maybe_async]
    //Reads the value, returns Err, if reading failed.
    pub(crate) async fn read_reg16(&mut self, reg: Register) -> Result<u16, Mpr121Error> {
        let mut val = [0u8, 0u8];
        self.i2c
            .write_read(self.addr.into(), &[reg.into()], &mut val)
            .await
            .map_err(|_| Mpr121Error::ReadError(reg))?;
        Ok(u16::from_le_bytes(val))
    }
}
