use std::{
    ffi::{c_char, c_void, CStr, CString},
    ptr::null_mut,
};

use ::esphome_client::{
    types::{
        EspHomeMessage, ListEntitiesRequest, PingRequest, SubscribeStatesRequest,
        SwitchCommandRequest, TextCommandRequest,
    },
    EspHomeClient,
};
use ceammc_rs_msg::{cstr_from_string, msg_cb, msg_notify, SendState};
use log::{debug, error};
use tokio::task::JoinHandle;
use tokio_util::sync::CancellationToken;

pub const ESPHOME_DEFAULT_PORT: u16 = 6053;

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_entity_id {
    id: u32,
    device_id: u32,
}

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_switch_state {
    value: bool,
}

#[repr(C)]
pub struct esphome_text_state {
    value: *const c_char,
    missing_state: bool,
}

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_switch_info {
    /// valid within callback only
    name: *const c_char,
    /// valid within callback only
    icon: *const c_char,
    /// valid within callback only
    object_id: *const c_char,
    /// valid within callback only
    device_class: *const c_char,
    id: esphome_entity_id,
    entity_category: i32,
    assumed_state: bool,
    disabled_by_default: bool,
}

#[derive(Debug, Clone)]
struct TextState {
    value: CString,
    missing_state: bool,
}

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_text_info {
    /// valid within callback only
    object_id: *const c_char,
    /// valid within callback only
    name: *const c_char,
    /// valid within callback only
    icon: *const c_char,
    /// valid within callback only
    pattern: *const c_char,
    id: esphome_entity_id,
    entity_category: i32,
    min_length: u32,
    max_length: u32,
    mode: i32,
    disabled_by_default: bool,
}

#[derive(Debug)]
enum Request {
    Ping,
    SubscribeStates,
    ListEntities,
    Switch(u32, bool),
    Text(u32, String),
}

#[derive(Clone, Debug)]
enum Reply {
    Pong,
    SwitchState(esphome_entity_id, esphome_switch_state),
    TextState(esphome_entity_id, TextState),
    ListEntitiesEnd,
    BinaryState {
        key: u32,
        state: bool,
        device_id: u32,
        missing_state: bool,
    },
    SwitchInfo {
        id: esphome_entity_id,
        name: CString,
        icon: CString,
        object_id: CString,
        device_class: CString,
        entity_category: i32,
        assumed_state: bool,
        disabled_by_default: bool,
    },
    TextInfo {
        id: esphome_entity_id,
        object_id: CString,
        name: CString,
        icon: CString,
        pattern: CString,
        disabled_by_default: bool,
        entity_category: i32,
        min_length: u32,
        max_length: u32,
        mode: i32,
    },
}

