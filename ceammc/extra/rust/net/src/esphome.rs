use std::{
    ffi::{c_char, c_void, CStr, CString},
    ptr::null_mut,
};

use ::esphome_client::{
    types::{
        ColorMode, DeviceInfoRequest, EntityCategory, EspHomeMessage, GetTimeRequest,
        LightCommandRequest, ListEntitiesRequest, NumberCommandRequest, PingRequest,
        SubscribeStatesRequest, SwitchCommandRequest, TextCommandRequest, TimeCommandRequest,
    },
    EspHomeClient,
};
use ceammc_rs_msg::{
    cstr_from_ptr, cstr_from_string, ffi_from_vcstr, msg_cb, msg_notify, vcstr_from_vstring,
    NumThreads, SendState, TokioClient, TokioRtShutdown,
};
use log::{debug, error};
use tokio::task::JoinHandle;

pub const ESPHOME_DEFAULT_PORT: u16 = 6053;

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_entity_id {
    key: u32,
    device_id: u32,
}

#[repr(u8)]
#[derive(Debug)]
#[allow(non_camel_case_types)]
/// esphome entity category
pub enum esphome_category {
    None,
    Config,
    Diagnostic,
}

#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq)]
#[allow(non_camel_case_types)]
pub enum esphome_color_mode {
    Unknown,
    OnOff,
    LegacyBrightness,
    Brightness,
    White,
    ColorTemperature,
    ColdWarmWhite,
    Rgb,
    RgbWhite,
    RgbColorTemperature,
    RgbColdWarmWhite,
}

macro_rules! const_to_colormode {
    ($value:expr, $($variant:ident),* $(,)?) => {{
        $(
            const $variant: i32 = ColorMode::$variant as i32;
        )*

        match $value {
            $(
                $variant => Self::$variant,
            )*
            _ => Self::Unknown,
        }
    }};
}

impl From<i32> for esphome_color_mode {
    #[allow(non_upper_case_globals)]
    fn from(value: i32) -> Self {
        const_to_colormode!(
            value,
            OnOff,
            LegacyBrightness,
            Brightness,
            White,
            ColorTemperature,
            ColdWarmWhite,
            Rgb,
            RgbWhite,
            RgbColorTemperature,
            RgbColdWarmWhite
        )
    }
}

macro_rules! colormode_to_const {
    ($value:expr, $($variant:ident),* $(,)?) => {
        match $value {
            $(
                esphome_color_mode::$variant => ColorMode::$variant as i32,
            )*
        }
    };
}

impl Into<i32> for esphome_color_mode {
    fn into(self) -> i32 {
        colormode_to_const!(
            self,
            Unknown,
            OnOff,
            LegacyBrightness,
            Brightness,
            White,
            ColorTemperature,
            ColdWarmWhite,
            Rgb,
            RgbWhite,
            RgbColorTemperature,
            RgbColdWarmWhite
        )
    }
}

impl From<i32> for esphome_category {
    fn from(value: i32) -> Self {
        const CFG: i32 = EntityCategory::Config as i32;
        const DIAG: i32 = EntityCategory::Diagnostic as i32;
        match value {
            CFG => Self::Config,
            DIAG => Self::Diagnostic,
            _ => Self::None,
        }
    }
}

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_binary_state {
    value: bool,
    missing_state: bool,
}

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_switch_state {
    value: bool,
}

