mod mqtt {

    use bytes::Bytes;
    use rumqttc::{
        Client, ConnectReturnCode, Connection, Event, MqttOptions, Packet, QoS, RecvTimeoutError,
    };
    use std::ffi::{CStr, CString};
    use std::os::raw::{c_char, c_void};
    use std::slice;
    use std::sync::Mutex;
    use std::time::Duration;

    #[allow(dead_code)]
    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub enum mqtt_qos {
        /// may lose messages
        AtMostOnce = 0,
        /// guarantees the message delivery but potentially exists duplicate messages
        AtLeastOnce = 1,
        /// ensures that messages are delivered exactly once without duplication
        ExactlyOnce = 2,
    }

    #[derive(Debug)]
    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub enum mqtt_rc {
        Ok = 0,
        InvalidString,
        InvalidData,
        InvalidClient,
        ClientError,
        Disconnected,
        ConnectionError,
    }

    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub struct mqtt_err_cb {
        pub user: *mut c_void,
        pub cb: Option<extern "C" fn(user: *mut c_void, msg: *const c_char)>,
    }

    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub struct mqtt_ping_cb {
        pub user: *mut c_void,
        pub cb: Option<extern "C" fn(user: *mut c_void)>,
    }

    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub struct mqtt_pub_cb {
        pub user: *mut c_void,
        pub cb: Option<
            extern "C" fn(user: *mut c_void, topic: *const c_char, data: *const u8, len: usize),
        >,
    }

    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub struct mqtt_cb {
        on_err: mqtt_err_cb,
        on_ping: mqtt_ping_cb,
        on_pub: mqtt_pub_cb,
    }

    impl mqtt_cb {
        fn err(&self, msg: &str) {
            let msg = CString::new(msg).unwrap();
            self.on_err.cb.map(|cb| {
                cb(self.on_err.user, msg.as_ptr());
            });
        }
        fn ping(&self) {
            self.on_ping.cb.map(|cb| {
                cb(self.on_err.user);
            });
        }
        fn publish(&self, topic: &String, data: &Bytes) {
            let topic = CString::new(topic.as_bytes()).unwrap();
            self.on_pub.cb.map(|cb| {
                cb(self.on_pub.user, topic.as_ptr(), data.as_ptr(), data.len());
            });
        }
    }

    #[allow(non_camel_case_types)]
    pub struct mqtt_client {
        mqtt: Mutex<(Client, Connection)>,
        cb: mqtt_cb,
    }

    impl mqtt_client {
        fn new(
            id: &str,
            host: &str,
            port: u16,
            user: &str,
            pass: &str,
            keep_alive: u8,
            cb: mqtt_cb,
        ) -> Self {
            let id = if id.is_empty() { "ceammc_mqtt_pd" } else { id };
            let mut mqttoptions = MqttOptions::new(id, host, port);
            mqttoptions.set_keep_alive(Duration::from_secs(keep_alive.into()));
            if !user.is_empty() && !pass.is_empty() {
                mqttoptions.set_credentials(user, pass);
                println!("connect with {user}:{pass}");
            }

            let mqtt = Mutex::new(Client::new(mqttoptions, 10));
            mqtt_client { mqtt, cb }
        }
    }

    #[no_mangle]
    /// create new mqtt client
    /// @param host - mqtt broker hostname
    /// @param port - mqtt broker port
    /// @param id - client id (can be NULL)
    /// @param user - mqtt username (can be NULL)
    /// @param pass - mqtt password (can be NULL)
    /// @param cb - callbacks
    /// @return pointer to mqtt_client (must be freed by ceammc_mqtt_client_free()) on success
    ///         or NULL on error
    pub extern "C" fn ceammc_mqtt_client_create(
        host: *const c_char,
        port: u16,
        id: *const c_char,
        user: *const c_char,
        password: *const c_char,
        cb: mqtt_cb,
    ) -> *mut mqtt_client {
        if host.is_null() {
            cb.err("invalid host string");
            return std::ptr::null_mut();
        }
        let host = unsafe { CStr::from_ptr(host) }.to_str();
        if host.is_err() {
            cb.err("invalid host string");
            return std::ptr::null_mut();
        }

        let id = if !id.is_null() {
            match unsafe { CStr::from_ptr(id) }.to_str() {
                Ok(s) => s,
                Err(_) => "",
            }
        } else {
            ""
        };

        let port = if port == 0 { 1883 } else { port };

        let user = if user.is_null() {
            ""
        } else {
            match unsafe { CStr::from_ptr(user) }.to_str() {
                Ok(s) => s,
                Err(_) => "",
            }
        };

        let password = if password.is_null() {
            ""
        } else {
            match unsafe { CStr::from_ptr(password) }.to_str() {
                Ok(s) => s,
                Err(_) => "",
            }
        };

        let cli = mqtt_client::new(id, host.unwrap(), port, user, password, 5, cb);
        Box::into_raw(Box::new(cli))
    }

