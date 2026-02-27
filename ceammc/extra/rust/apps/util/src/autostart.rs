use crate::common::{self, home_path, output_error, output_header, output_rule, Error};
use colored::Colorize;
use log::{info, warn};
use std::{
    fs::File,
    io::{BufRead, BufReader},
    path::PathBuf,
};

const MAIN_PATCH_SYMLINK: &str = "Documents/Pd/main.pd";
const DESKTOP: &str = ".config/autostart/pd-ceammc.desktop";
const RUN_SCRIPT: &str = "bin/pd_start.sh";
const ORIG_SCRIPT: &str = "/usr/lib/pd_ceammc/share/rpi/pd_start.sh";
const ORIG_DESKTOP: &str = "/usr/lib/pd_ceammc/share/rpi/pd-ceammc-autostart.desktop";
pub const ORIG_MAIN_PATCH: &str = "/usr/lib/pd_ceammc/share/rpi/examples/main.pd";

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

pub fn main_patch_symlink_path() -> PathBuf {
    home_path(MAIN_PATCH_SYMLINK)
}

pub fn check_orig_main_patch_path() -> Result<PathBuf, Error> {
    let path = orig_main_patch_path();
    if path.is_file() {
        Ok(path)
    } else {
        Err(Error::FileNotFound(
            path,
            Some("original main path".to_string()),
        ))
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

fn copy(from: &PathBuf, dest: &PathBuf) -> Result<(), common::Error> {
    std::fs::copy(from, dest)
        .map_err(|err| Error::FileCopyError(from.clone(), dest.clone(), err.to_string()))?;
    info!(
        "copy {} -> {}",
        format!("{from:?}").cyan(),
        format!("{dest:?}").cyan()
    );
    Ok(())
}

fn remove_file(path: &PathBuf) -> Result<(), common::Error> {
    std::fs::remove_file(path)
        .map_err(|err| Error::FileRemoveError(path.clone(), err.to_string()))?;
    info!("remove {}", format!("{path:?}").cyan());
    Ok(())
}

fn remove_symlink(symlink: &PathBuf, force: bool) -> Result<(), common::Error> {
    if symlink.exists() {
        info!("removing symlink {}", format!("{symlink:?}").cyan());
        if !symlink.is_symlink() && !force {
            output_error(&Error::NotSymlink(symlink.clone()));
            return Err(Error::Common(
                "(re)move or rename this file manually, then try again".to_string(),
            ));
        }
        remove_file(symlink)
    } else {
        Ok(())
    }
}

fn copy_run_script() -> Result<(), common::Error> {
    let from = check_orig_run_script()?;
    copy(&from, &run_script_path())?;
    Ok(())
}

fn fix_desktop_vars(path: &PathBuf) -> Result<(), common::Error> {
    let data = std::fs::read_to_string(path)
        .map_err(|err| Error::Common(format!("file read error: {err}")))?;

    let data = data.replace(
        "${HOME}",
        &std::env::home_dir()
            .map(|p| p.display().to_string())
            .unwrap_or_default(),
    );

    std::fs::write(path, data).map_err(|err| Error::Common(format!("file write error: {err}")))
}

fn copy_desktop() -> Result<(), common::Error> {
    let from = check_orig_desktop()?;
    let dest = desktop_path();
    copy(&from, &dest)?;
    fix_desktop_vars(&dest)
}

pub fn enable() -> Result<(), common::Error> {
    copy_run_script()?;
    copy_desktop()?;

    if !main_patch_symlink_path().exists() {
        warn!("main patch not exists, copying default");
        let path = check_orig_main_patch_path()?;
        let dest = &&main_patch_symlink_path();
        copy(&path, &dest)?;
    }

    output_rule();
    info!("enabled");
    Ok(())
}

fn remove_run_script() -> Result<(), common::Error> {
    // this check is required
    let _ = check_orig_run_script()?;

    if run_script_path().exists() {
        remove_file(&run_script_path())?;
    }

    Ok(())
}

fn remove_desktop() -> Result<(), common::Error> {
    // this check is required
    let _ = check_orig_desktop()?;

    if desktop_path().exists() {
        remove_file(&desktop_path())?;
    }

    Ok(())
}

pub fn disable() -> Result<(), common::Error> {
    remove_run_script()?;
    remove_desktop()?;

    output_rule();
    info!("disabled");
    Ok(())
}

pub fn is_enabled() -> bool {
    [desktop(), main_patch_symlink(), run_script()]
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

pub fn main_patch_symlink() -> Option<String> {
    to_string_path(&main_patch_symlink_path())
}

pub fn desktop() -> Option<String> {
    to_string_path(&desktop_path())
}

pub fn run_script() -> Option<String> {
    to_string_path(&run_script_path())
}

fn create_symlink(original: &PathBuf, link: &PathBuf) -> Result<(), common::Error> {
    std::os::unix::fs::symlink(&original, &link)
        .map_err(|err| Error::SymlinkError(original.clone(), link.clone(), err.to_string()))?;

    info!("create symlink from {link:?} -> {original:?}");
    Ok(())
}

fn restore_main_patch_link() -> Result<(), common::Error> {
    let original = check_orig_main_patch_path()?;
    let symlink = main_patch_symlink_path();

    remove_symlink(&symlink, true)?;
    create_symlink(&original, &symlink)?;

    output_rule();
    println!(
        "autostart file: {}",
        original.as_os_str().to_string_lossy().bright_cyan()
    );

    Ok(())
}

fn check_is_pd_patch(path: &PathBuf) -> Result<(), common::Error> {
    let file = File::open(path).map_err(|err| Error::Common(err.to_string()))?;
    let reader = BufReader::new(file);

    if reader
        .lines()
        .next()
        .ok_or(Error::Common("read error".to_string()))?
        .map_err(|err| Error::Common(err.to_string()))?
        .starts_with("#N canvas")
    {
        Ok(())
    } else {
        Err(Error::Common("not a Pd patch".to_string()))
    }
}

fn set_main_patch_link(file: &str) -> Result<(), common::Error> {
    let original = PathBuf::from(file);
    if !original.is_file() {
        return Err(common::Error::FileNotFound(
            original,
            Some("new patch".to_string()),
        ));
    }

    let original = original
        .canonicalize()
        .map_err(|_| Error::FileNotFound(original, Some("new patch".to_string())))?;

    info!(
        "adding patch to autostart: {}",
        format!("{original:?}").cyan()
    );

    info!("checking if valid PureData patch ...");
    check_is_pd_patch(&original)?;

    let symlink = main_patch_symlink_path();
    remove_symlink(&symlink, false)?;
    create_symlink(&original, &symlink)?;

    output_rule();
    println!(
        "new autostart file: {}",
        original.as_os_str().to_string_lossy().bright_cyan()
    );

    Ok(())
}

pub enum ProcessOptions {
    SetMainPatchLink(String),
    RestoreMainPatchLink,
    Enable,
    Disable,
    ShortInfo,
    VerboseInfo,
}

pub fn process(opts: ProcessOptions) -> Result<(), Error> {
    match opts {
        ProcessOptions::SetMainPatchLink(file) => set_main_patch_link(&file),
        ProcessOptions::RestoreMainPatchLink => restore_main_patch_link(),
        ProcessOptions::Enable => enable(),
        ProcessOptions::Disable => disable(),
        ProcessOptions::ShortInfo => {
            output_header("autostart");
            if is_enabled() {
                println!("PureData autostart is {}", "enabled".cyan());
                println!(
                    "patch:                {}",
                    main_patch_symlink().unwrap_or_default().cyan()
                );
            } else {
                println!("PureData autostart is {}", "disabled".magenta().underline());
            }
            Ok(())
        }
        ProcessOptions::VerboseInfo => {
            output_header("autostart");
            println!(
                "patch:    \t{}",
                main_patch_symlink().unwrap_or_default().cyan()
            );
            println!("script:   \t{}", run_script().unwrap_or_default().cyan());
            println!("desktop:  \t{}", desktop().unwrap_or_default().cyan());
            Ok(())
        }
    }
}
