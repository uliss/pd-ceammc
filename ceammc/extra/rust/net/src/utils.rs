const CONTENT_DISPOSITION_TXT: &str = "Content-Disposition:";
const CONTENT_DISPOSITION_LEN: usize = CONTENT_DISPOSITION_TXT.len();
const FILENAME_TXT: &str = "filename=";
const FILENAME_LEN: usize = FILENAME_TXT.len();

pub fn parse_content_disposition_filename(s: &str) -> Option<String> {
    let mut txt = s;
    if txt.starts_with(CONTENT_DISPOSITION_TXT) {
        txt = &txt[CONTENT_DISPOSITION_LEN..];
    }

    for entry in txt.split(";") {
        if entry.trim().starts_with(FILENAME_TXT) {
            let filename = &entry[FILENAME_LEN+2..entry.len()-1];
            return Some(filename.to_owned());
        }
    }

    None
}

#[cfg(test)]
mod tests {
    use super::parse_content_disposition_filename;

    #[test]
    fn test_parse_content_disposition_filename() {
        assert_eq!(
            parse_content_disposition_filename("Content-Disposition: inline"),
            None
        );
        assert_eq!(
            parse_content_disposition_filename("Content-Disposition: attachment"),
            None
        );
        assert_eq!(
            parse_content_disposition_filename(
                "Content-Disposition: attachment; filename=\"filename.jpg\""
            ),
            Some("filename.jpg".to_owned())
        );
        assert_eq!(
            parse_content_disposition_filename(
                "Content-Disposition: attachment; filename=\"filename.jpg\";"
            ),
            Some("filename.jpg".to_owned())
        );
        assert_eq!(
            parse_content_disposition_filename(
                "Content-Disposition: attachment; filename=\"filename.jpg\"; something"
            ),
            Some("filename.jpg".to_owned())
        );
        assert_eq!(
            parse_content_disposition_filename(
                "attachment; filename=\"filename.jpg\""
            ),
            Some("filename.jpg".to_owned())
        );
    }
}
