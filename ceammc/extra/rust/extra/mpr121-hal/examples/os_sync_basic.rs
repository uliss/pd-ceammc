use ftdi::Device;
use ftdi_embedded_hal::Delay;
use ftdi_embedded_hal::{self as hal, I2c};
use mpr121_hal::mpr121::Mpr121;
use mpr121_hal::Channel;
use std::error::Error;

#[cfg(feature = "async")]
compile_error!("You cant run this example in async mode. Try setting the sync feature");

fn main() {
    // This is a placeholder for the main function.
    // You can add your code here to test the MPR121 functionality.
    let i2c_bus = setup_i2c().unwrap();

    // You can now use the i2c instance to communicate with the MPR121.
    let mut mpr121 = Mpr121::new(
        i2c_bus,
        mpr121_hal::Mpr121Address::Default,
        &mut Delay::new(),
        true,
    )
    .unwrap();
    loop {
        // Call single read get_touched method
        println!("Calling get_touched! (grouped touch channels");
        let touch_status = mpr121.get_touched().unwrap();
        println!("Touch status: {:?}", touch_status);
        std::thread::sleep(std::time::Duration::from_millis(500));
        let release_status = mpr121.get_touched().unwrap();
        println!("Release status: {:?}", release_status);

        // Call individual channel methods
        // Wait as to not spam the console
        std::thread::sleep(std::time::Duration::from_millis(1000));
        println!("Calling get_snesor_touch! (individual touch channels");
        for selected in 0..Channel::NUM_CHANNELS {
            println!(
                "Channel: {:?} : Touch Status: {:?}",
                selected,
                mpr121
                    .get_sensor_touch(Channel::try_from(selected).unwrap())
                    .unwrap(),
            );
        }
        // Add a delay to avoid flooding the output
        std::thread::sleep(std::time::Duration::from_millis(500));
    }
}

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
