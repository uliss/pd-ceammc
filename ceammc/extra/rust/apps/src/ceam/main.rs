use chrono::{DateTime, Local};
use clap::{Parser, Subcommand};
use colored::Colorize;
use humansize::{format_size, BINARY};
use std::{error::Error, time::SystemTime};
use sysinfo::{Networks, System};

#[derive(Clone, Subcommand)]
enum PdAutostart {}

#[derive(Clone, Subcommand)]
enum PdUpdate {}

#[derive(Clone, Subcommand)]
enum Pd {
    #[command(subcommand)]
    Autostart(PdAutostart),
    #[command(subcommand)]
    Update(PdUpdate),
}

#[derive(Subcommand, Clone)]
#[command(version, about, long_about = None)]
enum Commands {
    #[command(subcommand)]
    Pd(Pd),
    Info {
        #[arg(long)]
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

fn output_header(title: &str) {
    let size = term_size::dimensions().unwrap_or((48, 32));
    println!("{}", format!("[{title}]").bold().cyan());
    println!("{}", "=".repeat(size.0));
}

fn output_memory(sys: &System, use_bytes: bool) {
    println!("mem_total:    \t{}", data_size(sys.total_memory(), !use_bytes));
    println!("mem_used:     \t{}", data_size(sys.used_memory(), !use_bytes));
    println!("mem_swap:     \t{}", data_size(sys.total_swap(), !use_bytes));
    println!(
        "mem_swap_used:\t{}",
        data_size(sys.used_swap(), !use_bytes)
    );
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

fn output_info(
    use_bytes: bool,
    output_all: bool,
    output_mem: bool,
    cpu: bool,
    net: bool,
    system: bool,
) {
    // let info = os_info::get();
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
    }
}

fn main() -> Result<(), Box<dyn Error>> {
    env_logger::init();

    let args = Cli::parse();

    match args.command {
        Commands::Pd(pd) => match pd {
            Pd::Autostart(pd_autostart) => {}
            Pd::Update(pd_update) => {}
            _ => {}
        },
        Commands::Info { use_bytes, all } => {
            output_info(use_bytes, all, true, true, true, true);
        }
    }

    Ok(())
}
