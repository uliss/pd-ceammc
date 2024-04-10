pub mod http_req;
pub mod mqtt;
pub mod service;
pub mod telegram_bot;
pub mod ws_cli;
pub mod ws_srv;

use env_logger;

#[no_mangle]
pub extern "C" fn ceammc_init_rust_logging() {
    env_logger::builder().format_timestamp(None).init();
}