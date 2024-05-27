use clap::Parser;
use colored::*;
use log::{debug, info, warn};
use mach_object::{LoadCommand, MachCommand, OFile};
use memmap::Mmap;
use std::collections::{HashMap, HashSet};
use std::error::Error;
use std::fmt;
use std::fs::File;
use std::io::Cursor;
use std::path::{Path, PathBuf};
use std::process::Command;

#[derive(Parser)]
#[command(version, about, long_about = None)]
struct Cli {
    /// dylib files
    #[arg(long, required=true, num_args(1..))]
    pub files: Vec<String>,
    /// rpaths search directories
    #[arg(long, required=false, num_args(0..))]
    pub rpaths: Vec<PathBuf>,
    /// output directory where to copy dylib files
    #[arg(long, short)]
    dir: String,
    /// output script actions
    #[arg(long, action)]
    script: bool,
}

struct FileFixes {
    fix_id: Option<String>,
    fix_rpath: HashSet<(String, String)>,
    delete_rpath: HashSet<String>,
}

impl FileFixes {
    fn new() -> Self {
        FileFixes {
            fix_id: None,
            fix_rpath: HashSet::new(),
            delete_rpath: HashSet::new(),
        }
    }
}

#[derive(Debug)]
enum DylibError {
    FileNotFound(String),
}

// Display implementation is required for std::error::Error.
impl fmt::Display for DylibError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            DylibError::FileNotFound(x) => write!(f, "file not found: '{x}'"),
        }
    }
}

impl Error for DylibError {}

fn make_rpath(path: &str) -> String {
    Path::new("@rpath")
        .join(
            Path::new(path)
                .file_name()
                .unwrap_or_default()
                .to_string_lossy()
                .to_string(),
        )
        .to_string_lossy()
        .to_string()
}

fn process_dylib(
    args: &Cli,
    path: &str,
    map: &mut HashMap<String, FileFixes>,
) -> Result<(), Box<dyn Error>> {
    info!("process {path}");

    let file = File::open(path)?;
    let mmap = unsafe { Mmap::map(&file) }?;

    let payload = mmap.as_ref();
    let mut cur = Cursor::new(payload);

    let mut rpaths = Vec::<PathBuf>::new();
    let mut rpath_deps = Vec::<String>::new();

    match OFile::parse(&mut cur)? {
        OFile::MachFile {
            header: _,
            commands,
        } => {
            for MachCommand(cmd, _) in commands.iter() {
                match cmd {
                    LoadCommand::IdDyLib(dylib) => {
                        if !dylib.name.starts_with("@rpath")
                            && !dylib.name.starts_with("@loader_path")
                        {
                            let new_id = make_rpath(dylib.name.as_str());

                            if let Some(v) = map.get_mut(path) {
                                v.fix_id = Some(new_id);
                            } else {
                                let mut fix = FileFixes::new();
                                fix.fix_id = Some(new_id);
                                map.insert(path.to_string(), fix);
                            }
                        }
                    }

                    LoadCommand::LoadDyLib(dylib)
                    | LoadCommand::LoadWeakDyLib(dylib)
                    | LoadCommand::ReexportDyLib(dylib)
                    | LoadCommand::LoadUpwardDylib(dylib)
                    | LoadCommand::LazyLoadDylib(dylib) => {
                        if dylib.name.starts_with("/usr/lib")
                            || dylib.name.starts_with("/System/Library")
                            || dylib.name.starts_with("@rpath/libc++.1.dylib")
                        {
                            continue;
                        }

                        if !dylib.name.starts_with("@") {
                            let dylib_full_path = dylib.name.to_string();
                            let replace = (
                                dylib_full_path.clone(),
                                make_rpath(dylib_full_path.as_str()),
                            );

                            if let Some(v) = map.get_mut(&path.to_owned()) {
                                v.fix_rpath.insert(replace);
                            } else {
                                let mut fix = FileFixes::new();
                                fix.fix_rpath.insert(replace);
                                map.insert(path.to_string(), fix);
                            }

                            process_dylib(args, &dylib.name, map)?;
                        } else {
                            rpath_deps.push(dylib.name.as_str().to_string());
                        }
                    }
                    LoadCommand::Rpath(x) => {
                        info!("rpath: {x}");
                        if x == "@rpath" || x == "@loader_path" {
                            rpaths.push(
                                Path::new(path)
                                    .parent()
                                    .expect("parent path expected")
                                    .to_path_buf(),
                            );
                            continue;
                        }

                        let rpath = PathBuf::from(x);
                        if !rpath.exists() {
                            warn!("rpath directory is not exists: '{x}'");
                            continue;
                        }

                        if rpath.is_dir() {
                            rpaths.push(rpath);

                            if let Some(v) = map.get_mut(&path.to_owned()) {
                                v.delete_rpath.insert(x.clone());
                            } else {
                                let mut fix = FileFixes::new();
                                fix.delete_rpath.insert(x.clone());
                            }
                        } else {
                            warn!("rpath is not a directory: '{x}'");
                        }
                    }
                    _ => {}
                }
            }
        }
        _ => {}
    }

    fn search_in_rpaths(dylib_name: &std::ffi::OsStr, rpath_dir: &PathBuf) -> Option<String> {
        let abs_path = rpath_dir.join(dylib_name);
        if abs_path.exists() {
            info!("@rpath dylib: {}", abs_path.to_string_lossy());
            return Some(abs_path.to_string_lossy().to_string());
        } else {
            debug!(
                "{} not found in: {}",
                dylib_name.to_string_lossy(),
                rpath_dir.to_string_lossy()
            );
            return None;
        }
    }

    'outer: for dylib in rpath_deps.iter() {
        info!("search rpath dylib: {dylib}");
        let dylib_name = Path::new(dylib).file_name().expect("invalid filepath");

        for dir in rpaths.iter() {
            if let Some(path) = search_in_rpaths(&dylib_name, dir) {
                process_dylib(args, &path, map)?;
                continue 'outer;
            }
        }

        for dir in args.rpaths.iter() {
            if let Some(path) = search_in_rpaths(&dylib_name, dir) {
                process_dylib(args, &path, map)?;
                continue 'outer;
            }
        }

        // exists in dylib parent directory
        if let Some(path) = search_in_rpaths(
            &dylib_name,
            &Path::new(path)
                .parent()
                .expect("parent directory expected...")
                .to_path_buf(),
        ) {
            process_dylib(args, &path, map)?;
            continue 'outer;
        }

        // exists in target directory
        if let Some(path) =
            search_in_rpaths(&dylib_name, &Path::new(args.dir.as_str()).to_path_buf())
        {
            process_dylib(args, &path, map)?;
            continue 'outer;
        }

        return Err(Box::new(DylibError::FileNotFound(dylib.clone())));
    }

    Ok(())
}

