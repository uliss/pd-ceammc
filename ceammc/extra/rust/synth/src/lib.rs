use std::ffi::{c_char, c_void, CStr, CString};

use synthahol_dx7::{Bank, Preset};

#[repr(C)]
#[allow(non_camel_case_types)]
enum synth_msg_level {
    DEBUG,
    POST,
    ERROR,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct synth_msg_cb {
    /// nullable
    user: *mut c_void,
    /// not NULL!                                            
    cb: extern "C" fn(*mut c_void, synth_msg_level, *const c_char),
}

impl synth_msg_cb {
    fn exec_str(&self, level: synth_msg_level, msg: &str) {
        let msg = CString::new(msg).unwrap_or_default();
        (self.cb)(self.user, level, msg.as_ptr());
    }
}

#[allow(non_camel_case_types)]
pub type synth_dx7_main_cb = extern "C" fn(
    user: *mut c_void,
    name: *const c_char,
    algorithm_id: u8,
    oscillator_key_sync: bool,
    feedback_level: u8,
    transpose: u8,
);

#[allow(non_camel_case_types)]
pub type synth_dx7_lfo_cb = extern "C" fn(
    user: *mut c_void,
    lfo_speed: u8,
    lfo_delay: u8,
    lfo_pitch_mod_depth: u8,
    lfo_pitch_mod_sensitivity: u8,
    lfo_amplitude_mod_depth: u8,
    lfo_waveform: u8,
    lfo_key_sync: bool,
);

#[allow(non_camel_case_types)]
pub type synth_dx7_env_cb = extern "C" fn(user: *mut c_void, levels: [u8; 4], rates: [u8; 4]);

#[allow(non_camel_case_types)]
pub type synth_dx7_op_cb = extern "C" fn(
    user: *mut c_void,
    index: usize, 
    levels: *const u8,
    rates: *const u8,
    scaling_break_point: u8,
    scaling_left_depth: u8,
    scaling_right_depth: u8,
    scaling_left_curve: u8,
    scaling_right_curve: u8,
    // -7 to 7. Stored as 0-14 in the preset.
    detune: i8,
    rate_scaling: u8,
    velocity_sensitivity: u8,
    modulation_sensitivity: u8,
    output_level: u8,
    is_fixed: u8,
    frequency_course: u8,
    frequency_fine: u8,
);

#[allow(non_camel_case_types)]
pub struct synth_dx7_sysex {
    msg_cb: synth_msg_cb,
    presets: Vec<Preset>,
}

impl synth_dx7_sysex {
    pub fn new(msg_cb: synth_msg_cb) -> Self {
        Self {
            msg_cb,
            presets: vec![],
        }
    }

    pub fn read_file(&mut self, path: *const c_char) -> bool {
        if path.is_null() {
            self.msg_cb
                .exec_str(synth_msg_level::ERROR, "Null path pointer");
            return false;
        }

        let path = &*unsafe { CStr::from_ptr(path) }.to_string_lossy();
        match Bank::read_file(path) {
            Ok(res) => {
                self.msg_cb
                    .exec_str(synth_msg_level::DEBUG, format!("read {path}").as_str());
                self.presets = res;
                true
            }
            Err(err) => {
                self.msg_cb
                    .exec_str(synth_msg_level::ERROR, err.to_string().as_str());
                false
            }
        }
    }

    pub fn print_info(&self) {
        self.msg_cb.exec_str(
            synth_msg_level::POST,
            format!("total: {}", self.presets.len()).as_str(),
        );

        for (i, p) in self.presets.iter().enumerate() {
            let info = format!("\t[{i}] name: {}\n", p.name);
            self.msg_cb.exec_str(synth_msg_level::POST, info.as_str());
        }
    }

