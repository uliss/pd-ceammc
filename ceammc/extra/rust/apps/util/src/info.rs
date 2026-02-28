use crate::{autostart, common::output_header, xdg};
use ceammc_shared_rs::config::config_load;
use chrono::{DateTime, Local};
use colored::Colorize;
use humansize::{format_size, BINARY};
use std::time::SystemTime;
use sysinfo::{Disks, Networks, System};

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

fn output_disk(use_bytes: bool) {
    for disk in Disks::new_with_refreshed_list().iter() {
        // macos skip
        if disk.mount_point().starts_with("/Volumes") {
            continue;
        }

        println!("disk_name:   \t{}", disk.name().display().to_string());
        println!(
            "disk_mount:  \t{}",
            disk.mount_point().display().to_string()
        );
        println!(
            "disk_free:   \t{}",
            data_size(disk.available_space(), !use_bytes)
        );
        println!(
            "disk_total:   \t{}",
            data_size(disk.total_space(), !use_bytes)
        );
        println!(
            "disk_usage:   \t{}%",
            100 - (100 * disk.available_space()) / disk.total_space()
        );
        println!(
            "disk_read:   \t{}",
            data_size(disk.usage().total_read_bytes, !use_bytes)
        );
        println!(
            "disk_write:   \t{}",
            data_size(disk.usage().total_written_bytes, !use_bytes)
        );
    }
}

fn output_pd() {
    println!("pd_distrib:   \t{}", crate::config::CEAMMC_DISTRIB_VERSION);
    println!("pd_ceam_ver:  \t{}", crate::config::CEAMMC_LIB_VERSION);
    println!(
        "pd_dpkg_ver:  \t{}",
        crate::apt::apt_version().unwrap_or("not installed".to_string())
    );
    println!(
        "pd_update:   \t{}",
        crate::apt::has_update().unwrap_or("no".to_string())
    );
    println!("pd_ver:       \t{}", crate::config::PD_TEXT_VERSION_FULL);
    println!("pd_git_branch:\t{}", crate::config::GIT_BRANCH);
    println!("pd_git_commit:\t{}", env!("GIT_COMMIT"));
    println!("pd_build_date:\t{}", compile_time::date_str!());
    println!("pd_build_time:\t{}", compile_time::time_str!());

    let lang = config_load()
        .map(|x| x.doc_lang.to_string())
        .unwrap_or_default();
    println!("pd_lang:      \t{lang}");

    println!(
        "auto_patch: \t{}",
        autostart::main_patch_symlink().unwrap_or_default().cyan()
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
    println!("pd_mime:      \t{}", xdg::get_mime_type());
    println!("pd_app:       \t{}", xdg::get_default_app());
}

pub fn output_info(
    use_bytes: bool,
    all: bool,
    mem: bool,
    cpu: bool,
    net: bool,
    system: bool,
    pd: bool,
    disk: bool,
) {
    let mut sys = sysinfo::System::new_all();
    sys.refresh_all();

    if all || mem {
        output_header("memory");
        output_memory(&sys, use_bytes);
        println!();
    }

    if all || disk {
        output_header("disks");
        output_disk(use_bytes);
        println!();
    }

    if all || cpu {
        output_header("cpu");
        output_cpu(&sys);
        println!();
    }

    if all || net {
        output_header("network");
        output_net();
        println!();
    }

    if all || system {
        output_header("system");
        output_system();
        println!();
    }

    if all || pd {
        output_header("puredata");
        output_pd();
        println!();
    }
}
