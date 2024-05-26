pub mod freesound;
pub mod http_req;
pub mod mqtt;
pub mod service;
pub mod telegram_bot;
pub mod utils;
pub mod ws_cli;
pub mod ws_srv;

use env_logger;

#[allow(non_camel_case_types)]
#[no_mangle]
pub extern "C" fn ceammc_net_logger_init() {
    let _ = env_logger::try_init();
}
