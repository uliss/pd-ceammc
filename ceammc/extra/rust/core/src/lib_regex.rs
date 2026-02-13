use regex::Regex;
use std::{
    ffi::{c_char, c_void, CStr, CString},
    str::FromStr,
};

#[allow(non_camel_case_types)]
pub struct regexp {
    re: Regex,
}

impl regexp {
    pub fn new(re: &str) -> Result<regexp, String> {
        match Regex::new(format!(r"\A{re}\z").as_str()) {
            Ok(re) => Ok(regexp { re }),
            Err(err) => Err(err.to_string()),
        }
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct regexp_cb_err {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, msg: *const c_char)>,
}

impl regexp_cb_err {
    fn exec(&self, msg: &str) {
        let msg = CString::new(msg).unwrap_or_default();
        self.cb.map(|f| f(self.user, msg.as_ptr()));
    }
}

#[no_mangle]
/// create new regexp
/// @param re - regexp
/// @param on_err - error callback
pub extern "C" fn ceammc_regexp_create(re: *const c_char, on_err: regexp_cb_err) -> *mut regexp {
    match unsafe { CStr::from_ptr(re) }.to_str() {
        Ok(str) => match String::from_str(str) {
            Ok(str) => {
                return match regexp::new(str.as_str()) {
                    Ok(re) => Box::into_raw(Box::new(re)),
                    Err(err) => {
                        on_err.exec(err.to_string().as_str());
                        return std::ptr::null_mut();
                    }
                };
            }
            Err(err) => {
                on_err.exec(err.to_string().as_str());
                return std::ptr::null_mut();
            }
        },
        Err(err) => {
            on_err.exec(err.to_string().as_str());
            return std::ptr::null_mut();
        }
    }
}

#[no_mangle]
/// free regexp
/// @param regexp - pointer to regexp struct created with ceammc_regexp_create()
pub extern "C" fn ceammc_regexp_free(re: *mut regexp) {
    if !re.is_null() {
        let _ = unsafe { Box::from_raw(re) };
    }
}

#[no_mangle]
/// @param regexp - pointer to regexp struct created with ceammc_regexp_create()
/// @param re - regexp
/// @param on_err - error callback
pub extern "C" fn ceammc_regexp_is_match(
    re: *const regexp,
    msg: *const c_char,
    on_err: regexp_cb_err,
) -> bool {
    if re.is_null() {
        return false;
    }

    let re = unsafe { &*re };

    match unsafe { CStr::from_ptr(msg) }.to_str() {
        Ok(str) => {
            return re.re.is_match(str);
        }
        Err(err) => {
            on_err.exec(err.to_string().as_str());
            return false;
        }
    }
}