#[repr(C)]
pub struct esphome_text_state {
    /// valid within callback only
    value: *const c_char,
    missing_state: bool,
}

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_sensor_state {
    value: f32,
    missing_state: bool,
}

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_number_state {
    value: f32,
    missing_state: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct esphome_select_state {
    /// valid within callback only
    value: *const c_char,
    missing_state: bool,
}

#[derive(Debug, Clone)]
struct LightState {
    effect: CString,
    brightness: f32,
    color_brightness: f32,
    red: f32,
    green: f32,
    blue: f32,
    white: f32,
    color_temperature: f32,
    cold_white: f32,
    warm_white: f32,
    state: bool,
    color_mode: esphome_color_mode,
}

#[repr(C)]
#[derive(Debug)]
pub struct esphome_light_state {
    /// valid within callback only
    effect: *const c_char,
    brightness: f32,
    color_brightness: f32,
    red: f32,
    green: f32,
    blue: f32,
    white: f32,
    color_temperature: f32,
    cold_white: f32,
    warm_white: f32,
    state: bool,
    color_mode: esphome_color_mode,
}

impl Into<LightState> for esphome_light_state {
    fn into(self) -> LightState {
        LightState {
            effect: cstr_from_ptr(self.effect),
            brightness: self.brightness,
            color_brightness: self.color_brightness,
            red: self.red,
            green: self.green,
            blue: self.blue,
            white: self.white,
            color_temperature: self.color_temperature,
            cold_white: self.cold_white,
            warm_white: self.warm_white,
            state: self.state,
            color_mode: self.color_mode,
        }
    }
}

#[repr(C)]
#[derive(Debug, Clone)]
pub struct esphome_time_state {
    pub hour: u32,
    pub minute: u32,
    pub second: u32,
    /// unused when set time
    pub missing_state: bool,
}

#[repr(C)]
#[derive(Debug)]
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
    entity_category: esphome_category,
    assumed_state: bool,
    disabled_by_default: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct esphome_binary_info {
    /// valid within callback only
    name: *const c_char,
    /// valid within callback only
    icon: *const c_char,
    /// valid within callback only
    object_id: *const c_char,
    /// valid within callback only
    device_class: *const c_char,
    id: esphome_entity_id,
    entity_category: esphome_category,
    disabled_by_default: bool,
    is_status_binary_sensor: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct esphome_sensor_info {
    /// valid within callback only
    object_id: *const c_char,
    /// valid within callback only
    name: *const c_char,
    /// valid within callback only
    icon: *const c_char,
    /// valid within callback only
    unit_of_measurement: *const c_char,
    /// valid within callback only
    device_class: *const c_char,
    id: esphome_entity_id,
    accuracy_decimals: i32,
    state_class: i32,
    entity_category: esphome_category,
    disabled_by_default: bool,
    force_update: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct esphome_time_info {
    /// valid within callback only
    object_id: *const c_char,
    /// valid within callback only
    name: *const c_char,
    /// valid within callback only
    icon: *const c_char,
    id: esphome_entity_id,
    entity_category: esphome_category,
    disabled_by_default: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct esphome_number_info {
    /// valid within callback only
    object_id: *const c_char,
    /// valid within callback only
    name: *const c_char,
    /// valid within callback only
    icon: *const c_char,
    /// valid within callback only
    unit_of_measurement: *const c_char,
    device_class: *const c_char,
    /// valid within callback only
    id: esphome_entity_id,
    min_value: f32,
    max_value: f32,
    step: f32,
    entity_category: esphome_category,
    mode: i32,
    disabled_by_default: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct esphome_select_info {
    id: esphome_entity_id,
    /// valid within callback only
    object_id: *const c_char,
    /// valid within callback only
    name: *const c_char,
    /// valid within callback only
    icon: *const c_char,
    /// valid within callback only
    options: *const *const c_char,
    options_len: usize,
    entity_category: esphome_category,
    disabled_by_default: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct esphome_light_info {
    /// valid within callback only
    name: *const c_char,
    /// valid within callback only
    icon: *const c_char,
    /// valid within callback only
    object_id: *const c_char,
    id: esphome_entity_id,
    /// valid within callback only
    color_modes: *const esphome_color_mode,
    color_modes_len: usize,
    /// valid within callback only
    effects: *const *const c_char,
    effects_len: usize,
    min_mireds: f32,
    max_mireds: f32,
    entity_category: esphome_category,
    disabled_by_default: bool,
}

#[repr(C)]
#[derive(Debug)]
pub struct esphome_device_info {
    /// valid within callback only
    name: *const c_char,
    /// valid within callback only
    mac_address: *const c_char,
    /// valid within callback only
    esphome_version: *const c_char,
    /// valid within callback only
    compilation_time: *const c_char,
    /// valid within callback only
    model: *const c_char,
    /// valid within callback only
    project_name: *const c_char,
    /// valid within callback only
    project_version: *const c_char,
    /// valid within callback only
    manufacturer: *const c_char,
    /// valid within callback only
    friendly_name: *const c_char,
    /// valid within callback only
    suggested_area: *const c_char,
    /// valid within callback only
    bluetooth_mac_address: *const c_char,
    webserver_port: u32,
}

#[derive(Debug, Clone)]
struct TextState {
    value: CString,
    missing_state: bool,
}

#[repr(C)]
#[derive(Debug)]
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
    entity_category: esphome_category,
    min_length: u32,
    max_length: u32,
    mode: i32,
    disabled_by_default: bool,
}

#[derive(Debug)]
enum Request {
    Ping,
    DeviceInfo,
    SubscribeStates,
    ListEntities,
    GetTime,
    Switch(esphome_entity_id, bool),
    Text(esphome_entity_id, String),
    Time(esphome_entity_id, esphome_time_state),
    Number(esphome_entity_id, f32),
    Light(esphome_entity_id, LightState),
}

#[derive(Clone, Debug)]
enum Reply {
    Pong,
    Connected(bool),
    ListEntitiesEnd,
    BinaryState(esphome_entity_id, esphome_binary_state),
    SwitchState(esphome_entity_id, esphome_switch_state),
    SensorState(esphome_entity_id, esphome_sensor_state),
    TextState(esphome_entity_id, TextState),
    TimeState(esphome_entity_id, esphome_time_state),
    NumberState(esphome_entity_id, esphome_number_state),
    SelectState(esphome_entity_id, CString, bool),
    LightState(esphome_entity_id, LightState),
    BinaryInfo {
        id: esphome_entity_id,
        name: CString,
        icon: CString,
        object_id: CString,
        device_class: CString,
        is_status_binary_sensor: bool,
        disabled_by_default: bool,
        entity_category: i32,
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
    LightInfo {
        id: esphome_entity_id,
        name: CString,
        icon: CString,
        object_id: CString,
        supported_color_modes: Vec<esphome_color_mode>,
        effects: Vec<CString>,
        min_mireds: f32,
        max_mireds: f32,
        entity_category: i32,
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
    SensorInfo {
        id: esphome_entity_id,
        object_id: CString,
        name: CString,
        icon: CString,
        unit_of_measurement: CString,
        accuracy_decimals: i32,
        force_update: bool,
        device_class: CString,
        state_class: i32,
        disabled_by_default: bool,
        entity_category: i32,
    },
    TimeInfo {
        id: esphome_entity_id,
        object_id: CString,
        name: CString,
        icon: CString,
        disabled_by_default: bool,
        entity_category: i32,
    },
    NumberInfo {
        id: esphome_entity_id,
        object_id: CString,
        name: CString,
        icon: CString,
        unit_of_measurement: CString,
        device_class: CString,
        min_value: f32,
        max_value: f32,
        step: f32,
        disabled_by_default: bool,
        entity_category: i32,
        mode: i32,
    },
    SelectInfo {
        id: esphome_entity_id,
        object_id: CString,
        name: CString,
        icon: CString,
        options: Vec<CString>,
        disabled_by_default: bool,
        entity_category: i32,
    },
    DeviceInfo {
        name: CString,
        mac_address: CString,
        esphome_version: CString,
        compilation_time: CString,
        model: CString,
        project_name: CString,
        project_version: CString,
        manufacturer: CString,
        friendly_name: CString,
        suggested_area: CString,
        bluetooth_mac_address: CString,
        webserver_port: u32,
    },
}

async fn process_message_from_device(
    msg: Result<EspHomeMessage, ::esphome_client::error::ClientError>,
) -> Result<Option<Reply>, String> {
    let msg = msg.map_err(|err| err.to_string())?;

    match msg {
        // EspHomeMessage::DisconnectResponse(disconnect_response) => todo!(),
        EspHomeMessage::PingResponse(_) => Ok(Some(Reply::Pong)),
        EspHomeMessage::DeviceInfoResponse(dev) => Ok(Some(Reply::DeviceInfo {
            name: cstr_from_string(dev.name),
            mac_address: cstr_from_string(dev.mac_address),
            esphome_version: cstr_from_string(dev.esphome_version),
            compilation_time: cstr_from_string(dev.compilation_time),
            model: cstr_from_string(dev.model),
            project_name: cstr_from_string(dev.project_name),
            project_version: cstr_from_string(dev.project_version),
            manufacturer: cstr_from_string(dev.manufacturer),
            friendly_name: cstr_from_string(dev.friendly_name),
            suggested_area: cstr_from_string(dev.suggested_area),
            bluetooth_mac_address: cstr_from_string(dev.bluetooth_mac_address),
            webserver_port: dev.webserver_port,
        })),
        EspHomeMessage::ListEntitiesDoneResponse(_) => Ok(Some(Reply::ListEntitiesEnd)),
        EspHomeMessage::ListEntitiesBinarySensorResponse(bin) => Ok(Some(Reply::BinaryInfo {
            id: esphome_entity_id {
                key: bin.key,
                device_id: bin.device_id,
            },
            name: cstr_from_string(bin.name),
            icon: cstr_from_string(bin.icon),
            object_id: cstr_from_string(bin.object_id),
            device_class: cstr_from_string(bin.device_class),
            is_status_binary_sensor: bin.is_status_binary_sensor,
            disabled_by_default: bin.disabled_by_default,
            entity_category: bin.entity_category,
        })),
        EspHomeMessage::BinarySensorStateResponse(bin) => Ok(Some(Reply::BinaryState(
            esphome_entity_id {
                key: bin.key,
                device_id: bin.device_id,
            },
            esphome_binary_state {
                value: bin.state,
                missing_state: bin.missing_state,
            },
        ))),
        EspHomeMessage::ListEntitiesCoverResponse(list_entities_cover_response) => todo!(),
        EspHomeMessage::CoverStateResponse(cover_state_response) => todo!(),
        EspHomeMessage::ListEntitiesFanResponse(list_entities_fan_response) => todo!(),
        EspHomeMessage::FanStateResponse(fan_state_response) => todo!(),
        EspHomeMessage::ListEntitiesLightResponse(light) => Ok(Some(Reply::LightInfo {
            id: esphome_entity_id {
                key: light.key,
                device_id: light.device_id,
            },
            name: cstr_from_string(light.name),
            icon: cstr_from_string(light.icon),
            object_id: cstr_from_string(light.object_id),
            supported_color_modes: light
                .supported_color_modes
                .iter()
                .map(|x| (*x).into())
                .collect(),
            effects: vcstr_from_vstring(light.effects),
            min_mireds: light.min_mireds,
            max_mireds: light.max_mireds,
            entity_category: light.entity_category,
            disabled_by_default: light.disabled_by_default,
        })),
        EspHomeMessage::LightStateResponse(light) => Ok(Some(Reply::LightState(
            esphome_entity_id {
                key: light.key,
                device_id: light.device_id,
            },
            LightState {
                effect: cstr_from_string(light.effect),
                brightness: light.brightness,
                color_brightness: light.color_brightness,
                red: light.red,
                green: light.green,
                blue: light.blue,
                white: light.white,
                color_temperature: light.color_temperature,
                cold_white: light.cold_white,
                warm_white: light.warm_white,
                state: light.state,
                color_mode: light.color_mode.into(),
            },
        ))),
        EspHomeMessage::ListEntitiesSensorResponse(sensor) => Ok(Some(Reply::SensorInfo {
            id: esphome_entity_id {
                key: sensor.key,
                device_id: sensor.device_id,
            },
            object_id: cstr_from_string(sensor.object_id),
            name: cstr_from_string(sensor.name),
            icon: cstr_from_string(sensor.icon),
            unit_of_measurement: cstr_from_string(sensor.unit_of_measurement),
            accuracy_decimals: sensor.accuracy_decimals,
            force_update: sensor.force_update,
            device_class: cstr_from_string(sensor.device_class),
            state_class: sensor.state_class,
            disabled_by_default: sensor.disabled_by_default,
            entity_category: sensor.entity_category,
        })),
        EspHomeMessage::SensorStateResponse(sensor) => Ok(Some(Reply::SensorState(
            esphome_entity_id {
                key: sensor.key,
                device_id: sensor.device_id,
            },
            esphome_sensor_state {
                value: sensor.state,
                missing_state: sensor.missing_state,
            },
        ))),
        EspHomeMessage::ListEntitiesSwitchResponse(sw) => Ok(Some(Reply::SwitchInfo {
            id: esphome_entity_id {
                key: sw.key,
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
                key: x.key,
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
                key: txt.key,
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
        EspHomeMessage::ListEntitiesNumberResponse(num) => Ok(Some(Reply::NumberInfo {
            id: esphome_entity_id {
                key: num.key,
                device_id: num.device_id,
            },
            object_id: cstr_from_string(num.object_id),
            name: cstr_from_string(num.name),
            icon: cstr_from_string(num.icon),
            unit_of_measurement: cstr_from_string(num.unit_of_measurement),
            device_class: cstr_from_string(num.device_class),
            min_value: num.min_value,
            max_value: num.max_value,
            step: num.step,
            disabled_by_default: num.disabled_by_default,
            entity_category: num.entity_category,
            mode: num.mode,
        })),
        EspHomeMessage::NumberStateResponse(num) => Ok(Some(Reply::NumberState(
            esphome_entity_id {
                key: num.key,
                device_id: num.device_id,
            },
            esphome_number_state {
                value: num.state,
                missing_state: num.missing_state,
            },
        ))),
        EspHomeMessage::ListEntitiesSelectResponse(sel) => Ok(Some(Reply::SelectInfo {
            id: esphome_entity_id {
                key: sel.key,
                device_id: sel.device_id,
            },
            object_id: cstr_from_string(sel.object_id),
            name: cstr_from_string(sel.name),
            icon: cstr_from_string(sel.icon),
            options: vcstr_from_vstring(&sel.options),
            disabled_by_default: sel.disabled_by_default,
            entity_category: sel.entity_category,
        })),
        EspHomeMessage::SelectStateResponse(sel) => Ok(Some(Reply::SelectState(
            esphome_entity_id {
                key: sel.key,
                device_id: sel.device_id,
            },
            cstr_from_string(sel.state),
            sel.missing_state,
        ))),
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
                key: txt.key,
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
        EspHomeMessage::ListEntitiesTimeResponse(time) => Ok(Some(Reply::TimeInfo {
            id: esphome_entity_id {
                key: time.key,
                device_id: time.device_id,
            },
            object_id: cstr_from_string(time.object_id),
            name: cstr_from_string(time.name),
            icon: cstr_from_string(time.icon),
            disabled_by_default: time.disabled_by_default,
            entity_category: time.entity_category,
        })),
        EspHomeMessage::TimeStateResponse(time) => Ok(Some(Reply::TimeState(
            esphome_entity_id {
                key: time.key,
                device_id: time.device_id,
            },
            esphome_time_state {
                hour: time.hour,
                minute: time.minute,
                second: time.second,
                missing_state: time.missing_state,
            },
        ))),
        EspHomeMessage::ListEntitiesEventResponse(list_entities_event_response) => todo!(),
        EspHomeMessage::EventResponse(event_response) => todo!(),
        EspHomeMessage::ListEntitiesValveResponse(list_entities_valve_response) => todo!(),
        EspHomeMessage::ValveStateResponse(valve_state_response) => todo!(),
        EspHomeMessage::ListEntitiesDateTimeResponse(list_entities_date_time_response) => {
            //
            Ok(None)
        }
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
    on_binary:
        extern "C" fn(user: *mut c_void, key: esphome_entity_id, state: esphome_binary_state),
    on_text: extern "C" fn(user: *mut c_void, key: esphome_entity_id, state: esphome_text_state),
    on_sensor:
        extern "C" fn(user: *mut c_void, key: esphome_entity_id, state: esphome_sensor_state),
    on_number:
        extern "C" fn(user: *mut c_void, key: esphome_entity_id, state: esphome_number_state),
    on_select:
        extern "C" fn(user: *mut c_void, key: esphome_entity_id, state: esphome_select_state),
    on_light: extern "C" fn(user: *mut c_void, key: esphome_entity_id, state: esphome_light_state),
    on_time: extern "C" fn(user: *mut c_void, key: esphome_entity_id, state: esphome_time_state),
    on_info_switch: extern "C" fn(user: *mut c_void, info: &esphome_switch_info),
    on_info_text: extern "C" fn(user: *mut c_void, info: &esphome_text_info),
    on_info_binary: extern "C" fn(user: *mut c_void, info: &esphome_binary_info),
    on_info_sensor: extern "C" fn(user: *mut c_void, info: &esphome_sensor_info),
    on_info_number: extern "C" fn(user: *mut c_void, info: &esphome_number_info),
    on_info_select: extern "C" fn(user: *mut c_void, info: &esphome_select_info),
    on_info_light: extern "C" fn(user: *mut c_void, info: &esphome_light_info),
    on_info_time: extern "C" fn(user: *mut c_void, info: &esphome_time_info),
    on_info_device: extern "C" fn(user: *mut c_void, info: &esphome_device_info),
    on_connection: extern "C" fn(user: *mut c_void, state: bool),
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

    fn sensor(&self, key: esphome_entity_id, state: esphome_sensor_state) {
        (self.on_sensor)(self.user, key, state)
    }

    fn time(&self, key: esphome_entity_id, state: esphome_time_state) {
        (self.on_time)(self.user, key, state)
    }

    fn binary(&self, key: esphome_entity_id, state: esphome_binary_state) {
        (self.on_binary)(self.user, key, state)
    }

    fn number(&self, key: esphome_entity_id, state: esphome_number_state) {
        (self.on_number)(self.user, key, state)
    }

    fn select(&self, key: esphome_entity_id, state: CString, missing_state: bool) {
        (self.on_select)(
            self.user,
            key,
            esphome_select_state {
                value: state.as_ptr(),
                missing_state,
            },
        )
    }

    fn light(&self, key: esphome_entity_id, state: LightState) {
        let st = esphome_light_state {
            effect: state.effect.as_ptr(),
            brightness: state.brightness,
            color_brightness: state.color_brightness,
            red: state.red,
            green: state.green,
            blue: state.blue,
            white: state.white,
            color_temperature: state.color_temperature,
            cold_white: state.cold_white,
            warm_white: state.warm_white,
            state: state.state,
            color_mode: state.color_mode,
        };

        (self.on_light)(self.user, key, st)
    }

    fn info_binary(&self, info: esphome_binary_info) {
        (self.on_info_binary)(self.user, &info)
    }

    fn info_device(&self, info: esphome_device_info) {
        (self.on_info_device)(self.user, &info)
    }

    fn info_switch(&self, info: esphome_switch_info) {
        (self.on_info_switch)(self.user, &info)
    }

    fn info_text(&self, info: esphome_text_info) {
        (self.on_info_text)(self.user, &info)
    }

    fn info_sensor(&self, info: esphome_sensor_info) {
        (self.on_info_sensor)(self.user, &info)
    }

    fn info_time(&self, info: esphome_time_info) {
        (self.on_info_time)(self.user, &info)
    }

    fn info_number(&self, info: esphome_number_info) {
        (self.on_info_number)(self.user, &info)
    }

    fn info_select(&self, info: esphome_select_info) {
        (self.on_info_select)(self.user, &info)
    }

    fn info_light(&self, info: esphome_light_info) {
        (self.on_info_light)(self.user, &info)
    }

    fn connected(&self, state: bool) {
        (self.on_connection)(self.user, state)
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
                Reply::BinaryState(key, state) => cli.on_data.binary(key, state),
                Reply::TextState(key, state) => cli.on_data.text(key, state),
                Reply::SensorState(key, state) => cli.on_data.sensor(key, state),
                Reply::TimeState(key, state) => cli.on_data.time(key, state),
                Reply::NumberState(key, state) => cli.on_data.number(key, state),
                Reply::SelectState(key, state, missing_state) => {
                    cli.on_data.select(key, state, missing_state)
                }
                Reply::LightState(key, state) => cli.on_data.light(key, state),
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
                    entity_category: entity_category.into(),
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
                    entity_category: entity_category.into(),
                    disabled_by_default,
                    pattern: pattern.as_ptr(),
                    min_length,
                    max_length,
                    mode,
                }),
                Reply::ListEntitiesEnd => {}
                Reply::BinaryInfo {
                    id,
                    name,
                    icon,
                    object_id,
                    device_class,
                    is_status_binary_sensor,
                    disabled_by_default,
                    entity_category,
                } => cli.on_data.info_binary(esphome_binary_info {
                    name: name.as_ptr(),
                    icon: icon.as_ptr(),
                    object_id: object_id.as_ptr(),
                    device_class: device_class.as_ptr(),
                    id,
                    entity_category: entity_category.into(),
                    disabled_by_default,
                    is_status_binary_sensor,
                }),
                Reply::DeviceInfo {
                    name,
                    mac_address,
                    esphome_version,
                    compilation_time,
                    model,
                    project_name,
                    project_version,
                    manufacturer,
                    friendly_name,
                    suggested_area,
                    bluetooth_mac_address,
                    webserver_port,
                } => cli.on_data.info_device(esphome_device_info {
                    name: name.as_ptr(),
                    mac_address: mac_address.as_ptr(),
                    esphome_version: esphome_version.as_ptr(),
                    compilation_time: compilation_time.as_ptr(),
                    model: model.as_ptr(),
                    project_name: project_name.as_ptr(),
                    project_version: project_version.as_ptr(),
                    manufacturer: manufacturer.as_ptr(),
                    friendly_name: friendly_name.as_ptr(),
                    suggested_area: suggested_area.as_ptr(),
                    bluetooth_mac_address: bluetooth_mac_address.as_ptr(),
                    webserver_port,
                }),
                Reply::Connected(state) => cli.on_data.connected(state),
                Reply::SensorInfo {
                    id,
                    object_id,
                    name,
                    icon,
                    unit_of_measurement,
                    accuracy_decimals,
                    force_update,
                    device_class,
                    state_class,
                    disabled_by_default,
                    entity_category,
                } => cli.on_data.info_sensor(esphome_sensor_info {
                    object_id: object_id.as_ptr(),
                    name: name.as_ptr(),
                    icon: icon.as_ptr(),
                    unit_of_measurement: unit_of_measurement.as_ptr(),
                    device_class: device_class.as_ptr(),
                    accuracy_decimals,
                    state_class,
                    entity_category: entity_category.into(),
                    disabled_by_default,
                    force_update,
                    id,
                }),
                Reply::TimeInfo {
                    id,
                    object_id,
                    name,
                    icon,
                    disabled_by_default,
                    entity_category,
                } => cli.on_data.info_time(esphome_time_info {
                    object_id: object_id.as_ptr(),
                    name: name.as_ptr(),
                    icon: icon.as_ptr(),
                    id,
                    entity_category: entity_category.into(),
                    disabled_by_default,
                }),
                Reply::NumberInfo {
                    id,
                    object_id,
                    name,
                    icon,
                    unit_of_measurement,
                    device_class,
                    min_value,
                    max_value,
                    step,
                    disabled_by_default,
                    entity_category,
                    mode,
                } => cli.on_data.info_number(esphome_number_info {
                    object_id: object_id.as_ptr(),
                    name: name.as_ptr(),
                    icon: icon.as_ptr(),
                    unit_of_measurement: unit_of_measurement.as_ptr(),
                    device_class: device_class.as_ptr(),
                    id,
                    min_value,
                    max_value,
                    step,
                    entity_category: entity_category.into(),
                    mode,
                    disabled_by_default,
                }),
                Reply::SelectInfo {
                    id,
                    object_id,
                    name,
                    icon,
                    options,
                    disabled_by_default,
                    entity_category,
                } => {
                    let options = ffi_from_vcstr(&options);
                    cli.on_data.info_select(esphome_select_info {
                        id,
                        object_id: object_id.as_ptr(),
                        name: name.as_ptr(),
                        icon: icon.as_ptr(),
                        options: options.as_ptr(),
                        options_len: options.len(),
                        disabled_by_default,
                        entity_category: entity_category.into(),
                    })
                }
                Reply::LightInfo {
                    id,
                    name,
                    icon,
                    object_id,
                    supported_color_modes,
                    effects,
                    min_mireds,
                    max_mireds,
                    entity_category,
                    disabled_by_default,
                } => {
                    let fx = ffi_from_vcstr(&effects);
                    cli.on_data.info_light(esphome_light_info {
                        name: name.as_ptr(),
                        icon: icon.as_ptr(),
                        object_id: object_id.as_ptr(),
                        id,
                        color_modes: supported_color_modes.as_ptr(),
                        color_modes_len: supported_color_modes.len(),
                        effects: fx.as_ptr(),
                        effects_len: fx.len(),
                        min_mireds,
                        max_mireds,
                        entity_category: entity_category.into(),
                        disabled_by_default,
                    })
                }
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
        let obj = TokioClient::<Request, Reply>::start_worker(
            NumThreads::Current,
            TokioRtShutdown::NoWait,
            32,
            async move |mut channel, cancel| {
                let addr = format!("{addr}:{port}");
                channel
                    .send_debug(format!("connecting to {addr} ..."))
                    .to_worker_result()?;

                let client;
                let cancel_connect = cancel.clone();
                tokio::select! {
                    _ = cancel_connect.cancelled() => {
                        return Ok(())
                    },
                    cli = EspHomeClient::builder()
                    .address(&addr)
                    // .key(KEY)
                    .connect() => {
                        client = Some(cli.map_err(|err| err.to_string())?);
                    },
                }

                let mut client = client.ok_or("can't create client")?;

                channel
                    .send_debug(format!("connected ..."))
                    .to_worker_result()?;
                channel
                    .send_data(Reply::Connected(true))
                    .to_worker_result()?;

                let (dev_tx, mut dev_rx) = tokio::sync::mpsc::channel(24);

                let to_client = channel.clone_sender();
                let cancel_dev = cancel.clone();
                let dev_task: JoinHandle<Result<(), String>> = tokio::spawn(async move {
                    debug!("esphome device thread start");
                    loop {
                        tokio::select! {
                            msg = client.try_read() => {
                                if let Some(reply) = process_message_from_device(msg).await? {
                                    debug!("{reply:?}");
                                    to_client.send_data(reply).to_worker_result()?;
                                }
                            },
                            Some(msg) = dev_rx.recv() => {
                                client.try_write(msg).await.map_err(|err| err.to_string())?;
                            },
                            _ = cancel_dev.cancelled() => break,
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
                            Request::Switch(id, state) => {
                                let command =
                                    EspHomeMessage::SwitchCommandRequest(SwitchCommandRequest {
                                        key: id.key,
                                        state,
                                        device_id: id.device_id,
                                    });

                                dev_tx.send(command).await.map_err(|err| err.to_string())?;
                            }
                            Request::Text(id, state) => {
                                let command =
                                    EspHomeMessage::TextCommandRequest(TextCommandRequest {
                                        key: id.key,
                                        state,
                                        device_id: id.device_id,
                                    });

                                dev_tx.send(command).await.map_err(|err| err.to_string())?;
                            }
                            Request::DeviceInfo => {
                                let command =
                                    EspHomeMessage::DeviceInfoRequest(DeviceInfoRequest {});
                                dev_tx.send(command).await.map_err(|err| err.to_string())?;
                            }
                            Request::Time(id, time) => {
                                let command =
                                    EspHomeMessage::TimeCommandRequest(TimeCommandRequest {
                                        key: id.key,
                                        hour: time.hour,
                                        minute: time.minute,
                                        second: time.second,
                                        device_id: id.device_id,
                                    });
                                dev_tx.send(command).await.map_err(|err| err.to_string())?;
                            }
                            Request::GetTime => {
                                let command = EspHomeMessage::GetTimeRequest(GetTimeRequest {});
                                dev_tx.send(command).await.map_err(|err| err.to_string())?;
                            }
                            Request::Number(id, state) => {
                                let command =
                                    EspHomeMessage::NumberCommandRequest(NumberCommandRequest {
                                        key: id.key,
                                        state,
                                        device_id: id.device_id,
                                    });
                                dev_tx.send(command).await.map_err(|err| err.to_string())?;
                            }
                            Request::Light(id, state) => {
                                let command =
                                    EspHomeMessage::LightCommandRequest(LightCommandRequest {
                                        key: id.key,
                                        state: state.state,
                                        device_id: id.device_id,
                                        has_state: state.color_mode == esphome_color_mode::OnOff,
                                        has_brightness: state.color_mode
                                            == esphome_color_mode::Brightness,
                                        brightness: state.brightness,
                                        has_color_mode: state.color_mode
                                            != esphome_color_mode::Unknown,
                                        color_mode: state.color_mode.into(),
                                        has_color_brightness: state.color_mode
                                            == esphome_color_mode::Brightness,
                                        color_brightness: state.color_brightness,
                                        has_rgb: state.color_mode == esphome_color_mode::Rgb,
                                        red: state.red,
                                        green: state.green,
                                        blue: state.blue,
                                        has_white: state.color_mode == esphome_color_mode::White,
                                        white: state.white,
                                        has_color_temperature: state.color_mode
                                            == esphome_color_mode::ColorTemperature,
                                        color_temperature: state.color_temperature,
                                        has_cold_white: state.color_mode
                                            == esphome_color_mode::ColdWarmWhite,
                                        cold_white: state.cold_white,
                                        has_warm_white: state.color_mode
                                            == esphome_color_mode::ColdWarmWhite,
                                        warm_white: state.warm_white,
                                        has_transition_length: false,
                                        transition_length: 0,
                                        has_flash_length: false,
                                        flash_length: 0,
                                        has_effect: !state.effect.is_empty(),
                                        effect: state.effect.into_string().unwrap_or_default(),
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

                cancel.cancel();
                dev_task.await.map_err(|err| err.to_string())??;

                channel
                    .send_data(Reply::Connected(false))
                    .to_worker_result()?;

                Ok(())
            },
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
/// device info request
/// @param cli - esphome device handle
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_esphome_client_device_info(cli: *mut esphome_client) -> bool {
    esphome_client::send_request(cli, Request::DeviceInfo)
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
/// @param id - internal esphome switch id (not null!)
/// @param state - switch state
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_esphome_client_switch(
    cli: *mut esphome_client,
    id: &esphome_entity_id,
    state: bool,
) -> bool {
    esphome_client::send_request(cli, Request::Switch(id.clone(), state))
}

#[no_mangle]
pub extern "C" fn ceammc_esphome_client_text(
    cli: *mut esphome_client,
    id: &esphome_entity_id,
    text: *const c_char,
) -> bool {
    match ceammc_rs_msg::cstr_to_string(text) {
        Some(text) => esphome_client::send_request(cli, Request::Text(id.clone(), text)),
        None => false,
    }
}

#[no_mangle]
/// set esphome device time
/// @param cli - esphome device handle
/// @param id - internal esphome sensor id (not null!)
/// @param time - new time
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_esphome_client_set_time(
    cli: *mut esphome_client,
    id: &esphome_entity_id,
    time: esphome_time_state,
) -> bool {
    esphome_client::send_request(cli, Request::Time(id.clone(), time))
}

#[no_mangle]
/// get esphome device time
/// @param cli - esphome device handle
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_esphome_client_get_time(cli: *mut esphome_client) -> bool {
    esphome_client::send_request(cli, Request::GetTime)
}

#[no_mangle]
/// set esphome device number state
/// @param cli - esphome device handle
/// @param id - internal esphome sensor id (not null!)
/// @param state - new state
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_esphome_client_number(
    cli: *mut esphome_client,
    id: &esphome_entity_id,
    state: f32,
) -> bool {
    esphome_client::send_request(cli, Request::Number(id.clone(), state))
}

#[no_mangle]
/// set esphome device light state
/// @param cli - esphome device handle
/// @param id - internal esphome sensor id (not null!)
/// @param state - new light state
/// @return true on sucess, false on error (if device is disconnected etc.)
pub extern "C" fn ceammc_esphome_client_light(
    cli: *mut esphome_client,
    id: &esphome_entity_id,
    state: esphome_light_state,
) -> bool {
    esphome_client::send_request(cli, Request::Light(id.clone(), state.into()))
}
