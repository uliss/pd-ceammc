use std::{convert::Infallible, time::Duration};

use pn532::{nb, CountDown};

pub struct Timer {}

impl Timer {
    pub fn new() -> Timer {
        Timer {}
    }
}

impl Default for Timer {
    fn default() -> Timer {
        Timer::new()
    }
}

impl CountDown for Timer {
    type Time = Duration;

    fn start<T>(&mut self, count: T)
    where
        T: Into<Self::Time>,
    {
        std::thread::sleep(count.into());
    }

    fn wait(&mut self) -> nb::Result<(), Infallible> {
        Ok(())
    }
}
