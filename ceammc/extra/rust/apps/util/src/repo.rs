use regex::Regex;
use tagsoup::Document;
use thiserror::Error;

pub const RPI_APT_REPO_URL: &str = "http://repo.poltavski.ru/rpi/pool/contrib/p/pd-ceammc/";

#[derive(Error, Debug)]
pub enum ParseError {
    #[error("HTTP error: {0}")]
    Http(#[from] reqwest::Error),
    #[error("URL parse error: {0}")]
    Url(#[from] url::ParseError),
    #[error("Regex error: {0}")]
    Regex(#[from] regex::Error),
    #[error("No .deb files found on page {0}")]
    NoDebFiles(String),
}

/// Information about a .deb file
#[derive(Debug, Clone, PartialEq)]
pub struct DebFile {
    /// Full filename, e.g. package_1.2.3-4_amd64.deb
    filename: String,
    revision: String,
    version: String,
}

impl DebFile {
    pub fn info(&self) -> String {
        format!("{}.r{}", self.version, self.revision)
    }
}

/// Fetches the page at `url` and returns the list of deb files.
/// This is the synchronous (blocking) counterpart of `parse_deb_page`.
pub fn parse_deb_page(url: &str) -> Result<Vec<DebFile>, ParseError> {
    let client = reqwest::blocking::Client::new();
    let body = client.get(url).send()?.text()?;
    parse_deb_html(&body, url)
}

/// Parses HTML page content using tagsoup.
pub fn parse_deb_html(html: &str, base_url: &str) -> Result<Vec<DebFile>, ParseError> {
    let doc = Document::parse(html);

    // Regex for a deb filename: pd-ceammc-2000.00.r12345.*.deb
    let filename_re =
        Regex::new(r"^pd-ceammc-(?P<version>\d{4}\.\d{2})\.r(?P<revision>\d{4}).*\.deb$")?;

    let base = url::Url::parse(base_url)?;
    let mut result = Vec::new();

    // Query all <a> elements that have an href attribute.
    // Note: invalid selectors panic in tagsoup; a static selector is safe.
    let links = doc.query_selector_all("a[href]");

    for element in links {
        let href = match element.get_attribute_value("href") {
            Some(h) => h,
            None => continue,
        };

        // Build the absolute URL so the filename is extracted correctly.
        let absolute = match base.join(&href) {
            Ok(u) => u,
            Err(_) => continue,
        };

        // Filename from the URL (percent-decoded).
        let filename = match absolute.path_segments().and_then(|s| s.last()) {
            Some(seg) if !seg.is_empty() => percent_encoding::percent_decode_str(seg)
                .decode_utf8_lossy()
                .into_owned(),
            _ => continue,
        };

        if !filename.ends_with(".deb") {
            continue;
        }

        let caps = match filename_re.captures(&filename) {
            Some(c) => c,
            None => continue,
        };

        let version = caps["version"].to_string();
        let revision = caps["revision"].to_string();

        result.push(DebFile {
            filename,
            revision,
            version,
        });
    }

    if result.is_empty() {
        return Err(ParseError::NoDebFiles(base_url.to_string()));
    }

    Ok(result)
}

#[cfg(test)]
mod tests {
    use crate::repo::{parse_deb_page, RPI_APT_REPO_URL};

    #[test]
    fn test_parse_debs() {
        let res = parse_deb_page(RPI_APT_REPO_URL);
        assert!(res.is_ok());
        let res = res.unwrap();
        assert!(!res.is_empty());
    }
}
