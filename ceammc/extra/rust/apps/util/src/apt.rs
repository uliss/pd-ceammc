#[cfg(target_os = "linux")]
pub fn apt_version() -> Option<String> {
    use rust_apt::new_cache;

    let cache = new_cache!().ok()?;
    let pkg = cache.get("pd-ceammc")?;
    Some(pkg.installed()?.version().to_string())
}

#[cfg(target_os = "linux")]
pub fn has_update() -> Option<String> {
    use rust_apt::new_cache;

    let cache = new_cache!().ok()?;
    let pkg = cache.get("pd-ceammc")?;
    let inst = pkg.installed()?;
    let cand = pkg.candidate()?;

    if inst != cand {
        Some(format!("-> {}", cand.version().unwrap_or_default()))
    } else {
        None
    }
}

#[cfg(not(target_os = "linux"))]
pub fn apt_version() -> Option<String> {
    None
}

#[cfg(not(target_os = "linux"))]
pub fn has_update() -> Option<String> {
    None
}
