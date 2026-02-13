use regex::Regex;
use std::ffi::{c_char, c_void, CStr, CString};

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

    fn exec_cstr(&self, msg: &CStr) {
        self.cb.map(|f| f(self.user, msg.as_ptr()));
    }
}

#[no_mangle]
/// create new regexp
/// @param re - regexp
/// @param on_err - error callback
pub extern "C" fn ceammc_regexp_create(re: *const c_char, on_err: regexp_cb_err) -> *mut regexp {
    let inner_fn = || -> Result<*mut regexp, CString> {
        if re.is_null() {
            return Err(CString::new("Null str pointer").unwrap_or_default());
        }

        let str = unsafe { CStr::from_ptr(re) }
            .to_str()
            .map_err(|err| CString::new(err.to_string()).unwrap_or_default())?;

        let re =
            regexp::new(str).map_err(|err| CString::new(err.to_string()).unwrap_or_default())?;
        return Ok(Box::into_raw(Box::new(re)));
    };

    match inner_fn() {
        Ok(ptr) => ptr,
        Err(err) => {
            on_err.exec_cstr(err.as_c_str());
            std::ptr::null_mut()
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

#[no_mangle]
/// get regexp str
/// @param regexp - pointer to regexp struct created with ceammc_regexp_create()
/// @param user - user pointer passed into get callback, useful with pointers to objects
/// @param get_cb - get callback
pub extern "C" fn ceammc_regexp_get_str(
    re: *const regexp,
    user: *mut c_void,
    get_cb: Option<extern "C" fn(user: *mut c_void, str: *const c_char, len: usize)>,
) -> bool {
    if re.is_null() {
        return false;
    } else if let Some(cb) = get_cb {
        let pattern = unsafe { &*re }.re.as_str();
        if let Ok(cstr) = CString::new(pattern) {
            cb(user, cstr.as_ptr(), cstr.count_bytes());
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

#[no_mangle]
/// set regexp str
/// @param regexp - pointer to regexp struct created with ceammc_regexp_create()
/// @param str - new regep
pub extern "C" fn ceammc_regexp_set_str(
    re: *mut regexp,
    str: *const c_char,
    on_err: regexp_cb_err,
) -> bool {
    let inner_fn = || -> Result<(), CString> {
        if re.is_null() {
            return Err(CString::new("Null regexp pointer").unwrap_or_default());
        } else if !str.is_null() {
            let cstr = unsafe { CStr::from_ptr(str) };
            let str = cstr
                .to_str()
                .map_err(|err| CString::new(err.to_string()).unwrap_or_default())?;
            let re = unsafe { &mut *re };
            re.re =
                Regex::new(str).map_err(|err| CString::new(err.to_string()).unwrap_or_default())?;
            return Ok(());
        } else {
            return Err(CString::new("Null str pointer").unwrap_or_default());
        }
    };

    if let Err(err) = inner_fn() {
        on_err.exec_cstr(err.as_c_str());
        return false;
    } else {
        return true;
    }
}
