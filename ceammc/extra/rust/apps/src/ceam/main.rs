use clap::{Parser, Subcommand};
use humansize::{format_size, BINARY};
use std::error::Error;
use sysinfo::System;

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
    Info {
        #[arg(long)]
        use_bytes: bool,
    },
}

#[derive(Subcommand, Clone)]
#[command(version, about, long_about = None)]
enum Commands {
    #[command(subcommand)]
    Pd(Pd),
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

fn main() -> Result<(), Box<dyn Error>> {
    env_logger::init();

    let args = Cli::parse();

    match args.command {
        Commands::Pd(pd) => match pd {
            Pd::Autostart(pd_autostart) => {}
            Pd::Update(pd_update) => {}
            Pd::Info { use_bytes } => {
                let info = os_info::get();
                let mut sys = sysinfo::System::new_all();
                sys.refresh_all();

                println!(
                    "total_memory:\t{}",
                    data_size(sys.total_memory(), !use_bytes)
                );
                println!("used_memory:\t{}", data_size(sys.used_memory(), !use_bytes));
                println!("total_swap:\t{}", data_size(sys.total_swap(), !use_bytes));
                println!("used_swap:\t{}", data_size(sys.used_swap(), !use_bytes));
                println!("cpu_cores:\t{}", sys.cpus().len());
                println!("cpu_brand:\t{}", sys.cpus()[0].brand());
                println!("cpu_freq:\t{}", sys.cpus()[0].frequency());
                println!("cpu_usage:\t{}", sys.global_cpu_usage());
                println!("system:   \t{}", System::name().unwrap_or_default());
                println!(
                    "kernel_version:\t{}",
                    System::kernel_version().unwrap_or_default()
                );
                println!("os_version:\t{}", System::os_version().unwrap_or_default());
                println!("hostname:  \t{}", System::host_name().unwrap_or_default());

                // We display all disks' information:
                // println!("=> disks:");
                // let disks = Disks::new_with_refreshed_list();
                // for disk in &disks {
                //     println!("{disk:?}");
                // }
            }
            _ => {}
        },
        _ => {}
    }

    Ok(())
}
