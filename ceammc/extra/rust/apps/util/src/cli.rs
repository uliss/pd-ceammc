use clap::{Parser, Subcommand, ValueEnum};

#[derive(Debug, Clone, Copy, ValueEnum)]
pub enum LangName {
    Ru,
    En,
    Default,
}

#[derive(Clone, Subcommand)]
pub enum Pd {
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
        /// update examples
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

#[derive(Clone, Subcommand)]
pub enum Xdg {
    /// xdg MIME type and default application info
    Info,
}

/// CEAM utilities
#[derive(Subcommand, Clone)]
#[command(version, about, long_about = None)]
pub enum Commands {
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
    /// XDG (X Desktop Group)
    #[command(subcommand)]
    Xdg(Xdg),
}

#[derive(Parser)]
#[command(version, about, long_about = None)]
pub struct Cli {
    #[command(subcommand)]
    pub command: Commands,
}
