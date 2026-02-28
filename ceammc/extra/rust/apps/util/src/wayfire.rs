use std::fs;

use crate::common::{home_path, Error};

const WAYFIRE_CONFIG_PATH: &'static str = ".config/wayfire.ini";
const WAYFIRE_CONFIG_XCB_SECTION: &'static str = "input";
const WAYFIRE_CONFIG_XCB_LAYOUT: &'static str = "xkb_layout";
const WAYFIRE_CONFIG_XCB_OPTIONS: &'static str = "xkb_options";

pub enum LayoutChange {
    AltShift,
}

#[derive(Debug)]
pub enum LayoutIndicator {
    Caps,
    Scroll,
    Num,
}

impl TryFrom<String> for LayoutIndicator {
    type Error = String;

    fn try_from(value: String) -> Result<Self, Self::Error> {
        match value.as_str() {
            "caps" => Ok(Self::Caps),
            "scroll" => Ok(Self::Scroll),
            "num" => Ok(Self::Num),
            _ => Err(format!("unknown indicator: {value}")),
        }
    }
}

pub struct KeyboardLayout {
    pub layout: Option<String>,
    pub options: Option<String>,
}

impl ToString for LayoutIndicator {
    fn to_string(&self) -> String {
        match self {
            LayoutIndicator::Caps => "caps",
            LayoutIndicator::Scroll => "scroll",
            LayoutIndicator::Num => "num",
        }
        .to_string()
    }
}

pub fn fix_keyboard_layout(indicator: Option<LayoutIndicator>) -> Result<(), crate::common::Error> {
    let cfg = home_path(WAYFIRE_CONFIG_PATH);
    if !cfg.exists() {
        return Err(crate::common::Error::FileNotFound(
            cfg,
            Some("wayfire config".to_owned()),
        ));
    }

    // make backup
    let mut cfg_backup = cfg.clone();
    cfg_backup.add_extension(".bak");
    fs::copy(&cfg, &cfg_backup).map_err(|err| Error::from(&err, "creating backup", &cfg))?;

    let mut ini = ini::Ini::load_from_file(&cfg)
        .map_err(|err| Error::IOError(err.to_string(), "read config".to_string(), cfg.clone()))?;

    ini.set_to(
        Some(WAYFIRE_CONFIG_XCB_SECTION),
        WAYFIRE_CONFIG_XCB_LAYOUT.to_string(),
        "us,ru".to_string(),
    );
    ini.set_to(
        Some(WAYFIRE_CONFIG_XCB_SECTION),
        WAYFIRE_CONFIG_XCB_OPTIONS.to_string(),
        format!(
            "grp:alt_shift_toggle,grp_led:{}",
            indicator.map(|x| x.to_string()).unwrap_or_default()
        ),
    );

    ini.write_to_file(&cfg)
        .map_err(|err| Error::from(&err, "config write", &cfg))
}

pub fn keyboard_layout() -> Result<KeyboardLayout, crate::common::Error> {
    let cfg = home_path(WAYFIRE_CONFIG_PATH);
    if !cfg.exists() {
        return Err(crate::common::Error::FileNotFound(
            cfg,
            Some("wayfire config".to_owned()),
        ));
    }

    let ini = ini::Ini::load_from_file(&cfg)
        .map_err(|err| Error::IOError(err.to_string(), "read config".to_string(), cfg.clone()))?;

    Ok(KeyboardLayout {
        layout: ini
            .get_from(Some(WAYFIRE_CONFIG_XCB_SECTION), WAYFIRE_CONFIG_XCB_LAYOUT)
            .map(|x| x.to_string()),
        options: ini
            .get_from(Some(WAYFIRE_CONFIG_XCB_SECTION), WAYFIRE_CONFIG_XCB_OPTIONS)
            .map(|x| x.to_string()),
    })
}

pub fn output_keyboard_info() {
    match keyboard_layout() {
        Ok(layout) => {
            println!("kbd_layout:   \t{}", layout.layout.unwrap_or_default());
            println!("kbd_options:  \t{}", layout.options.unwrap_or_default());
        }
        Err(err) => crate::common::output_error(&err),
    }
}
