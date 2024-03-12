use std::ffi::{CStr, CString};
use std::net::IpAddr;
use std::os::raw::c_char;
use std::os::raw::c_void;
use std::str::FromStr;
use std::time::Duration;

use mdns_sd::{Error, ServiceDaemon, ServiceEvent, ServiceInfo, UnregisterStatus};

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct mdns_txt_property {
    key: *const c_char,
    value: *const c_char,
}

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct mdns_service_info {
    /// service port
    port: u16,
    /// service name
    fullname: *const c_char,
    /// service hostname
    hostname: *const c_char,
    /// service type, for ex.: _osc._udp.local.
    rtype: *const c_char,
    /// number of service ip addresses
    ip_count: usize,
    /// pointer to array of ip addresses
    ip: *const *const c_char,
    /// number of txt properties
    txt_prop_count: usize,
    /// pointer to array of txt properties
    txt_props: *const mdns_txt_property,
}

impl mdns_service_info {
    fn new(
        fullname: &CStr,
        hostname: &CStr,
        port: u16,
        rtype: &CStr,
        ip: &Vec<*const c_char>,
        props: &Vec<mdns_txt_property>,
    ) -> Self {
        mdns_service_info {
            port: port,
            fullname: fullname.as_ptr(),
            hostname: hostname.as_ptr(),
            rtype: rtype.as_ptr(),
            ip_count: ip.len(),
            ip: ip.as_ptr(),
            txt_prop_count: props.len(),
            txt_props: props.as_ptr(),
        }
    }
}

impl mdns_txt_property {
    fn new(key: &CStr, value: &CStr) -> Self {
        mdns_txt_property {
            key: key.as_ptr(),
            value: value.as_ptr(),
        }
    }
}

#[allow(non_camel_case_types)]
pub struct mdns {
    data: Result<ServiceDaemon, mdns_sd::Error>,
}

impl mdns {
    fn new() -> Self {
        mdns {
            data: ServiceDaemon::new(),
        }
    }
}

#[derive(Debug)]
#[allow(non_camel_case_types)]
#[repr(C)]
pub enum mdns_rc {
    Ok,
    NullService,
    ServiceError,
    InvalidServiceType,
    Utf8Error,
    BrowseFailed,
    InvalidStringPointer,
    SetOptionError,
    ServiceNotFound,
}

#[no_mangle]
/// create new MDNS service handler
/// @return pointer to MDNS service or NULL on error
pub extern "C" fn ceammc_rs_mdns_create() -> *mut mdns {
    let srv = mdns::new();
    if srv.data.is_err() {
        std::ptr::null_mut()
    } else {
        let srv = Box::new(srv);
        Box::into_raw(srv)
    }
}

fn name_to_iface(name: &str) -> mdns_sd::IfKind {
    let ifn = match name {
        "all" => mdns_sd::IfKind::All,
        "*" => mdns_sd::IfKind::All,
        "ipv4" => mdns_sd::IfKind::IPv4,
        "ipv6" => mdns_sd::IfKind::IPv6,
        _ => match IpAddr::from_str(name) {
            Ok(addr) => mdns_sd::IfKind::Addr(addr),
            _ => mdns_sd::IfKind::Name(name.to_string()),
        },
    };
    ifn
}

#[no_mangle]
/// enable/disable network interfaces to search for mdns services
/// @param mdns - pointer to mdns struct
/// @param name - network interface name, for example:
///  - '*', 'all' (all network interfaces)
///  - 'ipv4' (all ip v4 interfaces)
///  - 'ipv6' (all ip v6 interfaces)
///  - '127.0.0.1' etc (interface ip address)
///  - 'lo0', 'eth1' (interface name)
///
/// @note '!' can't be added to disable interface, for ex.: '!ipv6'
/// @return mdns_rc::Ok on success or other codes on error
pub extern "C" fn ceammc_rs_mdns_enable_iface(mdns: *mut mdns, name: *const c_char) -> mdns_rc {
    if mdns.is_null() {
        return mdns_rc::NullService;
    }

    let mdns = unsafe { &*mdns };
    if !mdns.data.is_ok() {
        return mdns_rc::ServiceError;
    }

    let mdns = mdns.data.as_ref().unwrap();
    if name.is_null() {
        return mdns_rc::InvalidStringPointer;
    }

    let ifn = unsafe { CStr::from_ptr(name) }.to_str();
    if ifn.is_err() {
        return mdns_rc::Utf8Error;
    }
    let ifn = ifn.unwrap();
    if ifn.starts_with("!") {
        let ifn = name_to_iface(&ifn[1..]);
        // println!("disable iface: {:?}", ifn);
        match mdns.disable_interface(ifn) {
            Ok(_) => mdns_rc::Ok,
            _ => mdns_rc::SetOptionError,
        }
    } else {
        let ifn = name_to_iface(ifn);
        // println!("enable iface: {:?}", ifn);
        match mdns.enable_interface(ifn) {
            Ok(_) => mdns_rc::Ok,
            _ => mdns_rc::SetOptionError,
        }
    }
}

