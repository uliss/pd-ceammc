use crate::autostart::ProcessOptions;
use crate::common::{output_error, output_header, output_rule};
use anyhow::anyhow;
use ceammc_shared_rs::config::{config_lang, config_load, config_store};
use chrono::{DateTime, Local};
use clap::{Parser, Subcommand, ValueEnum};
use colored::Colorize;
use humansize::{format_size, BINARY};
use std::process::{Command, Stdio};
use std::time::SystemTime;
use sysinfo::{Networks, System};

#[path = "../../src/ceammc_config.rs"]
mod config;

mod autostart;
mod common;

#[derive(Debug, Clone, Copy, ValueEnum)]
enum LangName {
    Ru,
    En,
    Default,
}

#[derive(Clone, Subcommand)]
enum Pd {
    /// autostart management
    #[group(required = false, multiple = false)]
    Autostart {
        /// add file to autostart
        #[arg(short, long)]
        add: Option<String>,
        /// restore default autostart patch link
        #[arg(long)]
        default: bool,
        /// enable autostart
        #[arg(short, long)]
        enable: bool,
        /// disable autostart
        #[arg(short, long)]
        disable: bool,
        /// print verbose autostart information
        #[arg(short, long)]
        info: bool,
    },
    /// set PureData doc language (for ceammc external)
    Lang {
        /// language name code
        #[arg(short, long)]
        set: Option<LangName>,
    },
    /// update CEAMMC PureData
    Update,
}

#[derive(Subcommand, Clone)]
#[command(version, about, long_about = None)]
enum Commands {
    #[command(subcommand)]
    /// PureData management
    Pd(Pd),
    /// output system information
    Info {
        #[arg(short, long)]
        use_bytes: bool,
        // output all information
        #[arg(short, long)]
        all: bool,
    },
}

#[derive(Parser)]
#[command(version, about, long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

fn data_size(size_bytes: u64, human: bool) -> String {
    if human {
        format_size(size_bytes, BINARY)
    } else {
        format!("{} bytes", size_bytes)
    }
}

fn output_memory(sys: &System, use_bytes: bool) {
    println!(
        "mem_total:    \t{}",
        data_size(sys.total_memory(), !use_bytes)
    );
    println!(
        "mem_used:     \t{}",
        data_size(sys.used_memory(), !use_bytes)
    );
    println!(
        "mem_swap:     \t{}",
        data_size(sys.total_swap(), !use_bytes)
    );
    println!("mem_swap_used:\t{}", data_size(sys.used_swap(), !use_bytes));
}

fn output_cpu(sys: &System) {
    println!("cpu_cores:\t{}", sys.cpus().len());
    println!("cpu_brand:\t{}", sys.cpus()[0].brand());
    println!("cpu_freq: \t{} MHz", sys.cpus()[0].frequency());
    println!("cpu_usage:\t{}", sys.global_cpu_usage());
    println!("cpu_load: \t{}", System::load_average().one);
}

fn output_net() {
    println!(
        "net_hostname:  \t{}",
        System::host_name().unwrap_or_default()
    );
    for (interface_name, network) in &Networks::new_with_refreshed_list() {
        if network.ip_networks().is_empty() {
            continue;
        }

        for ip in network.ip_networks() {
            println!(
                "net_iface:    \t[{interface_name}]\t{}",
                ip.addr.to_string()
            );
        }
    }
}

fn output_system() {
    let system_time = SystemTime::now();
    let datetime: DateTime<Local> = system_time.into();

    println!("sys_name:  \t{}", System::name().unwrap_or_default());
    println!("sys_date:  \t{}", datetime.format("%Y.%m.%d"));
    println!("sys_time:  \t{}", datetime.format("%H:%M:%S"));
    println!("sys_uptime:\t{} seconds", System::uptime());
    println!("sys_os:    \t{}", System::os_version().unwrap_or_default());
    println!(
        "sys_kernel:\t{}",
        System::kernel_version().unwrap_or_default()
    );
}

fn dpkg_version() -> String {
    std::process::Command::new("dpkg-query")
        .args(["-W", "-f", "${Version}", "pd-ceammc"])
        .output()
        .ok()
        .map(|x| String::from_utf8(x.stdout).unwrap_or("invalid output".to_string()))
        .unwrap_or("_".to_string())
}

