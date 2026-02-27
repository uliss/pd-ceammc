use crate::{autostart::ORIG_MAIN_PATCH, common::output_header};

pub fn info() {
    output_header("XDG");
    println!("xdg_mime: \t{}", get_mime_type().trim_end());
    println!("xdg_app:  \t{}", get_default_app().trim_end());
}

pub fn get_mime_type() -> String {
    std::process::Command::new("xdg-mime")
        .args(["query", "filetype", ORIG_MAIN_PATCH])
        .output()
        .ok()
        .map(|x| String::from_utf8(x.stdout).unwrap_or("invalid output".to_string()))
        .unwrap_or_default()
}

pub fn get_default_app() -> String {
    std::process::Command::new("xdg-mime")
        .args(["query", "default", "text/x-puredata"])
        .output()
        .ok()
        .map(|x| String::from_utf8(x.stdout).unwrap_or("invalid output".to_string()))
        .unwrap_or_default()
}