    fn load_preset(
        &self,
        preset: &Preset,
        user: *mut c_void,
        main_cb: synth_dx7_main_cb,
        lfo_cb: synth_dx7_lfo_cb,
        env_cb: synth_dx7_env_cb,
        ops_cb: synth_dx7_op_cb,
    ) -> bool {
        let name = CString::new(preset.name.to_string()).unwrap_or_default();

        (main_cb)(
            user,
            name.as_ptr(),
            preset.algorithm_id as u8,
            preset.oscillator_key_sync,
            preset.feedback_level,
            preset.transpose,
        );

        (lfo_cb)(
            user,
            preset.lfo_speed,
            preset.lfo_delay,
            preset.lfo_pitch_mod_depth,
            preset.lfo_pitch_mod_sensitivity,
            preset.lfo_amplitude_mod_depth,
            preset.lfo_waveform as u8,
            preset.lfo_key_sync,
        );

        (env_cb)(
            user,
            preset.pitch_envelope.levels,
            preset.pitch_envelope.rates,
        );

        for (i, op) in preset.operators.iter().enumerate() {
            ops_cb(
                user,
                i,
                op.envelope.levels.as_ptr(),
                op.envelope.rates.as_ptr(),
                op.scaling_break_point,
                op.scaling_left_depth,
                op.scaling_right_depth,
                op.scaling_left_curve,
                op.scaling_right_curve,
                op.detune,
                op.rate_scaling,
                op.velocity_sensitivity,
                op.modulation_sensitivity,
                op.output_level,
                op.mode as u8,
                op.frequency_course,
                op.frequency_fine,
            )
        }

        true
    }

    pub fn load_preset_by_index(
        &self,
        idx: u8,
        user: *mut c_void,
        main_cb: synth_dx7_main_cb,
        lfo_cb: synth_dx7_lfo_cb,
        env_cb: synth_dx7_env_cb,
        ops_cb: synth_dx7_op_cb,
    ) -> bool {
        match self.presets.get(idx as usize) {
            Some(p) => self.load_preset(p, user, main_cb, lfo_cb, env_cb, ops_cb),
            None => {
                self.msg_cb.exec_str(
                    synth_msg_level::ERROR,
                    format!("preset not found: [{idx}]").as_str(),
                );
                return false;
            }
        }
    }
}

#[no_mangle]
pub extern "C" fn ceammc_synth_dx7_sysex_new(msg_cb: synth_msg_cb) -> *mut synth_dx7_sysex {
    Box::into_raw(Box::new(synth_dx7_sysex::new(msg_cb)))
}

#[no_mangle]
pub extern "C" fn ceammc_synth_dx7_sysex_read_file(
    dx: *mut synth_dx7_sysex,
    file: *const c_char,
) -> bool {
    if dx.is_null() {
        return false;
    } else {
        let dx = unsafe { &mut *dx };
        dx.read_file(file)
    }
}

#[no_mangle]
pub extern "C" fn ceammc_synth_dx7_sysex_info(dx: *mut synth_dx7_sysex) -> bool {
    if dx.is_null() {
        return false;
    } else {
        let dx = unsafe { &mut *dx };
        if dx.presets.is_empty() {
            dx.msg_cb.exec_str(synth_msg_level::ERROR, "no presets");
            return false;
        }

        dx.print_info();
        true
    }
}

#[no_mangle]
pub extern "C" fn ceammc_synth_dx7_sysex_load(
    dx: *mut synth_dx7_sysex,
    idx: u8,
    user: *mut c_void,
    main_cb: synth_dx7_main_cb,
    lfo_cb: synth_dx7_lfo_cb,
    env_cb: synth_dx7_env_cb,
    ops_cb: synth_dx7_op_cb,
) -> bool {
    if dx.is_null() {
        return false;
    } else {
        let dx = unsafe { &mut *dx };
        dx.load_preset_by_index(idx, user, main_cb, lfo_cb, env_cb, ops_cb)
    }
}

#[no_mangle]
pub extern "C" fn ceammc_synth_dx7_sysex_free(dx7: *mut synth_dx7_sysex) {
    if !dx7.is_null() {
        let _ = unsafe { Box::from_raw(dx7) };
    }
}
