pub mod http_req;
pub mod mqtt;
pub mod service;
pub mod telegram_bot;
pub mod ws_cli;
pub mod ws_srv;

use env_logger;

#[no_mangle]
pub extern "C" fn ceammc_net_init_logging() {
    env_logger::init();
}
