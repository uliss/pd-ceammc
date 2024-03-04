use std::ffi::{c_char, CStr, CString};
use std::net::IpAddr;
use std::os::raw::c_void;
use std::str::FromStr;
use std::time::Duration;

use mdns_sd::{ServiceDaemon, ServiceEvent};

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

    if service_type.is_null() {
        return mdns_rc::InvalidServiceType;
    }

    let service_type = unsafe { CStr::from_ptr(service_type) }.to_str();
    if service_type.is_err() {
        return mdns_rc::Utf8Error;
    }

    let mut service_type = service_type.unwrap().to_owned();
    if !service_type.ends_with(".local.") {
        if service_type.ends_with(".local") {
            service_type += ".";
        } else {
            service_type += ".local.";
        }
    }

    // Browse for a service type.
    let receiver = mdns.browse(&service_type);
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
