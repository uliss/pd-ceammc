use colored::{ColoredString, Colorize};
use log::{error, Level};
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
    _Common(String),
    FileNotFound(PathBuf, Option<String>),
    FileCopyError(PathBuf, PathBuf, String),
    FileRemoveError(PathBuf, String),
    NotImplented(String),
}

pub fn output_error(err: &Error) {
    match err {
        Error::_Common(msg) => println!("{} {msg}", "[error]".magenta()),
        Error::FileNotFound(file, desc) => {
            let file = file.to_string_lossy().to_string();
            match desc {
                Some(desc) => {
                    error!("{desc} file not found: {}", file.cyan());
                }
                _ => error!("file not found: {}", file.cyan()),
            }
        }
        Error::NotImplented(name) => {
            error!("{} is {} yet!", name.underline(), "not implemented".red())
        }
        Error::FileCopyError(from, dest, err) => {
            println!(
                "while copying {} to {}: {err}",
                from.to_string_lossy().as_ref().cyan(),
                dest.to_string_lossy().as_ref().cyan(),
            )
        }
        Error::FileRemoveError(path, err) => println!(
            "while removing {}: {err}",
            path.to_string_lossy().as_ref().cyan(),
        ),
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
