use std::{
    ffi::CString,
    os::raw::{c_char, c_void},
    ptr::null_mut,
    time::Duration,
};

use gilrs::{Axis, Button, EventType, Gilrs, PowerInfo};
use tokio::time::sleep;

use crate::hw_notify_cb;

#[allow(non_camel_case_types)]
#[repr(C)]
pub enum hw_gamepad_rc {
    Ok = 0,
    InvalidHandle,
}


#[derive(Default, Debug)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub enum hw_gamepad_btn {
    // Action Pad
    South,
    East,
    North,
    West,
    C,
    Z,
    // Triggers
    LeftTrigger,
    LeftTrigger2,
    RightTrigger,
    RightTrigger2,
    // Menu Pad
    Select,
    Start,
    Mode,
    // Sticks
    LeftThumb,
    RightThumb,
    // D-Pad
    DPadUp,
    DPadDown,
    DPadLeft,
    DPadRight,
    #[default]
    Unknown,
}

impl hw_gamepad_btn {
    fn new(btn: Button) -> Self {
        match btn {
            Button::South => Self::South,
            Button::East => Self::East,
            Button::North => Self::North,
            Button::West => Self::West,
            Button::C => Self::C,
            Button::Z => Self::Z,
            Button::LeftTrigger => Self::LeftTrigger,
            Button::LeftTrigger2 => Self::LeftTrigger2,
            Button::RightTrigger => Self::RightTrigger,
            Button::RightTrigger2 => Self::RightTrigger2,
            Button::Select => Self::Select,
            Button::Start => Self::Start,
            Button::Mode => Self::Mode,
            Button::LeftThumb => Self::LeftThumb,
            Button::RightThumb => Self::RightThumb,
            Button::DPadUp => Self::DPadUp,
            Button::DPadDown => Self::DPadDown,
            Button::DPadLeft => Self::DPadLeft,
            Button::DPadRight => Self::DPadRight,
            Button::Unknown => Self::Unknown,
        }
    }
}

#[derive(Default, Debug)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub enum hw_gamepad_event_type {
    ButtonPressed,
    ButtonRepeated,
    ButtonReleased,
    ButtonChanged,
    AxisChanged,
    Connected,
    Disconnected,
    #[default]
    Dropped,
}

#[derive(Default, Debug)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub enum hw_gamepad_event_axis {
    LeftStickX,
    LeftStickY,
    LeftZ,
    RightStickX,
    RightStickY,
    RightZ,
    DPadX,
    DPadY,
    #[default]
    Unknown,
}

impl hw_gamepad_event_axis {
    fn new(a: Axis) -> Self {
        match a {
            Axis::LeftStickX => Self::LeftStickX,
            Axis::LeftStickY => Self::LeftStickY,
            Axis::LeftZ => Self::LeftZ,
            Axis::RightStickX => Self::RightStickX,
            Axis::RightStickY => Self::RightStickY,
            Axis::RightZ => Self::RightZ,
            Axis::DPadX => Self::DPadX,
            Axis::DPadY => Self::DPadY,
            Axis::Unknown => Self::Unknown,
        }
    }
}

#[derive(Default, Debug)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub struct hw_gamepad_event {
    /// gamepad id
    id: usize,
    button: hw_gamepad_btn,
    event: hw_gamepad_event_type,
    axis: hw_gamepad_event_axis,
    value: f32,
}

impl hw_gamepad_event {
    fn new(id: usize, ev: EventType) -> Self {
        match ev {
            EventType::ButtonPressed(b, _) => hw_gamepad_event {
                id,
                button: hw_gamepad_btn::new(b),
                event: hw_gamepad_event_type::ButtonPressed,
                ..Default::default()
            },
            EventType::ButtonRepeated(b, _) => hw_gamepad_event {
                id,
                button: hw_gamepad_btn::new(b),
                event: hw_gamepad_event_type::ButtonRepeated,
                ..Default::default()
            },
            EventType::ButtonReleased(b, _) => hw_gamepad_event {
                id,
                button: hw_gamepad_btn::new(b),
                event: hw_gamepad_event_type::ButtonReleased,
                ..Default::default()
            },
            EventType::ButtonChanged(b, v, _) => hw_gamepad_event {
                id,
                button: hw_gamepad_btn::new(b),
                event: hw_gamepad_event_type::ButtonChanged,
                value: v,
                ..Default::default()
            },
            EventType::AxisChanged(a, v, _) => hw_gamepad_event {
                id,
                event: hw_gamepad_event_type::AxisChanged,
                axis: hw_gamepad_event_axis::new(a),
                value: v,
                ..Default::default()
            },
            EventType::Connected => hw_gamepad_event {
                id,
                event: hw_gamepad_event_type::Connected,
                ..Default::default()
            },
            EventType::Disconnected => hw_gamepad_event {
                id,
                event: hw_gamepad_event_type::Disconnected,
                ..Default::default()
            },
            EventType::Dropped => hw_gamepad_event {
                id,
                event: hw_gamepad_event_type::Dropped,
                ..Default::default()
            },
        }
    }
}