fn script_mode(args: &Cli, action_map: HashMap<String, FileFixes>) -> Result<(), Box<dyn Error>> {
    let dir = Path::new(args.dir.as_str());
    println!("#!/bin/sh");
    println!("# this is autogenerated shell script!");
    println!("mkdir -p '{}'", dir.to_string_lossy());

    for (dylib, fix) in action_map.iter() {
        println!("# dylib '{dylib}'");

        let dylib_copy = dir
            .join(Path::new(dylib).file_name().expect("no filename"))
            .to_string_lossy()
            .to_string();

        println!("cp '{dylib}' '{dylib_copy}'");

        let mut cmd = "install_name_tool".to_owned();

        if let Some(new_id) = &fix.fix_id {
            cmd += format!(" -id {new_id}").as_str();
        }

        for (from, to) in fix.fix_rpath.iter() {
            cmd += format!(" -change '{}' '{}'", from, to).as_str();
        }

        for path in fix.delete_rpath.iter() {
            cmd += format!(" -delete_rpath '{path}'").as_str();
        }

        println!("{cmd} '{dylib_copy}'");
    }

    Ok(())
}

fn binary_mode(args: &Cli, action_map: HashMap<String, FileFixes>) -> Result<(), Box<dyn Error>> {
    let dir = Path::new(args.dir.as_str());
    if !dir.exists() {
        std::fs::create_dir_all(dir)?;
    }

    for (dylib, fix) in action_map.iter() {
        println!("{}", format!("{dylib}").blue());
        let dylib_copy = dir
            .join(Path::new(dylib).file_name().expect("no filename"))
            .to_string_lossy()
            .to_string();
        println!("\tcopy {dylib} to {dylib_copy}");
        std::fs::copy(dylib, &dylib_copy)?;

        let mut cmd = Command::new("install_name_tool");

        if let Some(new_id) = &fix.fix_id {
            println!("\tfix id: {}", format!("{new_id}").yellow());
            cmd.args(["-id", new_id]);
        }

        for (from, to) in fix.fix_rpath.iter() {
            println!(
                "\tfix rpath: {} -> {}",
                format!("{from}").red(),
                format!("{to}").green()
            );
            cmd.args(["-change", from, to]);
        }

        for rpath in fix.delete_rpath.iter() {
            println!("\t{} {rpath}", format!("delete rpath:").red());
            cmd.args(["-delete_rpath", rpath]);
        }

        cmd.arg(dylib_copy);

        cmd.output()?;
        let ok = cmd.status().and_then(|x| Ok(x.success()))?;
        if ok {
            println!("\t{}", "Ok".green());
        } else {
            println!("\t{}", "Failed".red());
        }
    }

    Ok(())
}

/// Reads a file and displays the name of each section.
fn main() -> Result<(), Box<dyn Error>> {
    env_logger::init();

    let args = Cli::parse();

    let mut dylib_map = HashMap::new();

    for file in args.files.iter() {
        process_dylib(&args, file.as_str(), &mut dylib_map)?;
    }

    if args.script {
        return script_mode(&args, dylib_map);
    } else {
        return binary_mode(&args, dylib_map);
    }
}