async fn process_message_from_device(
    msg: Result<EspHomeMessage, ::esphome_client::error::ClientError>,
) -> Result<Option<Reply>, String> {
    let msg = msg.map_err(|err| err.to_string())?;

    match msg {
        // EspHomeMessage::HelloResponse(hello_response) => todo!(),
        // EspHomeMessage::AuthenticationResponse(authentication_response) => todo!(),
        // EspHomeMessage::DisconnectResponse(disconnect_response) => todo!(),
        EspHomeMessage::PingResponse(_) => Ok(Some(Reply::Pong)),
        EspHomeMessage::DeviceInfoResponse(device_info_response) => todo!(),
        EspHomeMessage::ListEntitiesDoneResponse(_) => Ok(Some(Reply::ListEntitiesEnd)),
        EspHomeMessage::ListEntitiesBinarySensorResponse(list_entities_binary_sensor_response) => {
            Ok(None)
        }
        EspHomeMessage::BinarySensorStateResponse(x) => Ok(Some(Reply::BinaryState {
            key: x.key,
            state: x.state,
            device_id: x.device_id,
            missing_state: x.missing_state,
        })),
        EspHomeMessage::ListEntitiesCoverResponse(list_entities_cover_response) => todo!(),
        EspHomeMessage::CoverStateResponse(cover_state_response) => todo!(),
        EspHomeMessage::ListEntitiesFanResponse(list_entities_fan_response) => todo!(),
        EspHomeMessage::FanStateResponse(fan_state_response) => todo!(),
        EspHomeMessage::ListEntitiesLightResponse(light) => {
            debug!("{light:?}");
            Ok(None)
        }
        EspHomeMessage::LightStateResponse(light_state_response) => Ok(None),
        EspHomeMessage::ListEntitiesSensorResponse(list_entities_sensor_response) => Ok(None),
        EspHomeMessage::SensorStateResponse(sensor_state_response) => todo!(),
        EspHomeMessage::ListEntitiesSwitchResponse(sw) => Ok(Some(Reply::SwitchInfo {
            id: esphome_entity_id {
                id: sw.key,
                device_id: sw.device_id,
            },
            name: cstr_from_string(sw.name),
            icon: cstr_from_string(sw.icon),
            object_id: cstr_from_string(sw.object_id),
            device_class: cstr_from_string(sw.device_class),
            entity_category: sw.entity_category,
            assumed_state: sw.assumed_state,
            disabled_by_default: sw.disabled_by_default,
        })),
        EspHomeMessage::SwitchStateResponse(x) => Ok(Some(Reply::SwitchState(
            esphome_entity_id {
                id: x.key,
                device_id: x.device_id,
            },
            esphome_switch_state { value: x.state },
        ))),
        EspHomeMessage::ListEntitiesTextSensorResponse(txt) => {
            log::debug!("{txt:?}");
            Ok(None)
        }
        EspHomeMessage::TextSensorStateResponse(txt) => Ok(Some(Reply::TextState(
            esphome_entity_id {
                id: txt.key,
                device_id: txt.device_id,
            },
            TextState {
                value: cstr_from_string(txt.state),
                missing_state: txt.missing_state,
            },
        ))),
        EspHomeMessage::SubscribeLogsResponse(subscribe_logs_response) => todo!(),
        EspHomeMessage::NoiseEncryptionSetKeyResponse(noise_encryption_set_key_response) => todo!(),
        EspHomeMessage::HomeassistantActionResponse(homeassistant_action_response) => todo!(),
        EspHomeMessage::SubscribeHomeAssistantStateResponse(
            subscribe_home_assistant_state_response,
        ) => todo!(),
        EspHomeMessage::HomeAssistantStateResponse(home_assistant_state_response) => todo!(),
        EspHomeMessage::GetTimeResponse(get_time_response) => todo!(),
        EspHomeMessage::ListEntitiesServicesResponse(list_entities_services_response) => todo!(),
        EspHomeMessage::ListEntitiesCameraResponse(list_entities_camera_response) => todo!(),
        EspHomeMessage::CameraImageResponse(camera_image_response) => todo!(),
        EspHomeMessage::ListEntitiesClimateResponse(list_entities_climate_response) => todo!(),
        EspHomeMessage::ClimateStateResponse(climate_state_response) => todo!(),
        EspHomeMessage::ListEntitiesNumberResponse(list_entities_number_response) => todo!(),
        EspHomeMessage::NumberStateResponse(number_state_response) => todo!(),
        EspHomeMessage::ListEntitiesSelectResponse(list_entities_select_response) => todo!(),
        EspHomeMessage::SelectStateResponse(select_state_response) => todo!(),
        EspHomeMessage::ListEntitiesSirenResponse(list_entities_siren_response) => todo!(),
        EspHomeMessage::SirenStateResponse(siren_state_response) => todo!(),
        EspHomeMessage::ListEntitiesLockResponse(list_entities_lock_response) => todo!(),
        EspHomeMessage::LockStateResponse(lock_state_response) => todo!(),
        EspHomeMessage::ListEntitiesButtonResponse(list_entities_button_response) => todo!(),
        EspHomeMessage::ListEntitiesMediaPlayerResponse(list_entities_media_player_response) => {
            todo!()
        }
        EspHomeMessage::MediaPlayerStateResponse(media_player_state_response) => todo!(),
        EspHomeMessage::BluetoothLeAdvertisementResponse(bluetooth_le_advertisement_response) => {
            todo!()
        }
        EspHomeMessage::BluetoothLeRawAdvertisementsResponse(
            bluetooth_le_raw_advertisements_response,
        ) => todo!(),
        EspHomeMessage::BluetoothDeviceConnectionResponse(bluetooth_device_connection_response) => {
            todo!()
        }
        EspHomeMessage::BluetoothGattGetServicesResponse(bluetooth_gatt_get_services_response) => {
            todo!()
        }
        EspHomeMessage::BluetoothGattGetServicesDoneResponse(
            bluetooth_gatt_get_services_done_response,
        ) => todo!(),
        EspHomeMessage::BluetoothGattReadResponse(bluetooth_gatt_read_response) => todo!(),
        EspHomeMessage::BluetoothGattNotifyDataResponse(bluetooth_gatt_notify_data_response) => {
            todo!()
        }
        EspHomeMessage::BluetoothConnectionsFreeResponse(bluetooth_connections_free_response) => {
            todo!()
        }
        EspHomeMessage::BluetoothGattErrorResponse(bluetooth_gatt_error_response) => todo!(),
        EspHomeMessage::BluetoothGattWriteResponse(bluetooth_gatt_write_response) => todo!(),
        EspHomeMessage::BluetoothGattNotifyResponse(bluetooth_gatt_notify_response) => todo!(),
        EspHomeMessage::BluetoothDevicePairingResponse(bluetooth_device_pairing_response) => {
            todo!()
        }
        EspHomeMessage::BluetoothDeviceUnpairingResponse(bluetooth_device_unpairing_response) => {
            todo!()
        }
        EspHomeMessage::BluetoothDeviceClearCacheResponse(
            bluetooth_device_clear_cache_response,
        ) => todo!(),
        EspHomeMessage::BluetoothScannerStateResponse(bluetooth_scanner_state_response) => todo!(),
        EspHomeMessage::VoiceAssistantResponse(voice_assistant_response) => todo!(),
        EspHomeMessage::VoiceAssistantEventResponse(voice_assistant_event_response) => todo!(),
        EspHomeMessage::VoiceAssistantAudio(voice_assistant_audio) => todo!(),
        EspHomeMessage::VoiceAssistantTimerEventResponse(voice_assistant_timer_event_response) => {
            todo!()
        }
        EspHomeMessage::VoiceAssistantAnnounceFinished(voice_assistant_announce_finished) => {
            todo!()
        }
        EspHomeMessage::VoiceAssistantConfigurationResponse(
            voice_assistant_configuration_response,
        ) => todo!(),
        EspHomeMessage::VoiceAssistantSetConfiguration(voice_assistant_set_configuration) => {
            todo!()
        }
        EspHomeMessage::ListEntitiesAlarmControlPanelResponse(
            list_entities_alarm_control_panel_response,
        ) => todo!(),
        EspHomeMessage::AlarmControlPanelStateResponse(alarm_control_panel_state_response) => {
            todo!()
        }
        EspHomeMessage::ListEntitiesTextResponse(txt) => Ok(Some(Reply::TextInfo {
            id: esphome_entity_id {
                id: txt.key,
                device_id: txt.device_id,
            },
            object_id: cstr_from_string(txt.object_id),
            name: cstr_from_string(txt.name),
            icon: cstr_from_string(txt.icon),
            disabled_by_default: txt.disabled_by_default,
            entity_category: txt.entity_category,
            min_length: txt.min_length,
            max_length: txt.max_length,
            pattern: cstr_from_string(txt.pattern),
            mode: txt.mode,
        })),
        EspHomeMessage::TextStateResponse(txt) => {
            debug!("{txt:?}");
            Ok(None)
        }
        EspHomeMessage::ListEntitiesDateResponse(list_entities_date_response) => todo!(),
        EspHomeMessage::DateStateResponse(date_state_response) => todo!(),
        EspHomeMessage::ListEntitiesTimeResponse(list_entities_time_response) => todo!(),
        EspHomeMessage::TimeStateResponse(time_state_response) => todo!(),
        EspHomeMessage::ListEntitiesEventResponse(list_entities_event_response) => todo!(),
        EspHomeMessage::EventResponse(event_response) => todo!(),
        EspHomeMessage::ListEntitiesValveResponse(list_entities_valve_response) => todo!(),
        EspHomeMessage::ValveStateResponse(valve_state_response) => todo!(),
        EspHomeMessage::ListEntitiesDateTimeResponse(list_entities_date_time_response) => todo!(),
        EspHomeMessage::DateTimeStateResponse(date_time_state_response) => todo!(),
        EspHomeMessage::ListEntitiesUpdateResponse(list_entities_update_response) => todo!(),
        EspHomeMessage::UpdateStateResponse(update_state_response) => todo!(),
        _ => Ok(None),
    }
}