fn output_pd() {
    println!("pd_distrib:   \t{}", crate::config::CEAMMC_DISTRIB_VERSION);
    println!("pd_ceam_ver:  \t{}", crate::config::CEAMMC_LIB_VERSION);
    println!("pd_dpkg_ver:  \t{}", dpkg_version());
    println!("pd_ver:       \t{}", crate::config::PD_TEXT_VERSION_FULL);
    println!("pd_git_branch:\t{}", crate::config::GIT_BRANCH);
    println!("pd_git_commit:\t{}", crate::config::GIT_COMMIT);
    println!("pd_build_date:\t{}", compile_time::date_str!());
    println!("pd_build_time:\t{}", compile_time::time_str!());

    let lang = config_load()
        .map(|x| x.doc_lang.to_string())
        .unwrap_or_default();
    println!("pd_lang:      \t{lang}");

    println!(
        "auto_patch: \t{}",
        autostart::main_patch().unwrap_or_default().cyan()
    );
    println!(
        "auto_script: \t{}",
        autostart::run_script().unwrap_or_default().cyan()
    );
    println!(
        "auto_start:  \t{}",
        autostart::desktop()
            .map(|_| "on".to_string())
            .unwrap_or("off".to_string())
            .cyan()
    );
}

fn output_info(
    use_bytes: bool,
    output_all: bool,
    output_mem: bool,
    cpu: bool,
    net: bool,
    system: bool,
    pd: bool,
) {
    let mut sys = sysinfo::System::new_all();
    sys.refresh_all();

    if output_all || output_mem {
        output_header("memory");
        output_memory(&sys, use_bytes);
        println!();
    }

    if output_all || cpu {
        output_header("cpu");
        output_cpu(&sys);
        println!();
    }

    if output_all || net {
        output_header("network");
        output_net();
        println!();
    }

    if output_all || system {
        output_header("system");
        output_system();
        println!();
    }

    if output_all || pd {
        output_header("puredata");
        output_pd();
        println!();
    }
}

fn main() -> anyhow::Result<()> {
    common::init_logger();
    let args = Cli::parse();

    match args.command {
        Commands::Pd(pd) => match pd {
            Pd::Autostart {
                add,
                default,
                enable,
                disable,
                info,
            } => {
                let opts = if let Some(file) = add {
                    ProcessOptions::Add(file)
                } else if default {
                    ProcessOptions::RestoreMainPatchLink
                } else if enable {
                    ProcessOptions::Enable
                } else if disable {
                    ProcessOptions::Disable
                } else if info {
                    ProcessOptions::VerboseInfo
                } else {
                    ProcessOptions::Info
                };

                if let Err(err) = autostart::process(opts) {
                    output_error(&err);
                }
            }
            Pd::Update => {
                let cmd = ["apt", "update"];
                println!(
                    "running command: {}\nthis can request {} password",
                    cmd.join(" ").cyan(),
                    std::env::var("USER").unwrap_or("???".to_string()).magenta()
                );
                output_rule();
                let status = Command::new("sudo")
                    .args(cmd)
                    .stdin(Stdio::inherit())
                    .stdout(Stdio::inherit())
                    .stderr(Stdio::inherit())
                    .spawn()
                    .expect("Failed to execute command")
                    .wait()
                    .expect("failed to wait on child");

                if status.success() {
                    let cmd = ["apt", "upgrade", "--only-upgrade", "pd-ceammc"];
                    output_rule();
                    println!("running command: {}", cmd.join(" ").cyan());
                    output_rule();

                    let _status = Command::new("sudo")
                        .args(cmd)
                        .stdin(Stdio::inherit())
                        .stdout(Stdio::inherit())
                        .stderr(Stdio::inherit())
                        .spawn()
                        .expect("Failed to execute command")
                        .wait()
                        .expect("failed to wait on child");
                }
            }
            Pd::Lang { set } => {
                let mut cfg =
                    ceammc_shared_rs::config::config_load().map_err(|err| anyhow!(err))?;
                match set {
                    Some(lang) => {
                        let lang = match lang {
                            LangName::Ru => config_lang::Russian,
                            LangName::En => config_lang::English,
                            LangName::Default => config_lang::Default,
                        };
                        cfg.doc_lang = lang;
                        config_store(&cfg).map_err(|err| anyhow!(err))?;
                        println!("lang: {}", cfg.doc_lang.to_string().cyan())
                    }
                    None => println!("lang: {}", cfg.doc_lang.to_string().cyan()),
                }
            }
        },
        Commands::Info { use_bytes, all } => {
            output_info(use_bytes, all, true, true, true, true, true);
        }
    }

    Ok(())
}
