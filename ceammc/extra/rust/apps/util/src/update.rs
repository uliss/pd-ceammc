use crate::common::{self, home_path, output_header, output_rule, Error};
use colored::Colorize;
use log::warn;
use std::{
    os::unix::fs::PermissionsExt,
    path::PathBuf,
    process::{Command, Stdio},
    vec,
};

pub fn update_pd_ceammc() {
    output_header("update Pd CEAMMC");

    let args = ["apt", "update"];
    println!(
        "running command: {}\nthis can request {} password",
        args.join(" ").cyan(),
        std::env::var("USER").unwrap_or("???".to_string()).magenta()
    );
    output_rule();
    let status = Command::new("sudo")
        .args(args)
        .stdin(Stdio::inherit())
        .stdout(Stdio::inherit())
        .stderr(Stdio::inherit())
        .spawn()
        .expect("Failed to execute command")
        .wait()
        .expect("failed to wait on child");

    if status.success() {
        let args = ["apt", "install", "-y", "pd-ceammc"];
        output_rule();
        println!("running command: {}", args.join(" ").cyan());
        output_rule();

        let _status = Command::new("sudo")
            .args(args)
            .stdin(Stdio::inherit())
            .stdout(Stdio::inherit())
            .stderr(Stdio::inherit())
            .spawn()
            .expect("Failed to execute command")
            .wait()
            .expect("failed to wait on child");
    }
}

#[cfg(target_os = "linux")]
const SYS_EXAMPLE_DIR: &str = "/usr/lib/pd_ceammc/share/rpi/examples";
#[cfg(target_os = "linux")]
const USER_EXAMPLE_DIR: &str = "Documents/Pd";

#[cfg(target_os = "macos")]
const SYS_EXAMPLE_DIR: &str = "/Users/serge/Documents/Pd/ceam"; // for tests
#[cfg(target_os = "macos")]
const USER_EXAMPLE_DIR: &str = "Documents/Pd/rust"; // for tests

fn is_valid_example(path: &PathBuf) -> bool {
    path.is_file()
        && path.extension().unwrap_or_default() == "pd"
        && path
            .file_name()
            .map(|x| x.to_string_lossy().as_ref() != "main.pd")
            .unwrap_or(false)
}

pub fn update_examples(overwrite: bool) -> Result<(), Error> {
    // read examples
    let dir = PathBuf::from(SYS_EXAMPLE_DIR);
    if !(dir.exists() && dir.is_dir()) {
        return Err(common::Error::DirNotFound(
            dir,
            Some("original examples".to_string()),
        ));
    }

    let mut files = vec![];

    for x in std::fs::read_dir(dir) //
        .map_err(|err| Error::Common(err.to_string()))?
    {
        let entry = x.map_err(|err| Error::Common(err.to_string()))?;
        let path = entry.path();
        if !is_valid_example(&path) {
            continue;
        }

        files.push(path.display().to_string());
    }

    files.sort();

    // create user examples directory, if not exists
    let user_example_dir = home_path(USER_EXAMPLE_DIR);
    if !user_example_dir.exists() {
        common::create_full_path_dir(&user_example_dir)?;
        log::info!("mkdir: {}", user_example_dir.display().to_string().cyan());
    }

    // copy example files to user directory
    for f in &files {
        let from = PathBuf::from(f);
        let mut dest = user_example_dir.clone();
        if let Some(name) = from.file_name() {
            dest.push(name);

            if dest.exists() {
                if !overwrite {
                    warn!(
                    "overwrite attempt: {}, skipping ...\n\tuse {} flag to overwrite existing files",
                    dest.display().to_string().cyan(),
                    "--force".magenta()
                );
                    continue;
                }
                // fix permission
                let metadata = std::fs::metadata(&dest)
                    .map_err(|err| Error::from(&err, "get file permissions", &dest))?;

                if metadata.permissions().readonly() {
                    log::info!("fix permissions for {}", dest.display().to_string().cyan());
                    // (rw-r--r--)
                    let permissions = std::fs::Permissions::from_mode(0o644);
                    std::fs::set_permissions(&dest, permissions)
                        .map_err(|err| Error::from(&err, "set file permissions", &dest))?
                }
            }

            common::copy(&from, &dest)?;
        }
    }

    Ok(())
}
