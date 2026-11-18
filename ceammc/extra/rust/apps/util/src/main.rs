use crate::cli::{Cli, Commands, LangName, Pd};
use crate::common::output_error;
use crate::dpkg::dpkg_version;
use crate::repo::{parse_deb_page, rollback_to_version, RPI_APT_REPO_URL};
use crate::{autostart::ProcessOptions, info::output_info};
use anyhow::anyhow;
use ceammc_shared_rs::config::{config_lang, config_store};
use clap::Parser;
use colored::Colorize;

#[path = "../../src/ceammc_config.rs"]
mod config;

mod apt;
mod autostart;
mod cli;
mod common;
mod dpkg;
mod info;
mod repo;
mod samba;
mod system;
mod update;
mod wayfire;
mod xdg;

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
                    ProcessOptions::SetMainPatchLink(file)
                } else if default {
                    ProcessOptions::RestoreMainPatchLink
                } else if enable {
                    ProcessOptions::Enable
                } else if disable {
                    ProcessOptions::Disable
                } else if info {
                    ProcessOptions::VerboseInfo
                } else {
                    ProcessOptions::ShortInfo
                };

                if let Err(err) = autostart::process(opts) {
                    output_error(&err);
                }
            }
            Pd::Update {
                all,
                examples,
                pd,
                force: overwrite,
            } => {
                // update on empty also
                if all || pd || (!pd && !examples) {
                    update::update_pd_ceammc();
                }

                if all || examples {
                    if let Err(err) = update::update_examples(overwrite) {
                        output_error(&err);
                    }
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
            Pd::Version { act } => match act {
                Some(cmd) => match cmd {
                    cli::VersionAct::List => {
                        let debs = parse_deb_page(RPI_APT_REPO_URL)?;
                        println!("repository url: {}", RPI_APT_REPO_URL.cyan());
                        for deb in &debs {
                            println!("pd-ceammc: {}", deb.info().to_string().cyan())
                        }
                    }
                    cli::VersionAct::Rollback { version } => rollback_to_version(&version)?,
                },
                None => println!("Installed version: {}", dpkg_version()),
            },
        },
        Commands::Info {
            use_bytes,
            all,
            mem,
            cpu,
            system,
            pd: _,
            net,
            disk,
            keyboard,
        } => {
            output_info(use_bytes, all, mem, cpu, net, system, true, disk, keyboard);
        }
        Commands::Xdg(xdg) => match xdg {
            cli::Xdg::Info {} => xdg::info(),
        },
        Commands::Keyboard(keyboard) => {
            //
            if let Err(err) = match keyboard {
                cli::Keyboard::Fix => {
                    wayfire::fix_keyboard_layout(Some(wayfire::LayoutIndicator::Scroll))
                }
                cli::Keyboard::Info => {
                    common::output_header("keyboard");
                    wayfire::output_keyboard_info();
                    Ok(())
                }
            } {
                output_error(&err);
            }
        }
        Commands::System(system) => match system {
            cli::System::Fs(act) => {
                let res = match act {
                    cli::FileSystem::Readonly => system::enable_overlayfs(true),
                    cli::FileSystem::Readwrite => system::enable_overlayfs(false),
                };
                match res {
                    Ok(value) => match value {
                        system::OverlayFsState::NoChange(state) => {
                            let status = if state { "enabled" } else { "disabled" };
                            log::info!("OverlayFS is already {status}. Nothing to do");
                        }
                        system::OverlayFsState::Changed(new_state) => {
                            log::info!("OverlayFS: {new_state}");
                            log::warn!("You should reboot your computer to apply the changes");
                        }
                    },
                    Err(err) => output_error(&err),
                };
            }
            cli::System::FileShare(act) => match samba::file_share(act) {
                Ok(_) => {}
                Err(err) => output_error(&err),
            },
        },
    }

    Ok(())
}
