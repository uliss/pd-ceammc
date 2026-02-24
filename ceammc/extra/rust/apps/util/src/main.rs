use crate::common::output_error;
use crate::{autostart::ProcessOptions, info::output_info};
use anyhow::anyhow;
use ceammc_shared_rs::config::{config_lang, config_store};
use clap::{Parser, Subcommand, ValueEnum};
use colored::Colorize;

#[path = "../../src/ceammc_config.rs"]
mod config;

mod apt;
mod autostart;
mod common;
mod dpkg;
mod info;
mod update;

#[derive(Debug, Clone, Copy, ValueEnum)]
enum LangName {
    Ru,
    En,
    Default,
}

#[derive(Clone, Subcommand)]
enum Pd {
    /// auto start control: enable, disable, set main patch symlink
    #[group(required = false, multiple = false)]
    #[command(alias = "auto")]
    Autostart {
        /// create symlink to autostart patch
        #[arg(short, long, name = "FILE")]
        add: Option<String>,
        /// restore default autostart patch symlink
        #[arg(long)]
        default: bool,
        /// enable autostart
        #[arg(short, long)]
        enable: bool,
        /// disable autostart
        #[arg(short, long)]
        disable: bool,
        /// print verbose info
        #[arg(short, long)]
        info: bool,
    },
    /// set PureData doc language (for ceammc external)
    Lang {
        /// language name code
        #[arg(short, long)]
        set: Option<LangName>,
    },
    #[group(required = false)]
    /// update CEAMMC PureData
    Update {
        /// update all (puredata first, then examples)
        #[arg(short, long)]
        all: bool,
        /// update examples only
        #[arg(short, long)]
        examples: bool,
        /// update puredata
        #[arg(short, long)]
        pd: bool,
        /// overwrite existing files (use together with --examples flag)
        #[arg(short, long)]
        force: bool,
    },
}

/// CEAM utilities
#[derive(Subcommand, Clone)]
#[command(version, about, long_about = None)]
enum Commands {
    #[command(subcommand)]
    /// PureData control
    Pd(Pd),
    /// System information
    Info {
        /// use bytes in output
        #[arg(short, long)]
        use_bytes: bool,
        /// output all information
        #[arg(short, long)]
        all: bool,
        /// output memory information
        #[arg(long)]
        mem: bool,
        /// output cpu information
        #[arg(long)]
        cpu: bool,
        /// output system information
        #[arg(long)]
        system: bool,
        /// output PureData information (default)
        #[arg(long)]
        pd: bool,
        /// output network information
        #[arg(long)]
        net: bool,
    },
}

#[derive(Parser)]
#[command(version, about, long_about = None)]
struct Cli {
    #[command(subcommand)]
    command: Commands,
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
