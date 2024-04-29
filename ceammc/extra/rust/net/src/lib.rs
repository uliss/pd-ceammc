pub mod http_req;
pub mod mqtt;
pub mod service;
pub mod telegram_bot;
pub mod ws_cli;
pub mod ws_srv;
pub mod freesound;
pub mod utils;

use env_logger;

use std::sync::Once;
static LOG_INIT: Once = Once::new();

#[no_mangle]
pub extern "C" fn ceammc_init_rust_logging() {
    LOG_INIT.call_once(|| {
        env_logger::builder().format_timestamp(None).init();
    });
}
