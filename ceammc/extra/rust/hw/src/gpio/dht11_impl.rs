use dht11_gpio::{DHT11Controller, DHT11Result, Sensor};

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

            loop {
                match rx.try_recv() {
                    Ok(req) => match req {
                        Request::Poll(state) => {
                            cycle_mode = state;
                        }
                        Request::OneShot => {
                            cycle_mode = false;
                            Self::proc_sensor_data(&mut dht11, &result2, &notify);
                        }
                    },
                    Err(err) => match err {
                        TryRecvError::Empty => std::thread::sleep(Duration::from_millis(200)),
                        TryRecvError::Disconnected => break,
                    },
                };

                if cycle_mode {
                    Self::proc_sensor_data(&mut dht11, &result2, &notify);
                    std::thread::sleep(Duration::from_millis(1000));
                }
            }

            debug!("thread done");
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