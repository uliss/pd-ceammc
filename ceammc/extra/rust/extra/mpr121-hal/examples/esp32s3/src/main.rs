#![no_std]
#![no_main]

use defmt::info;
use embassy_embedded_hal::shared_bus::asynch::i2c::I2cDevice;
use embassy_executor::Spawner;
use embassy_sync::{blocking_mutex::raw::NoopRawMutex, mutex::Mutex};

use embassy_time::Duration;
use embassy_time::Timer;
use esp_hal::clock::CpuClock;
use esp_hal::i2c::master::Config;
use esp_hal::i2c::master::I2c;
use esp_hal::timer::systimer::SystemTimer;
use esp_hal::Async;
use mpr121_hal::mpr121::Mpr121;
use mpr121_hal::Mpr121Address;
use panic_rtt_target as _;
use static_cell::StaticCell;

extern crate alloc;

#[esp_hal_embassy::main]
async fn main(spawner: Spawner) {
    // generator version: 0.3.1

    rtt_target::rtt_init_defmt!();

    let config = esp_hal::Config::default().with_cpu_clock(CpuClock::max());
    let peripherals = esp_hal::init(config);

    esp_alloc::heap_allocator!(size: 72 * 1024);

    let timer0 = SystemTimer::new(peripherals.SYSTIMER);
    esp_hal_embassy::init(timer0.alarm0);

    info!("Embassy initialized!");

    // TODO: Spawn some tasks
    let _ = spawner;

    let i2c_bus: &mut Mutex<NoopRawMutex, I2c<'_, Async>> = {
        static BUS: StaticCell<Mutex<NoopRawMutex, I2c<'_, esp_hal::Async>>> = StaticCell::new();
        BUS.init(Mutex::new(
            I2c::new(peripherals.I2C0, Config::default())
                .expect("Failed to initialize I2C")
                .with_scl(peripherals.GPIO9)
                .with_sda(peripherals.GPIO8)
                .into_async(),
        ))
    };
    let i2c = I2cDevice::new(i2c_bus);
    let mut mpr121 = Mpr121::new(i2c, Mpr121Address::Default, &mut embassy_time::Delay, true)
        .await
        .expect("Failed to initialize MPR121");

    loop {
        // Read the touch status
        let touch_status = mpr121.get_touched().await.unwrap();
        info!("Touch status: {:?}", touch_status);

        // Read the release statuse
        let release_status = mpr121.get_touched().await.unwrap();
        info!("Release status: {:?}", release_status);

        // Add a delay to avoid flooding the output
        Timer::after(Duration::from_millis(500)).await;
    }
}
