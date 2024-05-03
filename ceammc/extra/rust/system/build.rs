extern crate cbindgen;

use std::env;

fn main() {
    let _crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();

    #[cfg(target_os = "macos")]
    cbindgen::generate(_crate_dir)
        .expect("Unable to generate bindings")
        .write_to_file("system_rust.hpp");
}
