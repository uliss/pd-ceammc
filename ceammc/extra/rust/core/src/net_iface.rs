use network_interface::{NetworkInterface, NetworkInterfaceConfig};
use std::{
    ffi::{c_char, CString},
    net::{Ipv4Addr, Ipv6Addr},
    os::raw::c_void,
    ptr::{null, null_mut},
};

pub type N = u16;

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
pub struct net_addr4 {
    addr: Ipv4Addr,
    str: CString,
}

#[allow(non_camel_case_types)]
pub struct net_addr6 {
    addr: Ipv6Addr,
    str: CString,
}

#[allow(non_camel_case_types)]
pub struct net_iface_addr4 {
    ip: net_addr4,
    netmask: Option<net_addr4>,
    broadcast: Option<net_addr4>,
}

#[allow(non_camel_case_types)]
pub struct net_iface_addr6 {
    ip: net_addr6,
    netmask: Option<net_addr6>,
    broadcast: Option<net_addr6>,
}

#[allow(non_camel_case_types)]
pub enum net_iface_addr {
    v4(net_iface_addr4),
    v6(net_iface_addr6),
}

#[allow(non_camel_case_types)]
pub struct net_iface {
    /// Interface's name
    name: CString,
    /// Interface's address
    addr: Vec<net_iface_addr>,
    /// MAC Address
    mac_addr: CString,
    /// Interface's index
    index: u32,
}

#[allow(non_camel_case_types)]
pub struct net_ifaces {
    ifa: Vec<NetworkInterface>,
}

