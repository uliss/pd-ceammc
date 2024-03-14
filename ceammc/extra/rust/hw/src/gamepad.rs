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
    pub struct hw_gamepad {
        gilrs: Gilrs,
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
        data: u8,
        state: hw_gamepad_powerstate,
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

    #[no_mangle]
    pub extern "C" fn ceammc_rs_hw_gamepad_new(
        user_data: *mut c_void,
        err_cb: Option<extern "C" fn(*mut c_void, *const c_char)>,
    ) -> *mut hw_gamepad {
        match Gilrs::new() {
            Ok(gp) => Box::into_raw(Box::new(hw_gamepad { gilrs: gp })),
            Err(err) => {
                err_cb.map(|cb| {
                    let str = CString::new(err.to_string()).unwrap();
                    cb(user_data, str.as_ptr());
                });

                std::ptr::null_mut()
            }
        }
    }

    #[no_mangle]
    pub extern "C" fn ceammc_rs_hw_gamepad_free(gp: *mut hw_gamepad) {
        if !gp.is_null() {
            drop(unsafe { Box::from_raw(gp) })
        }
    }

    #[no_mangle]
    pub extern "C" fn ceammc_rs_hw_gamepad_runloop(
        gp: *mut hw_gamepad,
        cb: Option<extern "C" fn(user_data: *mut c_void, event: &hw_gamepad_event)>,
        user_data: *mut c_void,
    ) -> hw_gamepad_rc {
        if gp.is_null() {
            return hw_gamepad_rc::InvalidHandle;
        }

        let gp = unsafe { &mut *gp };

        while let Some(event) = gp
            .gilrs
            .next_event_blocking(Some(Duration::from_millis(100)))
        {
            println!("{:?}", event);
            let ev = hw_gamepad_event::new(event.id.into(), event.event);
            cb.map(|f| f(user_data, &ev));
        }

        hw_gamepad_rc::Ok
    }

    #[no_mangle]
    pub extern "C" fn ceammc_rs_hw_gamepad_list(
        gp: *mut hw_gamepad,
        cb: Option<
            extern "C" fn(
                user_data: *mut c_void,
                name: *const c_char,
                id: usize,
                vid: u16,
                pid: u16,
                is_connected: bool,
                power: &hw_gamepad_powerinfo,
            ),
        >,
        user_data: *mut c_void,
    ) -> hw_gamepad_rc {
        if gp.is_null() {
            return hw_gamepad_rc::InvalidHandle;
        }

        let gp = unsafe { &*gp };

        cb.map(|cb| {
            for (_id, gamepad) in gp.gilrs.gamepads() {
                let name = CString::new(gamepad.name()).unwrap_or_default();
                let id = gamepad.id().into();
                let vid = gamepad.vendor_id().unwrap_or(0);
                let pid = gamepad.product_id().unwrap_or(0);
                let conn = gamepad.is_connected();
                let power = hw_gamepad_powerinfo::new(gamepad.power_info());

                cb(user_data, name.as_ptr(), id, vid, pid, conn, &power);

                println!("{} is {:?}", gamepad.name(), gamepad.power_info());
            }
        });

        hw_gamepad_rc::Ok
    }
}
