use std::{fs::read_dir, io::Write, path::Path};

use clap::Parser;

#[derive(Parser)]
#[command(version, about, long_about = None)]
struct Cli {
    /// Voice name
    #[arg(long)]
    voice: Option<String>,
    /// Language name
    #[arg(long)]
    lang: Option<String>,
    /// list available voices
    #[arg(long, action)]
    list_voices: bool,
    /// list available languages
    #[arg(long, action)]
    list_languages: bool,
    /// RHVoice root
    #[arg(long)]
    dest: Option<String>,
}

const LANGUAGES: [&'static str; 2] = ["Russian", "English"];
const VOICES: [&'static str; 2] = ["anna-rus", "victor-rus"];

fn default_download_directory() -> String {
    let mut home = match homedir::get_my_home() {
        Ok(dir) => match dir {
            Some(path) => path,
            None => Path::new(".").to_path_buf(),
        },
        Err(_err) => Path::new(".").to_path_buf(),
    };

    #[cfg(target_os = "macos")]
    {
        home.push("Documents");
        home.push("Pd");
        home.push("rhvoice");
    }

    home.as_os_str().to_string_lossy().to_string()
}

async fn download_latest_release(name: &str, dir: &str) -> bool {
    match octocrab::instance()
        .repos("RHVoice", name)
        .releases()
        .get_latest()
        .await
    {
        Ok(info) => {
            let mut count = 0;

            for x in info
                .assets
                .iter()
                .filter(|x| x.name.ends_with(".zip") || x.name.ends_with(".nvda-addon"))
            {
                let url = x.browser_download_url.clone();
                log::debug!("URL: {url}");
                match reqwest::get(url.clone()).await {
                    Ok(response) => {
                        let fname = url
                            .path_segments()
                            .and_then(|segments| segments.last())
                            .and_then(|name| if name.is_empty() { None } else { Some(name) })
                            .unwrap_or("tmp.bin");

                        log::debug!("file to download: '{}'", fname);

                        match tempfile::tempfile() {
                            Ok(mut file) => match response.bytes().await {
                                Ok(bytes) => {
                                    let _ = file.write(&bytes);
                                    let _ = file.flush();
                                    match zip::ZipArchive::new(file) {
                                        Ok(mut zip) => {
                                            if let Err(err) = zip.extract(dir) {
                                                log::error!("zip error: {err}");
                                                return false;
                                            } else {
                                                log::debug!("extracted into: {dir}");
                                                let base_path = Path::new(dir).to_path_buf();
                                                let data_path = base_path.join("data");
                                                if data_path.exists() {
                                                    for x in read_dir(data_path).unwrap() {
                                                        let _  = x.map(|x| {
                                                            let file_path = base_path.join(x.file_name());
                                                            let _ = std::fs::rename(x.path(), file_path);
                                                        });
                                                    }
                                                }


                                                count += 1;
                                            }
                                        }
                                        Err(err) => {
                                            log::error!("zip error: {err}");
                                            return false;
                                        }
                                    }
                                }
                                Err(err) => {
                                    log::error!("failed to download '{fname}' {err}");
                                    return false;
                                }
                            },
                            Err(err) => {
                                log::error!("can't create file: {err}");
                                return false;
                            }
                        }
                    }
                    //
                    // copy(&mut content.as_bytes(), &mut dest)?;
                    Err(err) => {
                        log::error!("can't download file: {err}");
                        return false;
                    }
                }
            }

            if count == 0 {
                log::error!("no archives found");
                return false;
            }
            // log::debug!("{info:?}");
            true
        }
        Err(err) => {
            log::error!("{err}");
            false
        }
    }
}

async fn download_voice(name: &str, dir: &str) -> bool {
    log::debug!("download voice: {name}");
    download_latest_release(name, format!("{dir}/voices/{name}").as_str()).await
}

async fn download_language(name: &str, dir: &str) -> bool {
    log::debug!("download language: {name}");
    download_latest_release(name, format!("{dir}/languages/{name}").as_str()).await
}

#[tokio::main(flavor = "current_thread")]
async fn main() {
    env_logger::init();

    let args = Cli::parse();

    if args.list_voices {
        for v in VOICES {
            println!("{v}");
        }
        std::process::exit(exitcode::OK);
    }

    if args.list_languages {
        for lang in LANGUAGES {
            println!("{lang}");
        }
        std::process::exit(exitcode::OK);
    }

    let dir = match args.dest {
        Some(dir) => dir,
        None => default_download_directory(),
    };

    if args.voice.is_some() {
        download_voice(args.voice.unwrap().as_str(), dir.as_str()).await;
    }

    if args.lang.is_some() {
        download_language(args.lang.unwrap().as_str(), dir.as_str()).await;
    }

    std::process::exit(exitcode::OK);
}
