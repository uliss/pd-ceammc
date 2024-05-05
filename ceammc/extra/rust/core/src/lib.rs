pub mod mdns_sd;
pub mod net_iface;

use env_logger;

use std::sync::Once;
static LOG_INIT: Once = Once::new();


/// init rust env_logger
/// logger config is done with a RUST_LOG env variable
#[no_mangle]
pub extern "C" fn ceammc_rust_log_init() {
    LOG_INIT.call_once(|| {
        env_logger::builder().format_timestamp(None).init();
    });
}