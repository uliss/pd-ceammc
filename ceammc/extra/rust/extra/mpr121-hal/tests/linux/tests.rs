use crate::i2c_driver;
use tests_common::*;

#[test]
pub fn test_new() {
    let i2c_bus = i2c_driver::setup_i2c().expect("I2C Bus failed to acquire");
    let mut delay = i2c_driver::setup_delay();
    generic_test_new(i2c_bus, &mut delay);
}
#[test]
pub fn test_new_default() {
    let i2c_bus = i2c_driver::setup_i2c().expect("I2C Bus failed to acquire");
    let mut delay = i2c_driver::setup_delay();
    generic_test_new_default(i2c_bus, &mut delay);
}
#[test]
pub fn test_is_over_current_set() {
    let i2c_bus = i2c_driver::setup_i2c().expect("I2C Bus failed to acquire");
    let mut delay = i2c_driver::setup_delay();
    generic_test_is_over_current_set(i2c_bus, &mut delay);
}
#[test]
pub fn test_get_touched() {
    let i2c_bus = i2c_driver::setup_i2c().expect("I2C Bus failed to acquire");
    let mut delay = i2c_driver::setup_delay();
    generic_test_get_touched(i2c_bus, &mut delay);
}
