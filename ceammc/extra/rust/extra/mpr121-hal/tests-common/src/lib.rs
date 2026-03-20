#![no_std]
use mpr121_hal::mpr121::Mpr121;

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

use hal_imports::*;

pub fn generic_test_new(i2c: impl I2c, delay: &mut impl DelayNs) {
    let mpr121_sensor = Mpr121::new(i2c, mpr121_hal::Mpr121Address::Default, delay, true);
    assert!(mpr121_sensor.is_ok());
}

pub fn generic_test_new_default(i2c: impl I2c, delay: &mut impl DelayNs) {
    let mpr121_sensor = Mpr121::new_default(i2c, delay);
    assert!(mpr121_sensor.is_ok());
}

pub fn generic_test_is_over_current_set(i2c: impl I2c, delay: &mut impl DelayNs) {
    let mut mpr121_sensor = Mpr121::new(i2c, mpr121_hal::Mpr121Address::Default, delay, true)
        .expect("Sensor Initialisation should not fail");
    let over_current_flag = mpr121_sensor
        .is_over_current_set()
        .expect("Communication with sensor should not fail");

    assert!(!over_current_flag);
}

pub fn generic_test_get_touched(i2c: impl I2c, delay: &mut impl DelayNs) {
    let mut mpr121_sensor = Mpr121::new(i2c, mpr121_hal::Mpr121Address::Default, delay, true)
        .expect("Sensor Initialisation should not fail");
    assert!(
        mpr121_sensor
            .get_touched()
            .expect("Communication should not fail")
            == 0
    ); // Nothing should be triggered if not connected to anything
}
