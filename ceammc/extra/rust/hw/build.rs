extern crate cbindgen;

use std::env;

#[cfg(target_os = "windows")] 
use std::path::PathBuf;
#[cfg(target_os = "windows")] 
use walkdir::WalkDir;

fn main() {
    let _crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();

    #[cfg(target_family = "unix")]
    cbindgen::generate(_crate_dir)
        .expect("Unable to generate bindings")
        .write_to_file("hw_rust.hpp");

    #[cfg(target_os = "windows")]
    {
        const PDFIUM_DLL: &str = "pdfium.dll";

        let out_dir = env::var_os("OUT_DIR").unwrap();
        // let out_dir = PathBuf::new(out_dir.to_str().unwrap());
        let mut dir = PathBuf::new();
        dir.push(out_dir.to_str().unwrap());
        dir.pop();
        dir.pop();

        let mut pdfium_dll = String::new();
        let wdir = WalkDir::new(dir.to_str().unwrap());
        for f in wdir.into_iter().filter_map(|e| e.ok()) {
            let fname = f.file_name().to_string_lossy();
            if fname == PDFIUM_DLL {
                pdfium_dll = f.path().to_string_lossy().into_owned();
                println!("PDFIUM DLL: {pdfium_dll}");
                break;
            }
        }

        dir.pop();
        dir.pop();
        dir.pop();
        dir.push(PDFIUM_DLL);
        
        match std::fs::copy(pdfium_dll, dir) {
            Err(err) => println!("Error {err}"),
            _ => {}
        }
    }
}
