use crate::common::{self, home_path, output_header, Error};
use colored::Colorize;
use std::path::PathBuf;

const MAIN_PATCH: &str = "Documents/Pd/main.pd";
const DESKTOP: &str = ".config/autostart/pd-ceammc.desktop";
const RUN_SCRIPT: &str = "bin/pd_start.sh";
const ORIG_SCRIPT: &str = "/usr/lib/pd_ceammc/share/rpi/pd_start.sh";
const ORIG_DESKTOP: &str = "/usr/lib/pd_ceammc/share/rpi/pd-ceammc-autostart.desktop";
const ORIG_MAIN_PATCH: &str = "/usr/lib/pd_ceammc/share/rpi/main.pd";

fn orig_run_script_path() -> PathBuf {
    PathBuf::from(ORIG_SCRIPT)
}

fn orig_desktop_path() -> PathBuf {
    PathBuf::from(ORIG_DESKTOP)
}

fn orig_main_patch_path() -> PathBuf {
    PathBuf::from(ORIG_MAIN_PATCH)
}

fn run_script_path() -> PathBuf {
    home_path(RUN_SCRIPT)
}

fn desktop_path() -> PathBuf {
    home_path(DESKTOP)
}

fn main_patch_path() -> PathBuf {
    home_path(MAIN_PATCH)
}

pub fn check_orig_main_patch_path() -> Result<PathBuf, Error> {
    let path = orig_main_patch_path();
    if path.is_file() {
        Ok(path)
    } else {
        Err(Error::FileNotFound(path, None))
    }
}

pub fn check_orig_desktop() -> Result<PathBuf, Error> {
    let path = orig_desktop_path();
    if path.is_file() {
        Ok(path)
    } else {
        Err(Error::FileNotFound(
            path,
            Some("original desktop".to_string()),
        ))
    }
}

pub fn check_orig_run_script() -> Result<PathBuf, Error> {
    let path = orig_run_script_path();
    if path.is_file() {
        Ok(path)
    } else {
        Err(Error::FileNotFound(
            path,
            Some("original run script".to_string()),
        ))
    }
}

fn copy_run_script() -> Result<(), common::Error> {
    let from = check_orig_run_script()?;
    std::fs::copy(from, run_script_path()).map_err(|err| Error::Common(err.to_string()))?;
    Ok(())
}

fn copy_desktop() -> Result<(), common::Error> {
    let from = check_orig_desktop()?;
    std::fs::copy(from, desktop_path()).map_err(|err| Error::Common(err.to_string()))?;
    Ok(())
}

pub fn enable() -> Result<(), common::Error> {
    copy_run_script()?;
    copy_desktop()?;
    Ok(())
}

fn remove_run_script() -> Result<(), common::Error> {
    // this is required
    let _ = check_orig_run_script()?;
    std::fs::remove_file(run_script_path()).map_err(|err| Error::Common(format!("{err}")))?;
    Ok(())
}

fn remove_desktop() -> Result<(), common::Error> {
    // this is required
    let _ = check_orig_desktop()?;
    std::fs::remove_file(desktop_path()).map_err(|err| Error::Common(format!("{err}")))?;
    Ok(())
}

pub fn disable() -> Result<(), common::Error> {
    remove_run_script()?;
    remove_desktop()?;
    Ok(())
}

pub fn is_enabled() -> bool {
    [desktop(), main_patch(), run_script()]
        .iter()
        .all(|x| x.as_ref().is_some_and(|x| !x.is_empty()))
}

fn to_string_path(path: &PathBuf) -> Option<String> {
    if path.is_file() {
        Some(path.to_string_lossy().to_string())
    } else {
        None
    }
}

pub fn main_patch() -> Option<String> {
    to_string_path(&main_patch_path())
}

pub fn desktop() -> Option<String> {
    to_string_path(&desktop_path())
}

pub fn run_script() -> Option<String> {
    to_string_path(&run_script_path())
}

pub enum ProcessOptions {
    Add(String),
    RestoreDefault,
    Enable,
    Disable,
    Info,
    VerboseInfo,
}

pub fn process(opts: ProcessOptions) -> Result<(), Error> {
    match opts {
        ProcessOptions::Add(_file) => Err(Error::NotImplented("add_file".to_string())),
        ProcessOptions::RestoreDefault => Err(Error::NotImplented("restore default".to_string())),
        ProcessOptions::Enable => enable(),
        ProcessOptions::Disable => disable(),
        ProcessOptions::Info => {
            output_header("autostart");
            if is_enabled() {
                println!("PureData autostart is {}", "enabled".cyan());
                println!(
                    "patch:                {}",
                    main_patch().unwrap_or_default().cyan()
                );
            } else {
                println!("PureData autostart is {}", "disabled".magenta().underline());
            }
            Ok(())
        }
        ProcessOptions::VerboseInfo => {
            output_header("autostart");
            println!("patch:    \t{}", main_patch().unwrap_or_default().cyan());
            println!("script:   \t{}", run_script().unwrap_or_default().cyan());
            println!("desktop:  \t{}", desktop().unwrap_or_default().cyan());
            Ok(())
        }
    }
}
