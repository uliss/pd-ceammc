use crate::common::{self, output_header, output_rule, Error};
use colored::Colorize;
use std::{
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
        let args = ["apt", "upgrade", "--only-upgrade", "pd-ceammc"];
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
const RPI_EXAMPLES_DIR: &str = "/usr/lib/pd_ceammc/share/rpi/examples";
#[cfg(target_os = "macos")]
const RPI_EXAMPLES_DIR: &str = "/Users/serge/Documents/Pd"; // for tests

fn is_valid_example(path: &PathBuf) -> bool {
    path.is_file() && path.extension().unwrap_or_default() == "pd"
}

pub fn update_examples() -> Result<(), Error> {
    // output_header("update examples");
    let dir = PathBuf::from(RPI_EXAMPLES_DIR);
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

    log::info!("{files:?}");

    Ok(())

    // let files = std::fs::read_dir(dir)
    //     .map_err(|err| Error::Common(err.to_string()))?
    //     .into_iter()
    //     .filter(|x| x.is_ok())
    //     .map(|x| x.unwrap())
    //     .collect::<Vec<_>>();

    // todo!()
}
