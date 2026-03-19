use std::{
    convert::Infallible,
    time::{Duration, Instant},
};

use pn532::{nb, CountDown};

/// A timer based on [`std::time::Instant`], which is a monotonically nondecreasing clock.
pub struct Timer {
    start: Instant,
    duration: Duration,
}

impl Timer {
    pub fn new() -> Timer {
        Timer {
            start: Instant::now(),
            duration: Duration::from_millis(0),
        }
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
        self.start = Instant::now();
        self.duration = count.into();
    }

    fn wait(&mut self) -> nb::Result<(), Infallible> {
        if (Instant::now() - self.start) >= self.duration {
            Ok(())
        } else {
            Err(nb::Error::WouldBlock)
        }
    }
}
