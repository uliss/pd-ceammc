use std::path::PathBuf;

use colored::Colorize;
use terminal_size::terminal_size;

pub enum Error {
    Common(String),
    FileNotFound(PathBuf, Option<String>),
    FileCopyError(PathBuf, PathBuf, String),
    NotImplented(String),
}

pub fn output_error(err: &Error) {
    match err {
        Error::Common(msg) => println!("{} {msg}", "[error]".magenta()),
        Error::FileNotFound(file, desc) => {
            let file = file.to_string_lossy().to_string();
            match desc {
                Some(desc) => {
                    println!(
                        "{} {desc} file not found: {}",
                        "[error]".magenta(),
                        file.cyan()
                    );
                }
                _ => println!("{} file not found: {}", "[error]".magenta(), file.cyan()),
            }
        }
        Error::NotImplented(name) => {
            println!(
                "{} {} is {} yet!",
                "[error]".magenta(),
                name.underline(),
                "not implemented".red()
            )
        }
        Error::FileCopyError(from, dest, err) => {
            println!(
                "{} while copying {} to {}: {err}",
                "[error]".magenta(),
                from.to_string_lossy().as_ref().cyan(),
                dest.to_string_lossy().as_ref().cyan(),
            )
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
