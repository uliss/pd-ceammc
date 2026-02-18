use crate::rust_str_cb;

use log::error;
use whoami;

#[no_mangle]
/// cross-platform user name
pub extern "C" fn ceammc_current_user(cb: rust_str_cb) -> bool {
    match whoami::username() {
        Ok(name) => {
            cb.exec_str(name.as_str());
            true
        }
        Err(err) => {
            error!("can't get username: {err}");
            false
        }
    }
}
