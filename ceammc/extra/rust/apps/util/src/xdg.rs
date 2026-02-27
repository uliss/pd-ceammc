use crate::{autostart::ORIG_MAIN_PATCH, common::output_header};

pub fn info() {
    output_header("XDG");
    println!("xdg_mime:  	{}", get_mime());
    println!("xdg_app:      {}", get_default_app());
}

fn get_mime() -> String {
    std::process::Command::new("xdg-mime")
        .args(["query", "filetype", ORIG_MAIN_PATCH])
        .output()
        .ok()
        .map(|x| String::from_utf8(x.stdout).unwrap_or("invalid output".to_string()))
        .unwrap_or_default()
}

fn get_default_app() -> String {
    std::process::Command::new("xdg-mime")
        .args(["query", "default", "text/x-puredata"])
        .output()
        .ok()
        .map(|x| String::from_utf8(x.stdout).unwrap_or("invalid output".to_string()))
        .unwrap_or_default()
}
