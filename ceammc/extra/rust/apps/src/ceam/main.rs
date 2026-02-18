use std::{error::Error, path::PathBuf};
use clap::Parser;
use log::error;

#[derive(Parser)]
#[command(version, about, long_about = None)]
struct Cli {

}

fn main() -> Result<(), Box<dyn Error>> {
    env_logger::init();
    let args = Cli::parse();

    println!("Hello, world!");

    Ok(())
}