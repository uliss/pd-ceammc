use crate::{cli::FileShare, common::Error};

use std::process::{Command, Output};

/// Runs a command through sudo and returns its output.
/// Fails if the process cannot be spawned or exits with a non-zero status.
fn run_sudo(args: &[&str]) -> Result<Output, Error> {
    let output = Command::new("sudo")
        .args(args)
        .output()
        .map_err(|e| Error::Common(format!("failed to spawn sudo: {e}")))?;

    if !output.status.success() {
        let stderr = String::from_utf8_lossy(&output.stderr);
        return Err(Error::Common(format!(
            "command 'sudo {}' failed: {}",
            args.join(" "),
            stderr.trim()
        )));
    }

    Ok(output)
}

/// Manages the Samba file share on a Raspberry Pi.
pub fn file_share(act: FileShare) -> Result<(), Error> {
    match act {
        FileShare::Enable => {
            // Remove the mask (if set), then enable autostart and start now
            run_sudo(&["systemctl", "unmask", "smbd", "nmbd"])?;
            run_sudo(&["systemctl", "enable", "--now", "smbd", "nmbd"])?;
        }
        FileShare::Disable => {
            // Stop the services and remove them from autostart
            run_sudo(&["systemctl", "stop", "smbd", "nmbd"])?;
            run_sudo(&["systemctl", "disable", "smbd", "nmbd"])?;
        }
        FileShare::Status => {
            // "status" returns non-zero exit code when service is stopped,
            // so we need to capture output without treating it as an error.
            let output = Command::new("sudo")
                .args(["systemctl", "status", "smbd", "nmbd", "--no-pager"])
                .output()
                .map_err(|e| Error::Common(format!("failed to spawn systemctl: {e}")))?;

            // Print stdout (contains status info) and stderr (contains hints)
            print!("{}", String::from_utf8_lossy(&output.stdout));
            eprint!("{}", String::from_utf8_lossy(&output.stderr));
        }
    }
    Ok(())
}