    #[no_mangle]
    /// free mqtt client
    /// @param cli - mqtt client
    pub extern "C" fn ceammc_mqtt_client_free(cli: *mut mqtt_client) {
        if !cli.is_null() {
            let _ = unsafe { Box::from_raw(cli) };
        }
    }

    #[no_mangle]
    /// subscribe to mqtt topic
    /// @param cli - mqtt client
    /// @param topic - mqtt topic
    /// @return ceammc_mqtt_rc::Ok on success
    pub extern "C" fn ceammc_mqtt_client_subscribe(
        cli: *mut mqtt_client,
        topic: *const c_char,
    ) -> mqtt_rc {
        if cli.is_null() {
            return mqtt_rc::InvalidClient;
        }

        let cli = unsafe { &mut *cli };

        if topic.is_null() {
            cli.cb.err("invalid topic string");
            return mqtt_rc::InvalidString;
        }

        let topic = unsafe { CStr::from_ptr(topic) }.to_str();
        if topic.is_err() {
            cli.cb.err("invalid topic string");
            return mqtt_rc::InvalidString;
        }

        match cli.mqtt.lock() {
            Ok(mut mqtt) => match mqtt.0.try_subscribe(topic.unwrap(), QoS::AtMostOnce) {
                Ok(_) => mqtt_rc::Ok,
                Err(err) => {
                    cli.cb.err(format!("subscribe error: {err}").as_str());
                    mqtt_rc::ClientError
                }
            },
            Err(err) => {
                cli.cb.err(format!("subscribe error: {err}").as_str());
                mqtt_rc::ClientError
            }
        }
    }

    #[no_mangle]
    /// unsubscribe from mqtt topic
    /// @param cli - mqtt client
    /// @param topic - mqtt topic
    /// @return ceammc_mqtt_rc::Ok on success
    pub extern "C" fn ceammc_mqtt_client_unsubscribe(
        cli: *mut mqtt_client,
        topic: *const c_char,
    ) -> mqtt_rc {
        if cli.is_null() {
            return mqtt_rc::InvalidClient;
        }

        let cli = unsafe { &mut *cli };

        if topic.is_null() {
            cli.cb.err("invalid topic string");
            return mqtt_rc::InvalidString;
        }

        let topic = unsafe { CStr::from_ptr(topic) }.to_str();
        if topic.is_err() {
            cli.cb.err("invalid topic string");
            return mqtt_rc::InvalidString;
        }

        match cli.mqtt.lock() {
            Ok(mut mqtt) => match mqtt.0.try_unsubscribe(topic.unwrap()) {
                Ok(_) => mqtt_rc::Ok,
                Err(err) => {
                    cli.cb.err(format!("unsubscribe error: {err}").as_str());
                    mqtt_rc::ClientError
                }
            },
            Err(err) => {
                cli.cb.err(format!("unsubscribe error: {err}").as_str());
                mqtt_rc::ClientError
            }
        }
    }

    fn qos2qos(qos: mqtt_qos) -> QoS {
        match qos {
            mqtt_qos::AtLeastOnce => QoS::AtLeastOnce,
            mqtt_qos::AtMostOnce => QoS::AtMostOnce,
            mqtt_qos::ExactlyOnce => QoS::ExactlyOnce,
        }
    }

    #[no_mangle]
    /// publish text message into mqtt topic
    /// @param topic - mqtt topic ('+' single layer wildcard, '#' recursive layer wildcard)
    /// @param msg - mqtt message
    /// @param qos - Quality of Service flag
    /// @param retain - This flag tells the broker to store the message for a topic
    ///        and ensures any new client subscribing to that topic will receive the stored message.
    /// @return ceammc_mqtt_rc::Ok on success
    pub extern "C" fn ceammc_mqtt_client_publish(
        cli: *mut mqtt_client,
        topic: *const c_char,
        msg: *const c_char,
        qos: mqtt_qos,
        retain: bool,
    ) -> mqtt_rc {
        if cli.is_null() {
            return mqtt_rc::InvalidClient;
        }

        let cli = unsafe { &mut *cli };

        if topic.is_null() || msg.is_null() {
            cli.cb.err("invalid topic string");
            return mqtt_rc::InvalidString;
        }

        let topic = unsafe { CStr::from_ptr(topic) }.to_str();
        let msg = unsafe { CStr::from_ptr(msg) }.to_str();
        if topic.is_err() || msg.is_err() {
            cli.cb.err("invalid topic string");
            return mqtt_rc::InvalidString;
        }

        match cli.mqtt.lock() {
            Ok(mut mqtt) => {
                match mqtt
                    .0
                    .try_publish(topic.unwrap(), qos2qos(qos), retain, msg.unwrap())
                {
                    Ok(_) => mqtt_rc::Ok,
                    Err(err) => {
                        cli.cb.err(format!("publish error: {err}").as_str());
                        mqtt_rc::ClientError
                    }
                }
            }
            Err(err) => {
                cli.cb.err(format!("publish error: {err}").as_str());
                mqtt_rc::ClientError
            }
        }
    }

