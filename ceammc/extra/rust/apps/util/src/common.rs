use colored::{ColoredString, Colorize};
use log::{error, info, Level};
use std::io::Write;
use std::path::PathBuf;
use terminal_size::terminal_size;

fn log_level2str(level: Level) -> ColoredString {
    match level {
        Level::Error => "[error]".magenta(),
        Level::Warn => "[warn ]".yellow(),
        Level::Info => "[info ]".cyan(),
        Level::Debug => "[debug]".blue(),
        Level::Trace => "[trace]".white(),
    }
}

pub fn init_logger() {
    env_logger::Builder::new()
        .format_line_number(false)
        .format_timestamp(None)
        .format_module_path(false)
        .format_target(false)
        .filter_level(log::LevelFilter::Info)
        .format(|buf, record| writeln!(buf, "{} {}", log_level2str(record.level()), record.args()))
        .init();
}

pub enum Error {
    Common(String),
    FileNotFound(PathBuf, Option<String>),
    DirNotFound(PathBuf, Option<String>),
    FileCopyError(PathBuf, PathBuf, String),
    FileRemoveError(PathBuf, String),
    SymlinkError(PathBuf, PathBuf, String),
    NotSymlink(PathBuf),
}

pub fn output_error(err: &Error) {
    match err {
        Error::Common(msg) => println!("{} {msg}", "[error]".magenta()),
        Error::FileNotFound(file, desc) => {
            let file = file.to_string_lossy().to_string();
            match desc {
                Some(desc) => {
                    error!("{desc} file not found: {}", file.cyan());
                }
                _ => error!("file not found: {}", file.cyan()),
            }
        }
        Error::NotSymlink(path) => {
            error!("not a symlink: {}", path.to_string_lossy().as_ref().cyan())
        }
        Error::FileCopyError(from, dest, err) => {
            error!(
                "while copying {} to {}: {err}",
                from.to_string_lossy().as_ref().cyan(),
                dest.to_string_lossy().as_ref().cyan(),
            )
        }
        Error::FileRemoveError(path, err) => println!(
            "while removing {}: {err}",
            path.to_string_lossy().as_ref().cyan(),
        ),
        Error::SymlinkError(original, link, err) => {
            error!(
                "create link from {} to {}: {err}",
                link.to_string_lossy().as_ref().cyan(),
                original.to_string_lossy().as_ref().cyan(),
            )
        }
        Error::DirNotFound(dir, descr) => {
            let dir = dir.to_string_lossy().as_ref().cyan();
            match descr {
                Some(descr) => error!("{descr} directory not found: {dir}"),
                None => error!("directory not found: {dir}"),
            }
        }
    }
}

pub fn output_rule() {
    let width = terminal_size().map(|x| x.0 .0).unwrap_or(48);
    println!("{}", "=".repeat(width.into()).truecolor(100, 100, 100));
}

pub fn output_header(title: &str) {
    println!("{}", format!("[{title}]").bold().cyan());
    output_rule();
}

pub fn home_path(relpath: &str) -> PathBuf {
    let mut path = std::env::home_dir().unwrap();
    path.push(relpath);
    path
}

pub fn copy(from: &PathBuf, dest: &PathBuf) -> Result<(), Error> {
    std::fs::copy(from, dest)
        .map_err(|err| Error::FileCopyError(from.clone(), dest.clone(), err.to_string()))?;
    info!(
        "copy {} -> {}",
        format!("{from:?}").cyan(),
        format!("{dest:?}").cyan()
    );
    Ok(())
}

pub fn create_full_path_dir(path: &PathBuf) -> Result<(), Error> {
    fs_extra::dir::create_all(path, false).map_err(|err| Error::Common(err.to_string()))
}
