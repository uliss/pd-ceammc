#[cfg(target_os = "linux")]
pub fn apt_version() -> Option<String> {
    use rust_apt::new_cache;

    let cache = new_cache!().ok()?;
    let pkg = cache.get("pd-ceammc")?;
    Some(pkg.installed()?.version().to_string())
}

#[cfg(not(target_os = "linux"))]
pub fn apt_version() -> Option<String> {
    None
}
