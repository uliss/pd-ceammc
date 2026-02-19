use std::process::Command;

fn main() {
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
}