#[repr(C)]
pub struct esphome_client_cb {
    user: *mut c_void,
    on_pong: extern "C" fn(user: *mut c_void),
    on_switch_state:
        extern "C" fn(user: *mut c_void, key: esphome_entity_id, state: esphome_switch_state),
    on_binary: extern "C" fn(
        user: *mut c_void,
        key: u32,
        state: bool,
        device_id: u32,
        missing_state: bool,
    ),
    on_text: extern "C" fn(user: *mut c_void, key: esphome_entity_id, state: esphome_text_state),
    on_info_switch: extern "C" fn(user: *mut c_void, info: &esphome_switch_info),
    on_info_text: extern "C" fn(user: *mut c_void, info: &esphome_text_info),
}

impl esphome_client_cb {
    fn pong(&self) {
        (self.on_pong)(self.user)
    }

    fn state_switch(&self, key: esphome_entity_id, state: esphome_switch_state) {
        (self.on_switch_state)(self.user, key, state)
    }

    fn text(&self, key: esphome_entity_id, state: TextState) {
        let state = esphome_text_state {
            value: state.value.as_ptr(),
            missing_state: state.missing_state,
        };
        (self.on_text)(self.user, key, state)
    }

    fn binary(&self, key: u32, state: bool, device_id: u32, missing_state: bool) {
        (self.on_binary)(self.user, key, state, device_id, missing_state)
    }

