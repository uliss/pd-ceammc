use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
    sync::{Arc, Mutex},
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
    /// can not be NULL
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

pub struct hw_gpio_dht11 {
    sensor: Arc<Mutex<DHT11Controller>>,
    rx: std::sync::mpsc::Receiver<Reply>,
    tx: std::sync::mpsc::Sender<Reply>,
    notify: hw_notify_cb,
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
        let sensor = Arc::new(Mutex::new(
            DHT11Controller::new(pin)
                .map_err(|e| CString::new(e.to_string()).unwrap_or_default())?,
        ));

        let (tx, rx) = std::sync::mpsc::channel();

        Ok(hw_gpio_dht11 {
            sensor,
            rx,
            tx,
            notify,
            on_err,
            on_data,
        })
    }

    fn do_measure(sensor: &Arc<Mutex<DHT11Controller>>) -> Result<DHT11Result, String> {
        debug!("do_measure");

        Ok(sensor
            .lock()
            .map_err(|e| e.to_string())?
            .read_sensor_data()
            .map_err(|e| e.to_string())?)
    }

    fn measure(&mut self) {
        let tx = self.tx.clone();
        let notify = self.notify.clone();
        let sensor = self.sensor.clone();

        std::thread::spawn(move || {
            debug!("thread start");

            match Self::do_measure(&sensor) {
                Ok(res) => {
                    debug!("measure done {} {}", res.humidity, res.temperature);

                    let data = Reply::Measure(res);
                    let _ = tx
                        .send(data)
                        .and_then(|_| Ok(notify.notify()))
                        .or_else(|e| Err(error!("{e}")));
                }
                Err(err) => {
                    let _ = tx
                        .send(Reply::Error(CString::new(err).unwrap_or_default()))
                        .and_then(|_| Ok(notify.notify()))
                        .or_else(|e| Err(error!("{e}")));
                }
            }
            debug!("thread done");
        });
    }

    fn check_result(&self) {
        while let Ok(res) = self.rx.try_recv() {
            debug!("check_result");
            match res {
                Reply::Measure(res) => self.on_data.exec(&res),
                Reply::Error(msg) => {
                    self.on_err.exec_raw(msg.as_ptr());
                }
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
            drop(unsafe { Box::from_raw(dht) })
        }
    });
}

/// request measure
/// @param dht - pointer to DHT11 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_measure(dht: *mut hw_gpio_dht11) -> bool {
    gpio_check!({
        if dht.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let dht = unsafe { &mut *dht };

        dht.measure();

        false
    });
}

/// check measure data
/// @param dht - pointer to DHT11 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_dht11_process(dht: *mut hw_gpio_dht11) -> bool {
    gpio_check!({
        if !dht.is_null() {
            drop(unsafe { Box::from_raw(dht) })
        }

        if dht.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let dht = unsafe { &mut *dht };
        dht.check_result();
        true
    });
}
