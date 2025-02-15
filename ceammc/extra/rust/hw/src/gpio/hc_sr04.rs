use std::{
    ffi::{c_void, CString},
    ptr::null_mut,
    sync::{mpsc::TryRecvError, Arc, Mutex},
    time::Duration,
};

use hc_sr04::{HcSr04, Unit};
use log::{debug, error};

use crate::{hw_msg_cb, hw_notify_cb};

#[repr(C)]
#[allow(non_camel_case_types)]
/// error callback
pub struct hw_sr04_cb {
    /// pointer to user data (can be NULL)
    user: *mut c_void,
    /// not NULL!
    cb: extern "C" fn(*mut c_void, distance: f32),
}

impl hw_sr04_cb {
    pub fn exec(&self, data: f32) {
        (self.cb)(self.user, data);
    }
}

#[derive(Debug)]
enum Reply {
    Measure(Option<f32>),
    Error(CString),
}

enum Request {
    Poll(bool),
    OneShot,
}

pub struct hw_gpio_sr04 {
    result: Arc<Mutex<Option<Reply>>>,
    tx: std::sync::mpsc::Sender<Request>,
    on_err: hw_msg_cb,
    on_data: hw_sr04_cb,
}

impl hw_gpio_sr04 {
    fn new(
        trigger_pin: u8,
        echo_pin: u8,
        notify: hw_notify_cb,
        on_err: hw_msg_cb,
        on_data: hw_sr04_cb,
    ) -> Result<Self, CString> {
        let result = Arc::new(Mutex::new(None));
        let result2 = result.clone();

        let (tx, rx) = std::sync::mpsc::channel();

        std::thread::spawn(move || {
            debug!("thread start");

            let sr04 = HcSr04::new(trigger_pin, echo_pin, None);
            if let Err(err) = &sr04 {
                if let Err(err) = result2.lock().and_then(|mut x| {
                    let r = x.replace(Reply::Error(
                        CString::new(err.to_string()).unwrap_or_default(),
                    ));
                    notify.notify();
                    Ok(r)
                }) {
                    error!("{err}");
                }
                return;
            }

            debug!("create HcSr04 with trig [{trigger_pin}], echo [{echo_pin}]");

            let mut sr04 = sr04.unwrap();
            let mut cycle_mode = false;

            loop {
                match rx.try_recv() {
                    Ok(req) => match req {
                        Request::Poll(state) => {
                            cycle_mode = state;
                        }
                        Request::OneShot => {
                            debug!("one shot");
                            cycle_mode = false;
                            Self::proc_sensor_data(&mut sr04, &result2, &notify);
                        }
                    },
                    Err(err) => match err {
                        TryRecvError::Empty => {
                            debug!("sleep");
                            std::thread::sleep(Duration::from_millis(1000));
                        },
                        TryRecvError::Disconnected => break,
                    },
                };

                if cycle_mode {
                    Self::proc_sensor_data(&mut sr04, &result2, &notify);
                    std::thread::sleep(Duration::from_millis(1000));
                }
            }

            debug!("thread done");
        });

        Ok(hw_gpio_sr04 {
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
        sensor: &mut HcSr04,
        result: &Arc<Mutex<Option<Reply>>>,
        notify: &hw_notify_cb,
    ) {
        let reply = sensor
            .measure_distance(Unit::Centimeters)
            .map(|res| Reply::Measure(res))
            .unwrap_or_else(|err| Reply::Error(CString::new(err.to_string()).unwrap_or_default()));

        debug!("measure: {reply:?}");

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
                        res.map(|f| self.on_data.exec(f));
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

/// create new SR04 sensor struct
/// @param trigger_pin - connected GPIO pin
/// @param trigger_pin - connected GPIO pin
/// @param notify - data check callback
/// @param on_err - error message callback
/// @param on_data - data callback
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_new(
    trigger_pin: u8,
    echo_pin: u8,
    notify: hw_notify_cb,
    on_err: hw_msg_cb,
    on_data: hw_sr04_cb,
) -> *mut hw_gpio_sr04 {
    gpio_check!(null_mut(), {
        match hw_gpio_sr04::new(trigger_pin, echo_pin, notify, on_err, on_data) {
            Ok(sr04) => return Box::into_raw(Box::new(sr04)),
            Err(err) => {
                error!("{}", err.to_str().unwrap_or_default());
                on_err.exec_raw(err.as_ptr());
                return null_mut();
            }
        }
    });
}

/// free dht11 sensor
/// @param sr04 - pointer to SR04 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_free(sr04: *mut hw_gpio_sr04) {
    gpio_check!((), {
        if !sr04.is_null() {
            drop(unsafe { Box::from_raw(sr04) })
        }
    });
}

/// singe measure request
/// @param sr04 - pointer to SR04 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_measure(sr04: *const hw_gpio_sr04) -> bool {
    gpio_check!({
        if sr04.is_null() {
            error!("NULL dht pointer");
            return false;
        }

        let sr04 = unsafe { &*sr04 };
        sr04.send(Request::OneShot)
    });
}

/// check measure data
/// @param sr04 - pointer to SR04 struct
#[no_mangle]
pub extern "C" fn ceammc_hw_gpio_sr04_process(sr04: *const hw_gpio_sr04) -> bool {
    gpio_check!({
        if sr04.is_null() {
            error!("NULL sr04 pointer");
            return false;
        }

        let sr04 = unsafe { &*sr04 };
        sr04.check_result();
        true
    });
}
