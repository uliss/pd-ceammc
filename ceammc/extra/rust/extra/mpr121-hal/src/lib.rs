//! # MPR121-Hal
//!
//! This crate follows the [Adafruit implementation](https://github.com/adafruit/Adafruit_MPR121) closely but exposes it in terms of the embedded-hal project.
//!
//! The main responsibility of this crate is returning the current on/off state of all the (up to) 12 pins.
//!
//! The chip's data sheet can be found [here](https://www.nxp.com/docs/en/data-sheet/MPR121.pdf). The implementation however mostly mirrors the Adafruit implementation,
//! since this is probably the most widely used one.
//!
//! When working with this crate you can either use it in Synchronous/Blocking mode with the [embedded-hal](https://crates.io/crates/embedded-hal) or in Asynchronous mode with the [embedded-hal-async](https://crates.io/crates/embedded-hal-async).
//!This can be done by using the features `sync` and `async`. This crate does not pull in the `std` library and thus is fully `no-std`.
//! For MCU scale devices [Embassy](https://github.com/embassy-rs/embassy) is a valid framework to use the async feature or [Tokio](https://tokio.rs/) when using Linux/MacOS based devices.
#![no_std]
#![deny(unsafe_code, warnings)]

use num_enum::{IntoPrimitive, TryFromPrimitive};
use registers::Register;

mod communications;
pub mod mpr121;
mod registers;

#[cfg(all(feature = "sync", feature = "async"))]
compile_error!("You cannot use both sync and async features at the same time. Please choose one.");

#[cfg(all(not(feature = "async"), not(feature = "sync")))]
compile_error!("You must enable either the sync or async feature. Please choose one.");

/// The MPR121 Device has an Enumeration of potential driver errors, which are held in the enum below
#[derive(Clone, Copy, Debug, PartialEq, PartialOrd, Eq, Ord)]
pub enum Mpr121Error {
    ///If an operation exceeds the channel count (typically 12).
    ChannelExceed,
    ///If a read operation failed, contains the address that failed.
    ReadError(Register),
    /// If a data conversion failed, contains the address that failed to convert from
    DataConversionError(Register),
    ///If a write operation failed, contains the address that failed.
    WriteError(Register),
    ///If sending the reset signal failed, contains the register that failed.
    ResetFailed { was_read: bool, reg: Register },
    /// During Startup if there is an overcurrent detection, the driver will fail to initialise indicating a hardware fault
    OverCurrent,
    /// Wrong Device Connected, this can also happen if the device state is invalid possible due to a short circuit/overcurrent event
    WrongDevice {
        mismatched_register: Register,
        expected: u8,
        actual: u8,
    },
}

///The four values the sensor can be addressed as. Note that the address of the device is determined by
/// where the `ADDR` pin is connected to. Default is used if no connection, or a connection to `VSS` is made.
///
/// Have a look at page 4 "serial communication" for further specification.
#[repr(u8)]
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, IntoPrimitive)]
pub enum Mpr121Address {
    Default = 0x5a,
    Vdd = 0x5b,
    Sda = 0x5c,
    Scl = 0x5d,
}

#[repr(u8)]
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord, IntoPrimitive, TryFromPrimitive, Debug)]
/// This enum represents the channels of the sensor and is used to get the corresponding touch values
pub enum Channel {
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Eleven,
}

impl Channel {
    pub const NUM_CHANNELS: u8 = 12;
    /// Returns the bit mask associated with the selected channel
    pub(crate) fn get_bit_mask(self) -> u16 {
        1 << u8::from(self)
    }
}

#[repr(u8)]
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, IntoPrimitive, TryFromPrimitive)]
/// This enum represents the number of debounces see section 5.7 in the [MPR121 Data Sheet](https://www.nxp.com/docs/en/data-sheet/MPR121.pdf)
pub enum DebounceNumber {
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
}
