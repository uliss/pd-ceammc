use bytes::Bytes;
use rumqttc::{ConnectionError, Event, MqttOptions, Packet, QoS};
use std::ffi::{CStr, CString};
use std::os::raw::{c_char, c_void};
use std::ptr::null_mut;
use std::slice;
use std::time::Duration;

use crate::service::*;

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

#[allow(non_camel_case_types)]
#[repr(C)]
pub enum mqtt_rc {
    Success = 0,
    RefusedProtocolVersion,
    BadClientId,
    ServiceUnavailable,
    BadUserNamePassword,
    NotAuthorized,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct mqtt_client_result_cb {
    /// user data pointer (can be NULL)
    user: *mut c_void,
    /// ping callback function (can be NULL)
    ping_cb: Option<extern "C" fn(user: *mut c_void)>,
    /// connected callback function (can be NULL)
    conn_cb: Option<extern "C" fn(user: *mut c_void, code: mqtt_rc)>,
    /// publish callback function (can be NULL)
    pub_cb: Option<
        extern "C" fn(
            user: *mut c_void,
            topic: *const c_char,
            data: *const u8,
            data_len: usize,
            qos: mqtt_qos,
            retain: bool,
            pkid: u16,
        ),
    >,
}

impl ServiceCallback<MqttReply> for mqtt_client_result_cb {
    fn exec(&self, data: &MqttReply) {
        match data {
            MqttReply::Connected(x) => {
                use rumqttc::ConnectReturnCode::*;
                self.conn_cb.map(|f| {
                    f(
                        self.user,
                        match x.code {
                            Success => mqtt_rc::Success,
                            RefusedProtocolVersion => mqtt_rc::RefusedProtocolVersion,
                            BadClientId => mqtt_rc::BadClientId,
                            ServiceUnavailable => mqtt_rc::ServiceUnavailable,
                            BadUserNamePassword => mqtt_rc::BadUserNamePassword,
                            NotAuthorized => mqtt_rc::NotAuthorized,
                        },
                    )
                });
            }
            MqttReply::Publish(p) => {
                let topic = CString::new(p.topic.clone()).unwrap_or_default();
                self.pub_cb.map(|f| {
                    f(
                        self.user,
                        topic.as_ptr(),
                        p.payload.as_ptr(),
                        p.payload.len(),
                        match p.qos {
                            QoS::AtMostOnce => mqtt_qos::AtMostOnce,
                            QoS::AtLeastOnce => mqtt_qos::AtLeastOnce,
                            QoS::ExactlyOnce => mqtt_qos::ExactlyOnce,
                        },
                        p.retain,
                        p.pkid,
                    )
                });
            }
            MqttReply::Ping => {
                self.ping_cb.map(|f| f(self.user));
            }
        }
    }
}

#[derive(Debug)]
struct PublishRequest {
    topic: String,
    msg: String,
    qos: QoS,
    retain: bool,
}

#[derive(Debug)]
struct PublishDataRequest {
    topic: String,
    data: Bytes,
    qos: QoS,
    retain: bool,
}

#[derive(Debug)]
enum MqttRequest {
    Subscribe(String, QoS),
    Unsubscribe(String),
    Publish(PublishRequest),
    PublishData(PublishDataRequest),
}

#[derive(Debug)]
enum MqttReply {
    Connected(rumqttc::ConnAck),
    Publish(rumqttc::Publish),
    Ping,
}

type MqttService = Service<MqttRequest, MqttReply>;

#[allow(non_camel_case_types)]
pub struct mqtt_client {
    service: MqttService,
    quit_tx: tokio::sync::mpsc::Sender<bool>,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct mqtt_client_init {
    url: *const c_char,
    id: *const c_char,
}

async fn match_event(
    event: Result<Event, ConnectionError>,
    rep_tx: &tokio::sync::mpsc::Sender<Result<MqttReply, Error>>,
    cb_notify: callback_notify,
) -> bool {
    match event {
        Ok(ev) => {
            match ev {
                rumqttc::Event::Incoming(packet) => {
                    match packet {
                        // Packet::Connect(x) => todo!(),
                        Packet::ConnAck(x) => {
                            if let Err(err) = rep_tx.send(Ok(MqttReply::Connected(x))).await {
                                eprintln!("<- ConnAck: send error: {err:?}");
                            }
                            cb_notify.exec();
                        }
                        Packet::Publish(x) => {
                            if let Err(err) = rep_tx.send(Ok(MqttReply::Publish(x))).await {
                                eprintln!("<- Publish send error: {err:?}");
                            }
                            cb_notify.exec();
                        }
                        Packet::PingResp => {
                            if let Err(err) = rep_tx.send(Ok(MqttReply::Ping)).await {
                                eprintln!("<- PingResp send error: {err:?}");
                            }
                            cb_notify.exec();
                        }
                        _ => {}
                    }
                }
                _ => {}
            }
        }
        Err(err) => {
            if let Err(err) = rep_tx
                .send(Err(Error::Error(format!("connection error: {err:?}"))))
                .await
            {
                eprintln!("### send error: {err}");
            }
            cb_notify.exec();
            return false;
        }
    }

    true
}

#[no_mangle]
/// create new mqtt client
/// @param url - mqtt broker url in format mqtt://host:port
/// @param id - client id (can be NULL)
/// @param cb - callbacks
/// @return pointer to mqtt_client (must be freed by ceammc_mqtt_client_free()) on success
///         or NULL on error
pub extern "C" fn ceammc_mqtt_client_new(
    params: mqtt_client_init,
    cb_err: callback_msg,
    cb_post: callback_msg,
    cb_debug: callback_msg,
    cb_log: callback_msg,
    _cb_progress: callback_progress,
    cb_reply: mqtt_client_result_cb,
    cb_notify: callback_notify,
) -> *mut mqtt_client {
    if params.url.is_null() {
        cb_err.exec("invalid URL string");
        return std::ptr::null_mut();
    }
    let url = unsafe { CStr::from_ptr(params.url) }.to_str();
    if url.is_err() {
        cb_err.exec("invalid URL string");
        return std::ptr::null_mut();
    }

    let id = if !params.id.is_null() {
        match unsafe { CStr::from_ptr(params.id) }.to_str() {
            Ok(s) => s,
            Err(_) => "ceammc_mqtt_pd",
        }
    } else {
        "ceammc_mqtt_pd"
    };

    let url = url.unwrap().to_owned() + "?client_id=" + id;
    match MqttOptions::parse_url(url) {
        Err(err) => {
            cb_err.exec(format!("url error: {err}").as_str());
            return std::ptr::null_mut();
        }

        Ok(mut opts) => {
            if opts.broker_address().0.is_empty() {
                cb_err.exec("empty hostname");
                return std::ptr::null_mut();
            }

            opts.set_keep_alive(Duration::from_secs(5));
            // println!("connect with options {opts:?}");

            let rt = tokio::runtime::Runtime::new();
            let (cli, mut event_loop) = rumqttc::AsyncClient::new(opts, 10);

            match rt {
                Ok(rt) => {
                    let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<MqttRequest>(32);
                    let (rep_tx, rep_rx) =
                        tokio::sync::mpsc::channel::<Result<MqttReply, Error>>(32);

                    let (quit_tx, mut quit_rx) = tokio::sync::mpsc::channel::<bool>(1);

                    std::thread::spawn(move || {
                        rt.block_on(async move {
                            let w_tx = rep_tx.clone();

                            // worker thread
                            tokio::task::spawn(async move {
                                while let Some(task) = req_rx.recv().await {
                                    match task {
                                        MqttRequest::Subscribe(topic, qos) => {
                                            if let Err(err) = cli.subscribe(topic, qos).await {
                                                let _ = w_tx
                                                    .send(Err(Error::Error(format!("{err:?}"))))
                                                    .await;
                                                cb_notify.exec();
                                            }
                                        }
                                        MqttRequest::Unsubscribe(topic) => {
                                            if let Err(err) = cli.unsubscribe(topic).await {
                                                let _ = w_tx
                                                    .send(Err(Error::Error(format!("{err:?}"))))
                                                    .await;
                                                cb_notify.exec();
                                            }
                                        }
                                        MqttRequest::Publish(msg) => {
                                            if let Err(err) = cli
                                                .publish(msg.topic, msg.qos, msg.retain, msg.msg)
                                                .await
                                            {
                                                let _ = w_tx
                                                    .send(Err(Error::Error(format!("{err:?}"))))
                                                    .await;
                                                cb_notify.exec();
                                            }
                                        }
                                        MqttRequest::PublishData(data) => {
                                            if let Err(err) = cli
                                                .publish(
                                                    data.topic,
                                                    data.qos,
                                                    data.retain,
                                                    data.data,
                                                )
                                                .await
                                            {
                                                let _ = w_tx
                                                    .send(Err(Error::Error(format!("{err:?}"))))
                                                    .await;
                                                cb_notify.exec();
                                            }
                                        }
                                    }
                                }

                                // Once all senders have gone out of scope,
                                // the `.recv()` call returns None and it will
                                // exit from the while loop and shut down the
                                // thread.
                            });

                            loop {
                                tokio::select! {
                                    _val = quit_rx.recv() => {
                                        return ();
                                    },
                                    ev = event_loop.poll() => {
                                       let _ = match_event(ev, &rep_tx, cb_notify).await;
                                    }
                                }
                            }
                        });
                    });

                    let srv = Service::<MqttRequest, MqttReply>::new(
                        cb_err,
                        cb_post,
                        cb_debug,
                        cb_log,
                        callback_progress::default(),
                        Box::new(cb_reply),
                        req_tx,
                        rep_rx,
                    );

                    return Box::into_raw(Box::new(mqtt_client {
                        service: srv,
                        quit_tx,
                    }));
                }
                Err(err) => {
                    cb_err.exec(format!("tokio runtime creation error: {err}").as_str());
                    return null_mut();
                }
            }
        }
    }
}

#[no_mangle]
/// free mqtt client
/// @param cli - mqtt client (can be NULL)
pub extern "C" fn ceammc_mqtt_client_free(cli: *mut mqtt_client) {
    if !cli.is_null() {
        let cli = unsafe { Box::from_raw(cli) };
        let _ = cli.quit_tx.blocking_send(true);
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
/// subscribe to mqtt topic
/// @param cli - mqtt client
/// @param topic - mqtt topic
/// @return true on success
pub extern "C" fn ceammc_mqtt_client_subscribe(
    cli: *mut mqtt_client,
    topic: *const c_char,
    qos: mqtt_qos,
) -> bool {
    if cli.is_null() {
        return false;
    }

    let cli = unsafe { &mut *cli };

    if topic.is_null() {
        cli.service.on_error("NULL topic string");
        return false;
    }

    let topic = unsafe { CStr::from_ptr(topic) }
        .to_str()
        .map(|x| x.to_owned());
    match topic {
        Ok(topic) => {
            cli.service
                .send_request(MqttRequest::Subscribe(topic, qos2qos(qos)));
            true
        }
        Err(err) => {
            cli.service
                .on_error(format!("invalid topic string: {err}").as_str());
            false
        }
    }
}

#[no_mangle]
/// unsubscribe from mqtt topic
/// @param cli - mqtt client
/// @param topic - mqtt topic
/// @return true on success
pub extern "C" fn ceammc_mqtt_client_unsubscribe(
    cli: *mut mqtt_client,
    topic: *const c_char,
) -> bool {
    if cli.is_null() {
        return false;
    }

    let cli = unsafe { &mut *cli };

    if topic.is_null() {
        cli.service.on_error("NULL topic string");
        return false;
    }

    let topic = unsafe { CStr::from_ptr(topic) }
        .to_str()
        .map(|x| x.to_owned());
    match topic {
        Ok(topic) => {
            cli.service.send_request(MqttRequest::Unsubscribe(topic));
            true
        }
        Err(err) => {
            cli.service
                .on_error(format!("invalid topic string: {err}").as_str());
            true
        }
    }
}

#[no_mangle]
/// publish text message into mqtt topic
/// @param topic - mqtt topic ('+' single layer wildcard, '#' recursive layer wildcard)
/// @param msg - mqtt message
/// @param qos - Quality of Service flag
/// @param retain - This flag tells the broker to store the message for a topic
///        and ensures any new client subscribing to that topic will receive the stored message.
/// @return true on success
pub extern "C" fn ceammc_mqtt_client_publish(
    cli: *mut mqtt_client,
    topic: *const c_char,
    msg: *const c_char,
    qos: mqtt_qos,
    retain: bool,
) -> bool {
    if cli.is_null() {
        return false;
    }

    let cli = unsafe { &mut *cli };

    if topic.is_null() || msg.is_null() {
        cli.service.on_error("invalid topic string");
        return false;
    }

    let topic = unsafe { CStr::from_ptr(topic) }
        .to_str()
        .map(|x| x.to_owned());
    let msg = unsafe { CStr::from_ptr(msg) }
        .to_str()
        .map(|x| x.to_owned());
    if topic.is_err() || msg.is_err() {
        cli.service.on_error("invalid topic string");
        return false;
    }

    return cli
        .service
        .send_request(MqttRequest::Publish(PublishRequest {
            topic: topic.unwrap(),
            msg: msg.unwrap(),
            qos: qos2qos(qos),
            retain,
        }));
}

#[no_mangle]
/// publish binary data into mqtt topic
/// @param topic - mqtt topic ('+' single layer wildcard, '#' recursive layer wildcard)
/// @param data - mqtt binary data
/// @param len - mqtt binary data length
/// @param qos - Quality of Service flag
/// @param retain - This flag tells the broker to store the message for a topic
///        and ensures any new client subscribing to that topic will receive the stored message
/// @return true on success
pub extern "C" fn ceammc_mqtt_client_publish_data(
    cli: *mut mqtt_client,
    topic: *const c_char,
    data: *const u8,
    len: usize,
    qos: mqtt_qos,
    retain: bool,
) -> bool {
    if cli.is_null() {
        return false;
    }

    let cli = unsafe { &mut *cli };

    if topic.is_null() {
        cli.service.on_error("invalid topic string");
        return false;
    }

    let topic = unsafe { CStr::from_ptr(topic) }
        .to_str()
        .map(|x| x.to_owned());
    if topic.is_err() {
        cli.service.on_error("invalid topic string");
        return false;
    }

    if data.is_null() {
        cli.service.on_error("invalid data pointer");
        return false;
    }
    let data = Bytes::copy_from_slice(unsafe { slice::from_raw_parts(data, len) });

    return cli
        .service
        .send_request(MqttRequest::PublishData(PublishDataRequest {
            topic: topic.unwrap(),
            data,
            qos: qos2qos(qos),
            retain,
        }));
}

#[no_mangle]
/// iterate mqtt events
/// @param cli - mqtt client pointer
/// @return true on success
pub extern "C" fn ceammc_mqtt_client_process(cli: *mut mqtt_client) -> bool {
    if cli.is_null() {
        false
    } else {
        let cli = unsafe { &mut *cli };
        cli.service.process_results();
        true
    }
}