#[no_mangle]
#[must_use]
/// return pointer to network interface list
/// you should free it with ceammc_net_free_interfaces
/// @param err_cb - error callbacks (can be NULL)
/// @return pointer to interface list or NULL on error
pub extern "C" fn ceammc_net_list_interfaces(err_cb: net_err_cb) -> *mut net_ifaces {
    match NetworkInterface::show() {
        Ok(ifa) => Box::into_raw(Box::new(net_ifaces { ifa })),
        Err(err) => {
            err_cb.exec(format!("{err:?}").as_str());
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

#[no_mangle]
pub extern "C" fn ceammc_net_iface_name(ni: &net_iface) -> *const c_char {
    ni.name.as_ptr()
}

#[no_mangle]
pub extern "C" fn ceammc_net_iface_mac(ni: &net_iface) -> *const c_char {
    ni.mac_addr.as_ptr()
}

#[no_mangle]
pub extern "C" fn ceammc_net_iface_index(ni: &net_iface) -> u32 {
    ni.index
}

#[no_mangle]
pub extern "C" fn ceammc_net_iface_get_addr(ni: &net_iface, i: usize) -> *const net_iface_addr {
    &ni.addr[i] as *const net_iface_addr
}

#[no_mangle]
pub extern "C" fn ceammc_net_iface_num_addr(ni: &net_iface) -> usize {
    ni.addr.len()
}

#[no_mangle]
pub extern "C" fn ceammc_net_is_ifa_v4(va: &net_iface_addr) -> bool {
    matches!(va, net_iface_addr::v4(_))
}

#[no_mangle]
pub extern "C" fn ceammc_net_is_ifa_v6(va: &net_iface_addr) -> bool {
    matches!(va, net_iface_addr::v6(_))
}

#[no_mangle]
pub extern "C" fn ceammc_net_as_ifa_v4(va: &net_iface_addr) -> *const net_iface_addr4 {
    match va {
        net_iface_addr::v4(x) => x as *const net_iface_addr4,
        _ => null(),
    }
}

#[no_mangle]
pub extern "C" fn ceammc_net_as_ifa_v6(va: &net_iface_addr) -> *const net_iface_addr6 {
    match va {
        net_iface_addr::v6(x) => x as *const net_iface_addr6,
        _ => null(),
    }
}

/// @param x - non NULL!
/// @return pointer to network interface ip address v4
/// @note always return valid pointer
#[no_mangle]
pub extern "C" fn ceammc_net_ifa_ip4(x: &net_iface_addr4) -> *const net_addr4 {
    &x.ip as *const net_addr4
}

#[no_mangle]
/// @param x - non NULL!
/// return pointer to network interface broadcast address v4
/// @note can return NULL!
pub extern "C" fn ceammc_net_ifa_bcast4(x: &net_iface_addr4) -> *const net_addr4 {
    match &x.broadcast {
        Some(x) => x as *const net_addr4,
        None => null(),
    }
}

#[no_mangle]
/// @param x - non NULL!
/// return pointer to network interface mask address v4
/// @note can return NULL!
pub extern "C" fn ceammc_net_ifa_mask4(x: &net_iface_addr4) -> *const net_addr4 {
    match &x.netmask {
        Some(x) => x as *const net_addr4,
        None => null(),
    }
}

#[no_mangle]
/// @param x - non NULL!
/// @return pointer to network interface ip address v6
/// @note always return valid pointer
pub extern "C" fn ceammc_net_ifa_ip6(x: &net_iface_addr6) -> *const net_addr6 {
    &x.ip as *const net_addr6
}

#[no_mangle]
/// @param x - non NULL!
/// @return pointer to network interface broadcast address v6
/// @note can return NULL!
pub extern "C" fn ceammc_net_ifa_bcast6(x: &net_iface_addr6) -> *const net_addr6 {
    match &x.broadcast {
        Some(x) => x as *const net_addr6,
        None => null(),
    }
}

#[no_mangle]
/// @param x - non NULL!
/// return pointer to network interface mask address v6
/// @note can return NULL!
pub extern "C" fn ceammc_net_ifa_mask6(x: &net_iface_addr6) -> *const net_addr6 {
    match &x.netmask {
        Some(x) => x as *const net_addr6,
        None => null(),
    }
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr4_str(x: &net_addr4) -> *const c_char {
    x.str.as_ptr()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr4_octets(x: &net_addr4) -> *const u8 {
    x.addr.octets().as_ptr()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr4_is_bcast(x: &net_addr4) -> bool {
    x.addr.is_broadcast()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr4_is_doc(x: &net_addr4) -> bool {
    x.addr.is_documentation()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr4_is_link(x: &net_addr4) -> bool {
    x.addr.is_link_local()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr4_is_loop(x: &net_addr4) -> bool {
    x.addr.is_loopback()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr4_is_mcast(x: &net_addr4) -> bool {
    x.addr.is_multicast()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr4_is_priv(x: &net_addr4) -> bool {
    x.addr.is_private()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr4_is_unspec(x: &net_addr4) -> bool {
    x.addr.is_unspecified()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr6_str(x: &net_addr6) -> *const c_char {
    x.str.as_ptr()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr6_octets(x: &net_addr6) -> *const u8 {
    x.addr.octets().as_ptr()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr6_is_loop(x: &net_addr6) -> bool {
    x.addr.is_loopback()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr6_is_mcast(x: &net_addr6) -> bool {
    x.addr.is_multicast()
}

#[no_mangle]
/// @param x - non NULL!
pub extern "C" fn ceammc_net_addr6_is_unspec(x: &net_addr6) -> bool {
    x.addr.is_unspecified()
}

#[no_mangle]
/// free network interface list
/// @param ifl - pointer to list (non-NULL)
pub extern "C" fn ceammc_net_foreach_interfaces(
    ifl: &net_ifaces,
    user: *mut c_void,
    cb: Option<extern "C" fn(user: *mut c_void, ni: &net_iface)>,
) -> usize {
    cb.map(|f| {
        //
        for ni in ifl.ifa.iter() {
            let mut vaddr: Vec<net_iface_addr> = vec![];

            for a in ni.addr.iter() {
                match a {
                    network_interface::Addr::V4(addr) => {
                        vaddr.push(net_iface_addr::v4(net_iface_addr4 {
                            ip: net_addr4 {
                                addr: addr.ip,
                                str: CString::new(addr.ip.to_string()).unwrap_or_default(),
                            },
                            broadcast: addr.broadcast.map(|x| net_addr4 {
                                addr: x,
                                str: CString::new(x.to_string()).unwrap_or_default(),
                            }),
                            netmask: addr.netmask.map(|x| net_addr4 {
                                addr: x,
                                str: CString::new(x.to_string()).unwrap_or_default(),
                            }),
                        }));
                    }
                    network_interface::Addr::V6(addr) => {
                        vaddr.push(net_iface_addr::v6(net_iface_addr6 {
                            ip: net_addr6 {
                                addr: addr.ip,
                                str: CString::new(addr.ip.to_string()).unwrap_or_default(),
                            },
                            netmask: addr.netmask.map(|x| net_addr6 {
                                addr: x,
                                str: CString::new(x.to_string()).unwrap_or_default(),
                            }),
                            broadcast: addr.broadcast.map(|x| net_addr6 {
                                addr: x,
                                str: CString::new(x.to_string()).unwrap_or_default(),
                            }),
                        }));
                    }
                }
            }
            let mac = ni.mac_addr.clone().unwrap_or_default();

            let iface = net_iface {
                name: CString::new(ni.name.clone()).unwrap_or_default(),
                mac_addr: CString::new(mac).unwrap_or_default(),
                addr: vaddr,
                index: ni.index,
            };

            f(user, &iface)
        }
    });

    ifl.ifa.len()
}
