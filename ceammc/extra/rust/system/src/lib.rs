pub mod command;
pub mod sysinfo;

#[allow(non_camel_case_types)]
#[repr(C)]
pub struct system_notify_cb {
    client_id: usize,
    cb: Option<extern "C" fn(client_id: usize)>,
}

impl system_notify_cb {
    fn notify(&self) {
        self.cb.map(|f| f(self.client_id));
    }
}