    fn info_switch(&self, info: esphome_switch_info) {
        (self.on_info_switch)(self.user, &info)
    }

    fn info_text(&self, info: esphome_text_info) {
        (self.on_info_text)(self.user, &info)
    }
}

#[allow(non_camel_case_types)]
pub struct esphome_client {
    obj: ceammc_rs_msg::TokioClient<Request, Reply>,
    on_msg: msg_cb,
    on_data: esphome_client_cb,
}

impl esphome_client {
    fn process_reply_ptr(cli: *mut Self) -> bool {
        if cli.is_null() {
            false
        } else {
            let cli = unsafe { &mut *cli };
            cli.obj.recv_loop(|reply| match reply {
                Reply::Pong => cli.on_data.pong(),
                Reply::SwitchState(key, state) => cli.on_data.state_switch(key, state),
                Reply::BinaryState {
                    key,
                    state,
                    device_id,
                    missing_state,
                } => cli.on_data.binary(key, state, device_id, missing_state),
                Reply::TextState(key, state) => cli.on_data.text(key, state),
                Reply::SwitchInfo {
                    id,
                    name,
                    icon,
                    object_id,
                    device_class,
                    entity_category,
                    assumed_state,
                    disabled_by_default,
                } => cli.on_data.info_switch(esphome_switch_info {
                    id,
                    name: name.as_ptr(),
                    icon: icon.as_ptr(),
                    object_id: object_id.as_ptr(),
                    device_class: device_class.as_ptr(),
                    entity_category,
                    assumed_state,
                    disabled_by_default,
                }),
                Reply::TextInfo {
                    id,
                    object_id,
                    name,
                    icon,
                    disabled_by_default,
                    entity_category,
                    min_length,
                    max_length,
                    pattern,
                    mode,
                } => cli.on_data.info_text(esphome_text_info {
                    id,
                    name: name.as_ptr(),
                    icon: icon.as_ptr(),
                    object_id: object_id.as_ptr(),
                    entity_category,
                    disabled_by_default,
                    pattern: pattern.as_ptr(),
                    min_length,
                    max_length,
                    mode,
                }),
                Reply::ListEntitiesEnd => {}
            });
            true
        }
    }

    fn send_request(cli: *mut Self, req: Request) -> bool {
        if cli.is_null() {
            false
        } else {
            let cli = unsafe { &*cli };
            match cli.obj.send(req) {
                SendState::NoSpace => {
                    cli.on_msg.error_str("no space left in worker channel");
                }
                SendState::Disconnected => cli.on_msg.error_str("worker is disconnected"),
                SendState::Ok => {}
            }
            true
        }
    }

