#ifndef ceammc_rust_core_h
#define ceammc_rust_core_h

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdint>
#include <cstddef>
struct ceammc_mdns;

enum class ceammc_mdns_iface {
  ANY,
  V4,
  V6,
};

enum class ceammc_mdns_rc {
  Ok,
  /// when NULL service pointer given
  NullService,
  ServiceError,
  InvalidString,
  BrowseFailed,
  SetOptionError,
  ServiceNotFound,
};

struct ceammc_mdns_cb_err {
  void *user;
  void (*cb)(void *user, const char *msg);
};

struct ceammc_mdns_cb_srv {
  void *user;
  void (*cb)(void *user, const char *ty, const char *fullname, bool found);
};

struct ceammc_mdns_ip_addr {
  const char *addr;
  bool is_ipv4;
};

struct ceammc_mdns_txt_prop {
  const char *key;
  const char *value;
};

struct ceammc_mdns_service_info {
  /// service type, for ex.: _osc._udp.local.
  const char *stype;
  const char *fullname;
  const char *hostname;
  /// service port
  uint16_t port;
  uint32_t host_ttl;
  uint32_t other_ttl;
  uint16_t priority;
  uint16_t weight;
  /// pointer to array of ip addresses
  const ceammc_mdns_ip_addr *ip;
  /// number of service ip addresses
  size_t ip_len;
  /// pointer to array of txt properties
  const ceammc_mdns_txt_prop *txt;
  /// number of txt properties
  size_t txt_len;
};

struct ceammc_mdns_cb_resolv {
  void *user;
  void (*cb)(void *user, const ceammc_mdns_service_info *info);
};

/// `ty_domain` is the service type and the domain label, for example
/// "_my-service._udp.local.".
///
/// `my_name` is the instance name, without the service type suffix.
///
/// `host_name` is the "host" in the context of DNS. It is used as the "name"
/// in the address records (i.e. TYPE_A and TYPE_AAAA records). It means that
/// for the same hostname in the same local network, the service resolves in
/// the same addresses. Be sure to check it if you see unexpected addresses resolved.
struct ceammc_mdns_service_info_register {
  /// service type, for example "_my-service._udp", or "_my-service._udp.local.".
  const char *service;
  /// the instance name, without the service type suffix, for example "My Service v1.1"
  const char *name;
  /// is the "host" in the context of DNS. It is used as the "name"
  /// in the address records (i.e. TYPE_A and TYPE_AAAA records). It means that
  /// for the same hostname in the same local network, the service resolves in
  /// the same addresses. Be sure to check it if you see unexpected addresses resolved.
  const char *host;
  /// service port
  uint16_t port;
  /// pointer to array of txt properties
  const ceammc_mdns_txt_prop *txt;
  /// number of txt properties
  size_t txt_len;
  /// network interface to listen
  ceammc_mdns_iface iface;
};

struct ceammc_net_ifaces {
  ceammc_Vec<ceammc_NetworkInterface> ifa;
};

struct ceammc_net_err_cb {
  void *user;
  void (*cb)(void *user, const char *msg);
};


extern "C" {

/// create new MDNS service handler
/// @param on_err - error callback
/// @param on_srv - on service found/remove
/// @return pointer to MDNS service or NULL on error
ceammc_mdns *ceammc_mdns_create(ceammc_mdns_cb_err on_err,
                                ceammc_mdns_cb_srv on_srv,
                                ceammc_mdns_cb_resolv on_resolv);

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
ceammc_mdns_rc ceammc_mdns_enable_iface(ceammc_mdns *mdns, const char *name);

/// free mdns handler
/// @param mdns - pointer to mdns struct created with ceammc_mdns_create()
void ceammc_mdns_free(ceammc_mdns *mdns);

/// unsubscribe from mdns service
/// @param mdns - pointer to mdns
/// @return mdns_rc
ceammc_mdns_rc ceammc_mdns_process_events(ceammc_mdns *mdns, uint64_t timeout_ms);

/// register mdns service
/// @param mdns - mdns service pointer
/// @param info - service info pointer
/// @return mdns_rc
ceammc_mdns_rc ceammc_mdns_register(ceammc_mdns *mdns,
                                    const ceammc_mdns_service_info_register *info);

/// subscribe to mdns service events
/// @param mdns - pointer to mdns
/// @param service: mdns service name
/// @return mdns_rc
/// @note can block up to 10ms in case of eagain
ceammc_mdns_rc ceammc_mdns_subscribe(ceammc_mdns *mdns, const char *service);

/// unregister MDNS service
/// @note can block timeout_ms on eagain socket error
/// @param mdns - mdns handle
/// @param name - instance name
/// @param service - mdns service type
/// @param timeout_ms - timeout for unregister in milliseconds
/// @return mdns_rc::Ok on success and other codes or error
ceammc_mdns_rc ceammc_mdns_unregister(ceammc_mdns *mdns,
                                      const char *name,
                                      const char *service,
                                      uint64_t timeout_ms);

/// unsubscribe from mdns service
/// @param mdns - pointer to mdns
/// @param service name
/// @return mdns_rc
ceammc_mdns_rc ceammc_mdns_unsubscribe(ceammc_mdns *mdns, const char *service);

/// free network interface list
/// @param ifl - pointer to list
void ceammc_net_free_interfaces(ceammc_net_ifaces *ifl);

/// return pointer to network interface list
/// you should free it with ceammc_net_free_interfaces
/// @param err_cb - pointer to error callbacks (can be NULL)
/// @return pointer to interface list or NULL on error
ceammc_net_ifaces *ceammc_net_list_interfaces(const ceammc_net_err_cb *err_cb);

} // extern "C"

#endif // ceammc_rust_core_h
