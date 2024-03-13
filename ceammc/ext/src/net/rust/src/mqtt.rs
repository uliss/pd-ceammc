mod mqtt {

    use rumqttc::{
        Client, ConnectReturnCode, Connection, ConnectionError, Event, MqttOptions, Packet, QoS,
        RecvTimeoutError,
    };
    use std::ffi::{CStr, CString};
    use std::io::ErrorKind;
    use std::os::raw::{c_char, c_void};
    use std::slice;
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
        RefusedProtocolVersion,
        BadClientId,
        ServiceUnavailable,
        BadUserNamePassword,
        NotAuthorized,
        InvalidString,
        InvalidClient,
        ClientError,
        Disconnected,
        NetworkTimeout,
        FlushTimeout,
        ConnectionRefused,
        ConnectionReset,
        ConnectionError,
    }

    #[allow(non_camel_case_types)]
    pub struct mqtt_client {
        mqtt: Client,
        conn: Connection,
    }

    impl mqtt_client {
        fn new(id: &str, host: &str, port: u16, user: &str, pass: &str) -> Self {
            let id = if id.is_empty() { "ceammc_mqtt_pd" } else { id };
            let mut mqttoptions = MqttOptions::new(id, host, port);
            mqttoptions.set_keep_alive(Duration::from_secs(5));
            if !user.is_empty() && !pass.is_empty() {
                mqttoptions.set_credentials(user, pass);
            }
            let (client, connection) = Client::new(mqttoptions, 10);

            mqtt_client {
                mqtt: client,
                conn: connection,
            }
        }
    }

    #[no_mangle]
    /// create new mqtt client
    pub extern "C" fn ceammc_rs_mqtt_client_create(
        host: *const c_char,
        port: u16,
        id: *const c_char,
        user: *const c_char,
        password: *const c_char,
    ) -> *mut mqtt_client {
        if host.is_null() {
            return std::ptr::null_mut();
        }
        let host = unsafe { CStr::from_ptr(host) }.to_str();
        if host.is_err() {
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

        let cli = mqtt_client::new(id, host.unwrap(), port, user, password);
        Box::into_raw(Box::new(cli))
    }

    #[no_mangle]
    /// free mqtt client
    pub extern "C" fn ceammc_rs_mqtt_client_free(cli: *mut mqtt_client) {
        if !cli.is_null() {
            let asy = unsafe { Box::from_raw(cli) };
            drop(asy);
        }
    }

    #[no_mangle]
    /// subscribe to mqtt topic
    pub extern "C" fn ceammc_rs_mqtt_client_subscribe(
        cli: *mut mqtt_client,
        topic: *const c_char,
    ) -> mqtt_rc {
        if cli.is_null() {
            return mqtt_rc::InvalidClient;
        }

        let cli = unsafe { &mut *cli };

        if topic.is_null() {
            return mqtt_rc::InvalidString;
        }

        let topic = unsafe { CStr::from_ptr(topic) }.to_str();
        if topic.is_err() {
            return mqtt_rc::InvalidString;
        }

        match cli.mqtt.try_subscribe(topic.unwrap(), QoS::AtMostOnce) {
            Ok(_) => mqtt_rc::Ok,
            Err(_) => mqtt_rc::ClientError,
        }
    }

    #[no_mangle]
    /// unsubscribe from mqtt topic
    pub extern "C" fn ceammc_rs_mqtt_client_unsubscribe(
        cli: *mut mqtt_client,
        topic: *const c_char,
    ) -> mqtt_rc {
        if cli.is_null() {
            return mqtt_rc::InvalidClient;
        }

        let cli = unsafe { &mut *cli };

        if topic.is_null() {
            return mqtt_rc::InvalidString;
        }

        let topic = unsafe { CStr::from_ptr(topic) }.to_str();
        if topic.is_err() {
            return mqtt_rc::InvalidString;
        }

        match cli.mqtt.try_unsubscribe(topic.unwrap()) {
            Ok(_) => mqtt_rc::Ok,
            Err(_) => mqtt_rc::ClientError,
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
    pub extern "C" fn ceammc_rs_mqtt_client_publish(
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
            return mqtt_rc::InvalidString;
        }

        let topic = unsafe { CStr::from_ptr(topic) }.to_str();
        let msg = unsafe { CStr::from_ptr(msg) }.to_str();
        if topic.is_err() || msg.is_err() {
            return mqtt_rc::InvalidString;
        }

        match cli
            .mqtt
            .try_publish(topic.unwrap(), qos2qos(qos), retain, msg.unwrap())
        {
            Ok(_) => mqtt_rc::Ok,
            Err(_) => mqtt_rc::ClientError,
        }
    }

    #[no_mangle]
    /// publish data into mqtt topic
    /// @param topic - mqtt topic ('+' single layer wildcard, '#' recursive layer wildcard)
    /// @param data - mqtt message data
    /// @param len - mqtt message data length
    /// @param qos - Quality of Service flag
    /// @param retain - This flag tells the broker to store the message for a topic
    ///        and ensures any new client subscribing to that topic will receive the stored message.
    pub extern "C" fn ceammc_rs_mqtt_client_publish_data(
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
            return mqtt_rc::InvalidString;
        }

        let topic = unsafe { CStr::from_ptr(topic) }.to_str();
        if topic.is_err() {
            return mqtt_rc::InvalidString;
        }

        if data.is_null() {
            return mqtt_rc::InvalidString;
        }
        let data = unsafe { slice::from_raw_parts(data, len).to_vec() };

        // cli.mqtt.try_publish_with_properties(topic, qos, retain, payload, properties)

        match cli
            .mqtt
            .try_publish(topic.unwrap(), qos2qos(qos), retain, data)
        {
            Ok(_) => mqtt_rc::Ok,
            Err(_) => mqtt_rc::ClientError,
        }
    }

    #[no_mangle]
    /// iterate mqtt events
    /// @param cli - mqtt client pointer
    /// @param time_ms - time to blocking wait in milliseconds
    /// @param cb_data - pointer to user data for callbacks
    /// @param cb_ping - ping callback (user data)
    /// @param cb_pub - publish callback (user data, topic, message data, message size)
    /// @param cb_conn - connection callback (user_data, result code)
    pub extern "C" fn ceammc_rs_mqtt_runloop(
        cli: *mut mqtt_client,
        time_ms: u16,
        cb_data: *mut c_void,
        cb_ping: Option<extern "C" fn(*mut c_void)>,
        cb_pub: Option<extern "C" fn(*mut c_void, *const c_char, *const u8, usize)>,
        cb_conn: Option<extern "C" fn(*mut c_void, code: mqtt_rc)>,
    ) -> mqtt_rc {
        if cli.is_null() {
            return mqtt_rc::InvalidClient;
        }

        let cli = unsafe { &mut *cli };

        let rc = cli.conn.recv_timeout(Duration::from_millis(time_ms.into()));
        if rc.is_err() {
            return match rc.unwrap_err() {
                RecvTimeoutError::Disconnected => mqtt_rc::Disconnected,
                RecvTimeoutError::Timeout => mqtt_rc::Ok,
            };
        }
        let rc = rc.unwrap();
        if rc.is_err() {
            return match rc.unwrap_err() {
                ConnectionError::NetworkTimeout => mqtt_rc::NetworkTimeout,
                ConnectionError::FlushTimeout => mqtt_rc::FlushTimeout,
                ConnectionError::Io(err) => match err.kind() {
                    ErrorKind::ConnectionRefused => mqtt_rc::ConnectionRefused,
                    ErrorKind::ConnectionReset => mqtt_rc::ConnectionReset,
                    _ => mqtt_rc::ConnectionError,
                },
                _ => mqtt_rc::ConnectionError,
            };
        }

        match rc.unwrap() {
            Event::Incoming(pack) => {
                let _ = match pack {
                    Packet::PingResp => {
                        if let Some(f) = cb_ping {
                            f(cb_data);
                        }
                    }
                    Packet::Publish(p) => {
                        if let Some(f) = cb_pub {
                            let topic = CString::new(p.topic).unwrap();
                            let data = p.payload;
                            f(cb_data, topic.as_ptr(), data.as_ptr(), data.len());
                        }
                    }
                    Packet::ConnAck(conn) => {
                        if let Some(f) = cb_conn {
                            f(
                                cb_data,
                                match conn.code {
                                    ConnectReturnCode::RefusedProtocolVersion => {
                                        mqtt_rc::RefusedProtocolVersion
                                    }
                                    ConnectReturnCode::BadClientId => mqtt_rc::BadClientId,
                                    ConnectReturnCode::ServiceUnavailable => {
                                        mqtt_rc::ServiceUnavailable
                                    }
                                    ConnectReturnCode::BadUserNamePassword => {
                                        mqtt_rc::BadUserNamePassword
                                    }
                                    ConnectReturnCode::NotAuthorized => mqtt_rc::NotAuthorized,
                                    ConnectReturnCode::Success => mqtt_rc::Ok,
                                },
                            );
                        }
                    }
                    _ => println!("in event: {:?}", pack),
                };
                true
            }
            Event::Outgoing(ev) => {
                println!("out event: {:?}", ev);
                true
            }
        };

        mqtt_rc::Ok
    }
}
