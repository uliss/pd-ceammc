use network_interface::{NetworkInterface, NetworkInterfaceConfig};
use std::{ffi::{c_char, CString}, os::raw::c_void, ptr::null_mut};

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct net_err_cb {
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, msg: *const c_char)>,
}

impl net_err_cb {
    fn exec(&self, msg: &str) {
        let msg = CString::new(msg).unwrap_or_default();
        self.cb.map(|f| f(self.user, msg.as_ptr()));
    }
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct net_ifaces {
    ifa: Vec<NetworkInterface>,
}

#[no_mangle]
/// return pointer to network interface list
/// you should free it with ceammc_net_free_interfaces
/// @param err_cb - pointer to error callbacks (can be NULL)
/// @return pointer to interface list or NULL on error
pub extern "C" fn ceammc_net_list_interfaces(err_cb: *const net_err_cb) -> *mut net_ifaces {
    match NetworkInterface::show() {
        Ok(ifa) => Box::into_raw(Box::new(net_ifaces { ifa })),
        Err(err) => {
            if !err_cb.is_null() {
                let err_cb = unsafe { &*err_cb };
                err_cb.exec(format!("{err:?}").as_str());
            }
            null_mut()
        }
    }
}

#[no_mangle]
/// free network interface list
/// @param ifl - pointer to list
pub extern "C" fn ceammc_net_free_interfaces(ifl: *mut net_ifaces) {
    if !ifl.is_null() {
        drop(unsafe { Box::from_raw(ifl) })
    }
}
