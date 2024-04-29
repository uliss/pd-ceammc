mod gamepad {
    use std::{
        ffi::CString,
        os::raw::{c_char, c_void},
        time::Duration,
    };

    use gilrs::{Axis, Button, EventType, Gilrs, PowerInfo};

    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub enum hw_gamepad_rc {
        Ok = 0,
        InvalidHandle,
    }

    #[derive(Default)]
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

    #[derive(Default)]
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

    #[derive(Default)]
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

    #[derive(Default)]
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

    #[allow(non_camel_case_types)]
    /// gamepad opaque type
    pub struct hw_gamepad {
        gilrs: Gilrs,
        on_event: gamepad_event_cb,
        on_devinfo: gamepad_listdev_cb,
    }

    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub enum hw_gamepad_powerstate {
        Unknown = 0,
        Wired,
        Discharging,
        Charging,
        Charged,
    }

    #[allow(non_camel_case_types)]
    #[repr(C)]
    pub struct hw_gamepad_powerinfo {
        state: hw_gamepad_powerstate,
        data: u8,
    }

    impl hw_gamepad_powerinfo {
        fn new(info: PowerInfo) -> Self {
            match info {
                PowerInfo::Unknown => Self::_new(0, hw_gamepad_powerstate::Unknown),
                PowerInfo::Wired => Self::_new(0, hw_gamepad_powerstate::Wired),
                PowerInfo::Discharging(data) => {
                    Self::_new(data, hw_gamepad_powerstate::Discharging)
                }
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

    /// create new gamepad
    /// @param err_cb - error callback
    /// @param event_cb - gamepad event callback
    /// @param on_devinfo - gamepad list connected devices callback
    /// @return pointer to new gamepad or NULL on error
    #[no_mangle]
    pub extern "C" fn ceammc_hw_gamepad_new(
        on_err: gamepad_err_cb,
        on_event: gamepad_event_cb,
        on_devinfo: gamepad_listdev_cb,
    ) -> *mut hw_gamepad {
        match Gilrs::new() {
            Ok(gp) => Box::into_raw(Box::new(hw_gamepad {
                gilrs: gp,
                on_event,
                on_devinfo,
            })),
            Err(err) => {
                on_err.cb.map(|cb| {
                    let str = CString::new(err.to_string()).unwrap();
                    cb(on_err.user, str.as_ptr());
                });

                std::ptr::null_mut()
            }
        }
    }

    /// free gamepad
    #[no_mangle]
    pub extern "C" fn ceammc_hw_gamepad_free(gp: *mut hw_gamepad) {
        if !gp.is_null() {
            drop(unsafe { Box::from_raw(gp) })
        }
    }

    /// process gamepad events (blocking read)
    /// @param gp - gamepad pointer
    /// @param time_ms - event read timeout in milliseconds
    /// @return ceammc_hw_gamepad_rc
    #[no_mangle]
    pub extern "C" fn ceammc_hw_gamepad_process_events(
        gp: *mut hw_gamepad,
        time_ms: u64,
    ) -> hw_gamepad_rc {
        if gp.is_null() {
            return hw_gamepad_rc::InvalidHandle;
        }

        let gp = unsafe { &mut *gp };

        while let Some(event) = gp
            .gilrs
            .next_event_blocking(Some(Duration::from_millis(time_ms)))
        {
            let ev = hw_gamepad_event::new(event.id.into(), event.event);
            gp.on_event.exec(&ev);
        }

        hw_gamepad_rc::Ok
    }

    /// list connected gamepad devices
    /// @param gp - gamepad pointer
    #[no_mangle]
    pub extern "C" fn ceammc_hw_gamepad_list(gp: *mut hw_gamepad) -> hw_gamepad_rc {
        if gp.is_null() {
            return hw_gamepad_rc::InvalidHandle;
        }

        let gp = unsafe { &*gp };

        gp.on_devinfo.cb.map(|cb| {
            for (gid, gamepad) in gp.gilrs.gamepads() {
                let name = CString::new(gamepad.name()).unwrap_or_default();
                let os_name = CString::new(gamepad.os_name()).unwrap_or_default();
                let id = gid.into();
                let vid = gamepad.vendor_id().unwrap_or(0);
                let pid = gamepad.product_id().unwrap_or(0);
                let is_connected = gamepad.is_connected();
                let power = hw_gamepad_powerinfo::new(gamepad.power_info());

                cb(
                    gp.on_devinfo.user,
                    &gamepad_dev_info {
                        name: name.as_ptr(),
                        os_name: os_name.as_ptr(),
                        id,
                        vid,
                        pid,
                        is_connected,
                        has_ff: gamepad.is_ff_supported(),
                        power,
                    },
                );
            }
        });

        hw_gamepad_rc::Ok
    }
}
