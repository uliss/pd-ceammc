#[allow(dead_code)]
pub fn dpkg_version() -> String {
    std::process::Command::new("dpkg-query")
        .args(["-W", "-f", "${Version}", "pd-ceammc"])
        .output()
        .ok()
        .map(|x| String::from_utf8(x.stdout).unwrap_or("invalid output".to_string()))
        .unwrap_or("_".to_string())
}