#[no_mangle]
/// free mdns handler
/// @param mdns - pointer to mdns struct created with ceammc_rs_mdns_create()
pub extern "C" fn ceammc_rs_mdns_free(mdns: *mut mdns) {
    if !mdns.is_null() {
        drop(unsafe { Box::from_raw(mdns) })
    }
}

#[no_mangle]
/// converts error code to string
/// @param rc - pointer to mdns struct created with ceammc_rs_mdns_create()
/// @return pointer to error string
pub extern "C" fn ceammc_rs_mdns_strerr(rc: mdns_rc) -> *const c_char {
    match rc {
        mdns_rc::Ok => "ok\0",
        mdns_rc::NullService => "null service pointer\0",
        mdns_rc::ServiceError => "mdns service creation error\0",
        mdns_rc::InvalidServiceType => "invalid service type string\0",
        mdns_rc::Utf8Error => "UTF-8 service type error\0",
        mdns_rc::BrowseFailed => "browse error\0",
        mdns_rc::InvalidStringPointer => "invalid string pointer\0",
        mdns_rc::SetOptionError => "set option error\0",
        mdns_rc::ServiceNotFound => "service not found\0",
    }
    .as_ptr() as *const c_char
}

#[no_mangle]
/// browse mdns services on the network
/// @param mdns - point to mdns handle
/// @param service_type - MDNS service type to search, for ex.: '_http._tcp.local.'.
///        If '.local.' suffix if omitted it will be auto-added.
/// @param timeout - search timeout in seconds
/// @param cb - callback called each time a new service found
/// @param user_data - pointer to user data
/// @return mdns_rc::Ok on success and other codes on error
pub extern "C" fn ceammc_rs_mdns_browse(
    mdns: *mut mdns,
    service_type: *const c_char,
    timeout: u64,
    cb: Option<extern "C" fn(*mut c_void, &mdns_service_info)>,
    user_data: *mut c_void,
) -> mdns_rc {
    if mdns.is_null() {
        return mdns_rc::NullService;
    }

    let mdns = unsafe { &*mdns };
    if !mdns.data.is_ok() {
        return mdns_rc::ServiceError;
    }

    let mdns = mdns.data.as_ref().unwrap();

    let service_type = mdns_full_service_name(service_type);
    if service_type.is_err() {
        return service_type.unwrap_err();
    }

    // Browse for a service type.
    let receiver = mdns.browse(service_type.unwrap().as_str());
    if receiver.is_err() {
        return mdns_rc::BrowseFailed;
    }
    let receiver = receiver.unwrap();

    let now = std::time::Instant::now();
    let deadline = now.checked_add(Duration::new(timeout, 0)).unwrap();
    while let Ok(event) = receiver.recv_deadline(deadline) {
        match event {
            ServiceEvent::ServiceResolved(info) => {
                if cb.is_some() {
                    let cb = cb.unwrap();
                    // let port = info.port;
                    let hostname = CString::new(info.get_hostname()).unwrap();
                    let fullname = CString::new(info.get_fullname()).unwrap();
                    let rtype = CString::new(info.get_type()).unwrap();
                    let ip_str = &Vec::from_iter(
                        info.get_addresses()
                            .into_iter()
                            .map(|x| CString::new(x.to_string()).unwrap()),
                    );
                    let ip_ptr = &Vec::from_iter(ip_str.iter().map(|str| str.as_c_str().as_ptr()));
                    let props_str = Vec::from_iter(info.get_properties().iter().map(|p| {
                        (
                            CString::new(p.key()).unwrap(),
                            CString::new(p.val_str()).unwrap(),
                        )
                    }));
                    let props_ptr =
                        Vec::from_iter(props_str.iter().map(|(k, v)| mdns_txt_property::new(k, v)));

                    let info = mdns_service_info::new(
                        &fullname,
                        &hostname,
                        info.get_port(),
                        &rtype,
                        &ip_ptr,
                        &props_ptr,
                    );
                    cb(user_data, &info);
                }
            }
            _other_event => {
                // println!("At {:?} : {:?}", now.elapsed(), &_other_event);
            }
        }
    }

    mdns_rc::Ok
}