    #[no_mangle]
    /// publish binary data into mqtt topic
    /// @param topic - mqtt topic ('+' single layer wildcard, '#' recursive layer wildcard)
    /// @param data - mqtt binary data
    /// @param len - mqtt binary data length
    /// @param qos - Quality of Service flag
    /// @param retain - This flag tells the broker to store the message for a topic
    ///        and ensures any new client subscribing to that topic will receive the stored message
    /// @return ceammc_mqtt_rc::Ok on success
    pub extern "C" fn ceammc_mqtt_client_publish_data(
        cli: *mut mqtt_client,
        topic: *const c_char,
        data: *const u8,
        len: usize,
        qos: mqtt_qos,
        retain: bool,
    ) -> mqtt_rc {
        if cli.is_null() {
            return mqtt_rc::InvalidClient;
        }

        let cli = unsafe { &mut *cli };

        if topic.is_null() {
            cli.cb.err("invalid topic string");
            return mqtt_rc::InvalidString;
        }

        let topic = unsafe { CStr::from_ptr(topic) }.to_str();
        if topic.is_err() {
            cli.cb.err("invalid topic string");
            return mqtt_rc::InvalidString;
        }

        if data.is_null() {
            cli.cb.err("invalid data pointer");
            return mqtt_rc::InvalidData;
        }
        let data = unsafe { slice::from_raw_parts(data, len).to_vec() };

        // cli.mqtt.try_publish_with_properties(topic, qos, retain, payload, properties)
        match cli.mqtt.lock() {
            Ok(mut mqtt) => {
                match mqtt
                    .0
                    .try_publish(topic.unwrap(), qos2qos(qos), retain, data)
                {
                    Ok(_) => mqtt_rc::Ok,
                    Err(err) => {
                        cli.cb.err(format!("publish error: {err}").as_str());
                        mqtt_rc::ClientError
                    }
                }
            }
            Err(err) => {
                cli.cb.err(format!("publish error: {err}").as_str());
                mqtt_rc::ClientError
            }
        }
    }

    #[no_mangle]
    /// iterate mqtt events
    /// @note - this is blocking call
    /// @param cli - mqtt client pointer
    /// @param time_ms - time to blocking wait in milliseconds
    /// @return ceammc_mqtt_rc::Ok on success
    pub extern "C" fn ceammc_mqtt_process_events(cli: *mut mqtt_client, time_ms: u16) -> mqtt_rc {
        if cli.is_null() {
            return mqtt_rc::InvalidClient;
        }

        let cli = unsafe { &mut *cli };

        match cli.mqtt.lock() {
            Ok(mut mqtt) => {
                let rc = mqtt.1.recv_timeout(Duration::from_millis(time_ms.into()));
                if let Err(err) = rc {
                    return match err {
                        RecvTimeoutError::Disconnected => {
                            cli.cb.err("disconnected");
                            mqtt_rc::Disconnected
                        }
                        RecvTimeoutError::Timeout => mqtt_rc::Ok,
                    };
                }

                let rc = rc.unwrap();
                if let Err(err) = rc {
                    cli.cb.err(format!("{err}").as_str());
                    return mqtt_rc::ConnectionError;
                }

                match rc.unwrap() {
                    Event::Incoming(pack) => {
                        let _ = match pack {
                            Packet::PingResp => cli.cb.ping(),
                            Packet::Publish(p) => cli.cb.publish(&p.topic, &p.payload),
                            Packet::ConnAck(conn) => {
                                return match conn.code {
                                    ConnectReturnCode::Success => mqtt_rc::Ok,
                                    err => {
                                        cli.cb.err(format!("{err:?}").as_str());
                                        mqtt_rc::ConnectionError
                                    }
                                }
                            }
                            _ => println!("in event: {:?}", pack),
                        };
                    }
                    Event::Outgoing(ev) => {
                        println!("out event: {:?}", ev);
                    }
                };

                mqtt_rc::Ok
            }
            Err(err) => {
                cli.cb.err(format!("process events error: {err}").as_str());
                mqtt_rc::ClientError
            }
        }
    }
}
