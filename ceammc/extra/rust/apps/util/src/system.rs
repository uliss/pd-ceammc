use std::process::Command;

use crate::common::Error;

pub fn enable_overlayfs(new_state: bool) -> Result<bool, Error> {
    let current_state = get_overlay_status()?;
    if current_state == new_state {
        log::info!(
            "OverlayFS already {}. Nothing to do",
            if new_state { "enabled" } else { "disabled" }
        );
        Ok(new_state)
    } else {
        let output = Command::new("sudo")
            .args(["raspi-config", "nonint", "enable_overlayfs"])
            .output()
            .map_err(|e| {
                Error::SystemConfigError(format!("failed to run sudo raspi-config: {}", e))
            })?;

        if output.status.success() {
            Ok(new_state)
        } else {
            Err(Error::SystemConfigError(format!(
                "failed to enable overlayfs: {}",
                String::from_utf8_lossy(&output.stderr)
            )))
        }
    }
}

pub fn get_overlay_status() -> Result<bool, Error> {
    let output = Command::new("raspi-config")
        .args(["nonint", "get_overlay_now"])
        .output()
        .map_err(|e| Error::SystemConfigError(format!("failed to run raspi-config: {}", e)))?;

    let stdout = String::from_utf8_lossy(&output.stdout);
    let status = stdout.trim();

    match status {
        "0" => Ok(true),  // overlay is enabled
        "1" => Ok(false), // overlay is disabled
        "" => {
            // Fallback: older raspi-config versions don't write to stdout;
            // state is conveyed via the exit code
            match output.status.code() {
                Some(0) => Ok(true),
                Some(1) => Ok(false),
                Some(code) => Err(Error::SystemConfigError(format!(
                    "raspi-config exited with code {}",
                    code
                ))),
                None => Err(Error::SystemConfigError(
                    "raspi-config terminated by signal".to_string(),
                )),
            }
        }
        other => Err(Error::SystemConfigError(format!(
            "unexpected output from raspi-config: {:?}",
            other
        ))),
    }
}