    fn new(
        addr: &CStr,
        port: u16,
        notify: msg_notify,
        on_msg: msg_cb,
        on_data: esphome_client_cb,
    ) -> Self {
        let addr = addr.to_string_lossy().to_string();
        let obj = ceammc_rs_msg::TokioClient::<Request, Reply>::start_worker(
            None,
            async move |mut channel| {
                let addr = format!("{addr}:{port}");
                debug!("connecting to {addr} ...");

                let mut client = EspHomeClient::builder()
                    .address(&addr)
                    // .key(KEY)
                    .connect()
                    .await
                    .map_err(|err| err.to_string())?;

                debug!("connected ...");

                let cancel_dev1 = CancellationToken::new();
                let cancel_dev2 = cancel_dev1.clone();
                let (dev_tx, mut dev_rx) = tokio::sync::mpsc::channel(24);

                let to_client = channel.clone_sender();

                let dev_task: JoinHandle<Result<(), String>> = tokio::spawn(async move {
                    debug!("esphome device thread start");
                    loop {
                        tokio::select! {
                            msg = client.try_read() => {
                                if let Some(reply) = process_message_from_device(msg).await? {
                                    debug!("{reply:?}");
                                    to_client.send_data(reply).to_worker_result()?;
                                    debug!("done");
                                }
                            },
                            Some(msg) = dev_rx.recv() => {
                                client.try_write(msg).await.map_err(|err| err.to_string())?;
                            },
                            _ = cancel_dev1.cancelled() => break,
                        }
                    }
                    debug!("esphome device thread done");
                    Ok(())
                });

                if let Err(err) = channel
                    .recv_loop(&mut async |req| {
                        debug!("{req:?}");
                        match req {
                            Request::Ping => {
                                dev_tx
                                    .send(EspHomeMessage::PingRequest(PingRequest {}))
                                    .await
                                    .map_err(|err| err.to_string())?;
                            }
                            Request::SubscribeStates => {
                                dev_tx
                                    .send(EspHomeMessage::SubscribeStatesRequest(
                                        SubscribeStatesRequest {},
                                    ))
                                    .await
                                    .map_err(|err| err.to_string())?;
                            }
                            Request::ListEntities => {
                                dev_tx
                                    .send(EspHomeMessage::ListEntitiesRequest(
                                        ListEntitiesRequest {},
                                    ))
                                    .await
                                    .map_err(|err| err.to_string())?;
                            }
                            Request::Switch(key, state) => {
                                let command =
                                    EspHomeMessage::SwitchCommandRequest(SwitchCommandRequest {
                                        key,
                                        state,
                                        device_id: 0,
                                    });

                                dev_tx.send(command).await.map_err(|err| err.to_string())?;
                            }
                            Request::Text(key, state) => {
                                let command =
                                    EspHomeMessage::TextCommandRequest(TextCommandRequest {
                                        key,
                                        state,
                                        device_id: 0,
                                    });

                                dev_tx.send(command).await.map_err(|err| err.to_string())?;
                            }
                        }
                        Ok(())
                    })
                    .await
                {
                    error!("{err}")
                }

                cancel_dev2.cancel();
                dev_task.await.map_err(|err| err.to_string())??;

                Ok(())
            },
            32,
            notify,
            on_msg,
        );

        Self {
            obj,
            on_msg,
            on_data,
        }
    }
}

#[no_mangle]
/// create new esphome client
/// @param addr - esphome device ip address or server name
/// @param port - esphome device port (6053 by default)
/// @param notify - client notification from worker
/// @param on_msg - client callback for messages from worker
/// @param on_data - client callback for data from worker
/// @return pointer to handle or nullptr on error
pub extern "C" fn ceammc_esphome_client_new(
    addr: *const c_char,
    port: u16,
    notify: msg_notify,
    on_msg: msg_cb,
    on_data: esphome_client_cb,
) -> *mut esphome_client {
    if addr.is_null() {
        on_msg.error_str("NULL address pointer");
        return null_mut();
    }

    let addr = unsafe { CStr::from_ptr(addr) };
    return Box::into_raw(Box::new(esphome_client::new(
        addr, port, notify, on_msg, on_data,
    )));
}

#[no_mangle]
/// close and free connection to epshome device
/// @param cli - esphome device handle (nullable)
pub extern "C" fn ceammc_esphome_client_free(cli: *mut esphome_client) {
    if !cli.is_null() {
        let _ = unsafe { Box::from_raw(cli) };
    }
}

#[no_mangle]
/// process replies from esphome device
/// @param cli - esphome device handle
pub extern "C" fn ceammc_esphome_client_process(cli: *mut esphome_client) -> bool {
    esphome_client::process_reply_ptr(cli)
}

#[no_mangle]
/// send ping to the connected esphome device
/// @param cli - esphome device handle
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_esphome_client_ping(cli: *mut esphome_client) -> bool {
    esphome_client::send_request(cli, Request::Ping)
}

#[no_mangle]
pub extern "C" fn ceammc_esphome_client_subscribe(cli: *mut esphome_client) -> bool {
    esphome_client::send_request(cli, Request::SubscribeStates)
}

#[no_mangle]
pub extern "C" fn ceammc_esphome_client_list_entities(cli: *mut esphome_client) -> bool {
    esphome_client::send_request(cli, Request::ListEntities)
}

#[no_mangle]
/// turn on/off esphome device switch
/// @param cli - esphome device handle
/// @param key - internal esphome switch id
/// @param state - switch state
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_esphome_client_switch(
    cli: *mut esphome_client,
    key: u32,
    state: bool,
) -> bool {
    esphome_client::send_request(cli, Request::Switch(key, state))
}

#[no_mangle]
pub extern "C" fn ceammc_esphome_client_text(
    cli: *mut esphome_client,
    key: u32,
    text: *const c_char,
) -> bool {
    match ceammc_rs_msg::cstr_to_string(text) {
        Some(text) => esphome_client::send_request(cli, Request::Text(key, text)),
        None => false,
    }
}