#[derive(Debug)]
enum GamepadRequest {
    ListDevices,
    Quit,
}

struct GamepadInfo {
    name: CString,
    os_name: CString,
    id: usize,
    power: hw_gamepad_powerinfo,
    vid: u16,
    pid: u16,
    is_connected: bool,
    has_ff: bool,
}

enum ReplyData {
    Info(GamepadInfo),
    Event(hw_gamepad_event),
}

type GamepadReply = Result<ReplyData, CString>;
type GamepadTx = tokio::sync::mpsc::Sender<GamepadRequest>;
type GamepadRx = tokio::sync::mpsc::Receiver<GamepadReply>;

#[allow(non_camel_case_types)]
/// gamepad opaque type
pub struct hw_gamepad {
    on_err: gamepad_err_cb,
    on_event: gamepad_event_cb,
    on_devinfo: gamepad_listdev_cb,
    req_tx: GamepadTx,
    reply_rx: GamepadRx,
}

impl Drop for hw_gamepad {
    fn drop(&mut self) {
        log::debug!("[owner] sending quit");
        if let Err(err) = self.req_tx.blocking_send(GamepadRequest::Quit) {
            log::error!("{err}");
        }
        log::debug!("[owner] done");
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
pub enum hw_gamepad_powerstate {
    Unknown = 0,
    Wired,
    Discharging,
    Charging,
    Charged,
}

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Copy)]
pub struct hw_gamepad_powerinfo {
    state: hw_gamepad_powerstate,
    data: u8,
}

impl hw_gamepad_powerinfo {
    fn new(info: PowerInfo) -> Self {
        match info {
            PowerInfo::Unknown => Self::_new(0, hw_gamepad_powerstate::Unknown),
            PowerInfo::Wired => Self::_new(0, hw_gamepad_powerstate::Wired),
            PowerInfo::Discharging(data) => Self::_new(data, hw_gamepad_powerstate::Discharging),
            PowerInfo::Charging(data) => Self::_new(data, hw_gamepad_powerstate::Charging),
            PowerInfo::Charged => Self::_new(0, hw_gamepad_powerstate::Charged),
        }
    }

