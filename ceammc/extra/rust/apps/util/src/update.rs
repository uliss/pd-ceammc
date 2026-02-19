use crate::common::{output_header, output_rule};
use colored::Colorize;
use std::process::{Command, Stdio};

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

pub fn update_examples() {
    output_header("update examples");
}
