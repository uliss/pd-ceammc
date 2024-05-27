use clap::Parser;
use indicatif::{ProgressBar, ProgressStyle};
use std::{fs::read_dir, io::Write, path::Path};

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

const LANGUAGES: [&'static str; 14] = [
    "Albanian",
    "Brazilian-Portuguese",
    "Czech",
    "English",
    "Esperanto",
    "Georgian",
    "Kyrgyz",
    "Macedonian",
    "Polish",
    "Russian",
    "Serbian-bin",
    "Slovak",
    "Tatar",
    "Uzbek-bin",
];
const VOICES: [&'static str; 48] = [
    "alan-eng",
    "aleksandr-hq-rus",
    "aleksandr-rus",
    "alicja-pol",
    "anatol-ukr",
    "anna-rus",
    "arina-rus",
    "artemiy-rus",
    "azamat-kir",
    "bdl-eng",
    "cezary-pol",
    "clb-eng",
    "dilnavoz-uzb",
    "dragana-srp",
    "elena-rus",
    "evgeniy-eng",
    "evgeniy-rus",
    "irina-rus",
    "islom-uzb",
    "kiko-mkd",
    "ksp-eng",
    "lyubov-eng",
    "magda-pol",
    "marianna-ukr",
    "michal-pol",
    "mikhail-rus",
    "natalia-ukr",
    "natan-pol",
    "natia-kat",
    "nazgul-kir",
    "ondro-slk",
    "pavel-rus",
    "radek-cze",
    "slt-eng",
    "spomenka-epo",
    "suze-mkd",
    "talgat-tat",
    "tatiana-rus",
    "timofey-rus",
    "umka-rus",
    "victor-rus",
    "victoria-rus",
    "vitaliy-ng-rus",
    "vitaliy-rus",
    "volodymyr-ukr",
    "vsevolod-rus",
    "yuriy-rus",
    "zdenek-cze",
];

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
                    Ok(mut response) => {
                        let fname = url
                            .path_segments()
                            .and_then(|segments| segments.last())
                            .and_then(|name| if name.is_empty() { None } else { Some(name) })
                            .unwrap_or("tmp.bin");

                        log::debug!("file to download: '{}'", fname);

                        let total_size = response.content_length().unwrap_or(0);
                        log::debug!("total size in bytes: {total_size}");

                        match tempfile::tempfile() {
                            Ok(mut file) => {
                                let mut downloaded: u64 = 0;

                                // Indicatif setup
                                let pb = ProgressBar::new(total_size);
                                pb.set_style(ProgressStyle::default_bar()
                                .template("{msg}\n{spinner:.green} [{elapsed_precise}] [{wide_bar:.cyan/blue}] {bytes}/{total_bytes} ({bytes_per_sec}, {eta})").unwrap().progress_chars("#>-"));
                                pb.set_message(format!("Downloading {}", url));

                                while let Ok(chunk) = response.chunk().await {
                                    if let Some(bytes) = chunk {
                                        if let Err(err) = file.write_all(&bytes) {
                                            log::error!("write error: {}", err.to_string());
                                            return false;
                                        }

                                        if total_size > 0 {
                                            downloaded = std::cmp::min(
                                                downloaded + (bytes.len() as u64),
                                                total_size,
                                            );
                                            pb.set_position(downloaded);
                                        }
                                    } else {
                                        break;
                                    }
                                }

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
                                                    let _ = x.map(|x| {
                                                        let file_path =
                                                            base_path.join(x.file_name());
                                                        let _ =
                                                            std::fs::rename(x.path(), file_path);
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
