use crate::cli::{Cli, Commands, LangName, Pd};
use crate::common::output_error;
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
mod update;

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
        },
        Commands::Info {
            use_bytes,
            all,
            mem,
            cpu,
            system,
            pd: _,
            net,
        } => {
            output_info(use_bytes, all, mem, cpu, net, system, true);
        }
    }

    Ok(())
}
