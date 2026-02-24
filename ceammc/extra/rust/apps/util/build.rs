use clap::{self, CommandFactory};
use std::process::Command;

fn main() -> std::io::Result<()> {
    let git_commit = Command::new("git")
        .args(&["describe", "--tags", "HEAD"])
        .output()
        .map(|out| String::from_utf8(out.stdout).unwrap_or_default())
        .unwrap_or_default();

    let git_commit_rev = git_commit
        .split('-')
        .take(2)
        .collect::<Vec<_>>()
        .get(1)
        .map(|x| x.to_string())
        .unwrap_or("unknown".to_string());

    println!("cargo:rustc-env=GIT_COMMIT={}", git_commit);
    println!("cargo:rustc-env=GIT_COMMIT_REV={}", git_commit_rev);
    // Tell Cargo to re-run this script if the HEAD commit changes
    println!("cargo:rerun-if-changed=.git/HEAD");

    // man pages
    mod cli {
        include!(concat!(env!("CARGO_MANIFEST_DIR"), "/src/cli.rs"));
    }

    let cmd = <cli::Cli as clap::CommandFactory>::command();

    let manifest_dir = std::path::PathBuf::from(env!("CARGO_MANIFEST_DIR"));
    let out_dir = manifest_dir.join("man");
    std::fs::create_dir(&out_dir)?;
    clap_mangen::generate_to(cmd, &out_dir)?;

    Ok(())
}
