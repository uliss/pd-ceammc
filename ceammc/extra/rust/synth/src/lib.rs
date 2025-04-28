#[allow(non_camel_case_types)]
#[repr(C)]
pub struct synth_dx7_sysex {}

#[no_mangle]
pub extern "C" fn ceammc_synth_dx7_sysex_new() -> *mut synth_dx7_sysex {
    Box::into_raw(Box::new(synth_dx7_sysex {}))
}

#[no_mangle]
pub extern "C" fn ceammc_synth_dx7_sysex_free(dx7: *mut synth_dx7_sysex) {
    if !dx7.is_null() {
        let _ = unsafe { Box::from_raw(dx7) };
    }
}