#[no_mangle]
pub extern "C" fn ceammc_rs_mdns_register(
    mdns: *mut mdns,
    service: *const c_char,
    name: *const c_char,
    hostname: *const c_char,
    ip: *const c_char,
    port: u16,
) -> mdns_rc {
    if mdns.is_null() {
        return mdns_rc::NullService;
    }

    let mdns = unsafe { &*mdns };
    if !mdns.data.is_ok() {
        return mdns_rc::ServiceError;
    }

    let mdns = mdns.data.as_ref().unwrap();

    if service.is_null() || name.is_null() || hostname.is_null() || ip.is_null() {
        return mdns_rc::InvalidStringPointer;
    }

    let name = unsafe { CStr::from_ptr(name) }.to_str();
    let hostname = unsafe { CStr::from_ptr(hostname) }.to_str();
    let ip = unsafe { CStr::from_ptr(ip) }.to_str();

    if name.is_err() || hostname.is_err() || ip.is_err() {
        return mdns_rc::Utf8Error;
    }

    let service = mdns_full_service_name(service);
    if service.is_err() {
        return service.unwrap_err();
    }

    let service_info = ServiceInfo::new(
        service.unwrap().as_str(),
        name.unwrap(),
        hostname.unwrap(),
        ip.unwrap(),
        port,
        None,
    );

    if service_info.is_err() {
        return mdns_rc::SetOptionError;
    }

    let rc = mdns.register(service_info.unwrap());
    if rc.is_err() {
        return mdns_rc::ServiceError;
    }

    mdns_rc::Ok
}

fn mdns_full_service_name(name: *const c_char) -> Result<String, mdns_rc> {
    let name = unsafe { CStr::from_ptr(name) }.to_str();
    if name.is_err() {
        return Err(mdns_rc::InvalidStringPointer);
    }

    let mut name = name.unwrap().to_owned();
    if !name.ends_with(".local.") {
        if name.ends_with(".local") {
            name += ".";
        } else {
            name += ".local.";
        }
    };

    Ok(name)
}

#[no_mangle]
/// unregister MDNS service
/// @param mdns - mdns service handle
/// @param service - mdns service name
/// @param timeout - timeout for unregister
/// @return mdns_rc::Ok on success and other codes or error
pub extern "C" fn ceammc_rs_mdns_unregister(
    mdns: *mut mdns,
    service: *const c_char,
    timeout: u64,
) -> mdns_rc {
    if mdns.is_null() {
        return mdns_rc::NullService;
    }

    let mdns = unsafe { &*mdns };
    if !mdns.data.is_ok() {
        return mdns_rc::ServiceError;
    }

    let mdns = mdns.data.as_ref().unwrap();

    match mdns_full_service_name(service) {
        Ok(service) => mdns_unregister(mdns, service.as_str(), timeout, true),
        Err(err) => err,
    }
}

fn mdns_unregister(mdns: &ServiceDaemon, fullname: &str, timeout: u64, again: bool) -> mdns_rc {
    let res = mdns.unregister(fullname);
    if res.is_err() {
        if !again {
            return mdns_rc::ServiceError;
        }

        return match res.unwrap_err() {
            Error::Again => mdns_unregister(mdns, fullname, timeout, again),
            _ => mdns_rc::ServiceError,
        };
    }

    let rc = res.unwrap().recv_timeout(Duration::new(timeout, 0));

    match rc {
        Ok(st) => match st {
            UnregisterStatus::OK => mdns_rc::Ok,
            UnregisterStatus::NotFound => mdns_rc::ServiceNotFound,
        },
        Err(_) => mdns_rc::ServiceError,
    }
}
