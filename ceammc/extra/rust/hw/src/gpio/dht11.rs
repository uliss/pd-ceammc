use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
    sync::{Arc, Mutex},
    time::Duration,
};

use dht11_gpio::{DHT11Controller, DHT11Result, Sensor};
use log::{debug, error};

use crate::{hw_msg_cb, hw_notify_cb};

#[repr(C)]
#[allow(non_camel_case_types)]
/// error callback
pub struct hw_dht11_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// not NULL!
    cb: extern "C" fn(*mut c_void, temp: f64, hum: f64),
}

impl hw_dht11_cb {
    pub fn exec(&self, data: &DHT11Result) {
        (self.cb)(self.user, data.temperature, data.humidity);
    }
}

enum Reply {
    Measure(DHT11Result),
    Error(CString),
}

enum Request {
    Poll(bool),
    OneShot,
    Quit,
}

pub struct hw_gpio_dht11 {
    result: Arc<Mutex<Option<Reply>>>,
    tx: std::sync::mpsc::Sender<Request>,
    on_err: hw_msg_cb,
    on_data: hw_dht11_cb,
}

impl hw_gpio_dht11 {
    fn new(
        pin: u8,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
        on_data: hw_dht11_cb,
    ) -> Result<Self, CString> {
        let result = Arc::new(Mutex::new(None));
        let result2 = result.clone();

        let (tx, rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || {
            debug!("thread start");

            let dht11 = DHT11Controller::new(pin);
            if let Err(err) = &dht11 {
                if let Err(err) = result2.lock().and_then(|mut x| {
                    Ok(x.replace(Reply::Error(
                        CString::new(err.to_string()).unwrap_or_default(),
                    )))
                }) {
                    error!("{err}");
                }
                return;
            }

            let mut dht11 = dht11.unwrap();
            let mut cycle_mode = false;

            'outer: loop {
                'inner: while let Ok(req) = rx.recv_timeout(Duration::from_millis(200)) {
                    match req {
                        Request::Poll(state) => {
                            cycle_mode = state;

                            if cycle_mode {
                                break 'inner;
                            }
                        }
                        Request::OneShot => {
                            cycle_mode = false;
                            Self::proc_sensor_data(&mut dht11, &result2, &notify);
                        }
                        Request::Quit => break 'outer,
                    }
                }

                if cycle_mode {
                    Self::proc_sensor_data(&mut dht11, &result2, &notify);
                    std::thread::sleep(Duration::from_millis(1000));
                }
            }
        });

        Ok(hw_gpio_dht11 {
            result,
            tx,
            on_err,
            on_data,
        })
    }

    fn send(&self, req: Request) -> bool {
        if let Err(err) = self.tx.send(req) {
            error!("{err}");
            self.on_err.exec(err.to_string().as_str());
            false
        } else {
            true
        }
    }

    fn proc_sensor_data(
        sensor: &mut DHT11Controller,
        result: &Arc<Mutex<Option<Reply>>>,
        notify: &hw_notify_cb,
    ) {
        let reply = sensor
            .read_sensor_data()
            .map(|res| {
                debug!("measure done t={}°C h={}", res.temperature, res.humidity);
                Reply::Measure(res)
            })
            .unwrap_or_else(|err| Reply::Error(CString::new(err.to_string()).unwrap_or_default()));

        result
            .lock()
            .and_then(|mut m| {
                m.replace(reply);
                notify.notify();
                Ok(())
            })
            .unwrap_or_else(|err| {
                error!("{err}");
            });
    }

    fn check_result(&self) {
        match self.result.lock() {
            Ok(res) => match res.as_ref() {
                Some(reply) => match reply {
                    Reply::Measure(res) => {
                        self.on_data.exec(res);
                    }
                    Reply::Error(msg) => self.on_err.exec_raw(msg.as_ptr()),
                },
                None => self.on_err.exec("None"),
            },
            Err(err) => {
                self.on_err.exec(err.to_string().as_str());
            }
        }
    }
}

/// create new DHT11 sensor struct
/// @param pin - connected GPIO pin
/// @param notify - data check callback
/// @param on_err - error message callback
/// @param on_data - data callback
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_new(
    pin: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
    on_data: hw_dht11_cb,
) -> *mut hw_gpio_dht11 {
    gpio_check!(null_mut(), {
        match hw_gpio_dht11::new(pin, notify, on_err, on_data) {
            Ok(dht) => return Box::into_raw(Box::new(dht)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

/// free dht11 sensor
/// @param dht - pointer to DHT11 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_free(dht: *mut hw_gpio_dht11) {
    gpio_check!((), {
        if !dht.is_null() {
            let dht11 = unsafe { &*dht };
            if let Err(err) = dht11.tx.send(Request::Quit) {
                error!("{err}");
                dht11.on_err.exec(err.to_string().as_str());
            }

            drop(unsafe { Box::from_raw(dht) })
        }
    });
}

/// singe measure request
/// @param dht - pointer to DHT11 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_measure(dht: *const hw_gpio_dht11) -> bool {
    gpio_check!({
        if dht.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let dht = unsafe { &*dht };
        dht.send(Request::OneShot)
    });
}

/// poll request
/// @param dht - pointer to DHT11 struct
/// @param state - poll state
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_poll(dht: *const hw_gpio_dht11, state: bool) -> bool {
    gpio_check!({
        if dht.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let dht = unsafe { &*dht };
        dht.send(Request::Poll(state))
    });
}

/// check measure data
/// @param dht - pointer to DHT11 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_process(dht: *const hw_gpio_dht11) -> bool {
    gpio_check!({
        if dht.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let dht = unsafe { &*dht };
        dht.check_result();
        true
    });
}