    fn _new(data: u8, state: hw_gamepad_powerstate) -> Self {
        hw_gamepad_powerinfo {
            data: data,
            state: state,
        }
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct gamepad_err_cb {
    /// pointer to user data
    user: *mut c_void,
    cb: Option<extern "C" fn(*mut c_void, *const c_char)>,
}

impl gamepad_err_cb {
    fn exec(&self, msg: &str) {
        self.cb.map(|f| {
            f(self.user, CString::new(msg).unwrap_or_default().as_ptr());
        });
    }

    fn exec_cstr(&self, msg: &CString) {
        self.cb.map(|f| {
            f(self.user, msg.as_ptr());
        });
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct gamepad_event_cb {
    /// pointer to user data
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, event: &hw_gamepad_event)>,
}

impl gamepad_event_cb {
    fn exec(&mut self, event: &hw_gamepad_event) {
        self.cb.map(|f| {
            f(self.user, event);
        });
    }
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct gamepad_dev_info {
    /// gamepad name
    name: *const c_char,
    /// gamepad os_name
    os_name: *const c_char,
    /// gamepad id
    id: usize,
    /// gamepad power info
    power: hw_gamepad_powerinfo,
    /// gamepad vendor id
    vid: u16,
    /// gamepad product id
    pid: u16,
    /// gamepad is connected
    is_connected: bool,
    /// gamepad has force feedback capability
    has_ff: bool,
}

#[repr(C)]
#[allow(non_camel_case_types)]
pub struct gamepad_listdev_cb {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, info: &gamepad_dev_info)>,
}
impl gamepad_listdev_cb {
    fn exec(&self, info: &GamepadInfo) {
        self.cb.map(|f| {
            f(
                self.user,
                &gamepad_dev_info {
                    name: info.name.as_ptr(),
                    os_name: info.os_name.as_ptr(),
                    id: info.id,
                    power: info.power,
                    vid: info.vid,
                    pid: info.pid,
                    is_connected: info.is_connected,
                    has_ff: info.has_ff,
                },
            );
        });
    }
}

async fn list_devices(
    gp: &Gilrs,
    tx: &tokio::sync::mpsc::Sender<GamepadReply>,
    cb_notify: &hw_notify_cb,
) {
    for (gid, gamepad) in gp.gamepads() {
        log::debug!("gid: {gid}");
        let name = CString::new(gamepad.name()).unwrap_or_default();
        let os_name = CString::new(gamepad.os_name()).unwrap_or_default();
        let id = gid.into();
        let vid = gamepad.vendor_id().unwrap_or(0);
        let pid = gamepad.product_id().unwrap_or(0);
        let is_connected = gamepad.is_connected();
        let power = hw_gamepad_powerinfo::new(gamepad.power_info());

        if let Err(err) = tx
            .send(Ok(ReplyData::Info(GamepadInfo {
                name,
                os_name,
                id,
                power,
                vid,
                pid,
                is_connected,
                has_ff: gamepad.is_ff_supported(),
            })))
            .await
        {
            log::error!("send error: {err}");
        }

        // log::debug!("gamepad: {gamepad:?}");
        cb_notify.notify();
    }

    cb_notify.notify();
}

/// create new gamepad
/// @param on_err - error callback
/// @param on_event - gamepad event callback
/// @param on_devinfo - gamepad list connected devices callback
/// @param cb_notify - dispatcher notification
/// @return pointer to new gamepad or NULL on error
#[no_mangle]
pub extern "C" fn ceammc_hw_gamepad_new(
    on_err: gamepad_err_cb,
    on_event: gamepad_event_cb,
    on_devinfo: gamepad_listdev_cb,
    cb_notify: hw_notify_cb,
    poll_time_ms: u64,
) -> *mut hw_gamepad {
    let rt = tokio::runtime::Runtime::new();

    match rt {
        Ok(rt) => {
            log::debug!("[owner] creating tokio runtime ...");

            let (req_tx, mut req_rx) = tokio::sync::mpsc::channel::<GamepadRequest>(16);
            let (reply_tx, reply_rx) = tokio::sync::mpsc::channel::<GamepadReply>(64);

            std::thread::spawn(move || {
                log::debug!("[worker thread] starting ...");

                let x = rt.block_on(async move {
                    match Gilrs::new() {
                        Ok(mut gp) => {
                            log::debug!("[worker thread] starting runloop ...");
                            loop {
                                if let Ok(req) =  req_rx.try_recv() {
                                    match req {
                                        GamepadRequest::ListDevices => {
                                            log::debug!("[worker thread] list devices");
                                            list_devices(&gp, &reply_tx, &cb_notify).await;
                                        }
                                        GamepadRequest::Quit => {
                                            log::debug!("[worker thread] quit");
                                            return Ok(());
                                        }
                                    }
                                }

                                while let Some(event) = gp.next_event() {
                                    let ev = hw_gamepad_event::new(event.id.into(), event.event);
                                    log::debug!("event: {ev:?}");

                                    if let Err(err) = reply_tx.send(Ok(ReplyData::Event(ev))).await
                                    {
                                        log::error!("[worker thread] send error: {err}");
                                    }

                                    cb_notify.notify();
                                }

                                sleep(Duration::from_millis(poll_time_ms)).await;
                            }
                        }
                        Err(err) => {
                            return Err(err.to_string());
                        }
                    }
                });

                if let Err(err) = x {
                    log::error!("[worker thread] error: {err}");
                }

                log::debug!("[worker thread] exit");
            });

            Box::into_raw(Box::new(hw_gamepad {
                on_err,
                on_event,
                on_devinfo,
                req_tx,
                reply_rx,
            }))
        }
        Err(err) => {
            on_err.exec(err.to_string().as_str());
            return null_mut();
        }
    }
}

/// free gamepad
/// @param gp - pointer to gp
#[no_mangle]
pub extern "C" fn ceammc_hw_gamepad_free(gp: *mut hw_gamepad) {
    if !gp.is_null() {
        drop(unsafe { Box::from_raw(gp) })
    }
}

/// process events
/// @param gp - pointer to gp
#[no_mangle]
pub extern "C" fn ceammc_hw_gamepad_process_events(gp: *mut hw_gamepad) {
    if gp.is_null() {
        log::error!("NULL gamepad pointer");
        return;
    }

    let gp = unsafe { &mut *gp };

    while let Ok(reply) = gp.reply_rx.try_recv() {
        match reply {
            Ok(data) => match data {
                ReplyData::Info(info) => gp.on_devinfo.exec(&info),
                ReplyData::Event(ev) => gp.on_event.exec(&ev),
            },
            Err(err) => gp.on_err.exec_cstr(&err),
        }
    }
}

/// list connected devices
#[no_mangle]
pub extern "C" fn ceammc_hw_gamepad_list_devices(gp: *mut hw_gamepad) -> bool {
    if gp.is_null() {
        log::error!("NULL gamepad pointer");
        return false;
    }

    let gp = unsafe { &mut *gp };

    if let Err(err) = gp.req_tx.try_send(GamepadRequest::ListDevices) {
        gp.on_err
            .exec(format!("[owner] send error: {err}").as_str());
        return false;
    }

    true
}
