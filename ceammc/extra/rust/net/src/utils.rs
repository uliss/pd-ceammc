use nom::branch::alt;
use nom::bytes::complete::{escaped, tag};
use nom::character::complete::{anychar, none_of, one_of};
use nom::combinator::recognize;
use nom::multi::many0;
use nom::sequence::{delimited, preceded, tuple};
use nom::IResult;

fn parse_double_quoted(input: &str) -> IResult<&str, &str> {
    Ok(delimited(
        tag("\""),
        alt((
            escaped(none_of("\"\\"), '\\', one_of("\"\\")), //
            tag(""),
        )),
        tag("\""),
    )(input)?)
}

fn parse_single_quoted(input: &str) -> IResult<&str, &str> {
    Ok(delimited(
        tag("'"),
        alt((
            escaped(none_of("'\\"), '\\', one_of("'\\")), //
            tag(""),
        )),
        tag("'"),
    )(input)?)
}

fn parse_non_quoted(input: &str) -> IResult<&str, &str> {
    let mut all = recognize(tuple((none_of("\""), many0(anychar))));
    let res = all(input)?;
    Ok(res)
}

pub fn parse_ascii_filename(input: &str) -> IResult<&str, &str> {
    let res = preceded(
        tag("filename="),
        alt((parse_single_quoted, parse_double_quoted, parse_non_quoted)),
    )(input)?;
    Ok(res)
}

fn unescape(str: &str) -> String {
    let mut res: String = String::new();
    res.reserve(str.len());

    for ch in str.chars() {
        if ch != '\\' {
            res.push(ch);
        }
    }

    res
}

pub fn parse_content_disposition_filename(s: &str) -> Option<String> {
    if let Some(i) = s.find("filename=") {
        let f = parse_ascii_filename(&s[i..]).ok()?.1;
        Some(unescape(f))
    } else {
        None
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_ascii_filename() {
        assert_eq!(parse_ascii_filename("filename=\"\"").unwrap(), ("", ""));
        assert_eq!(parse_ascii_filename("filename=\" \"").unwrap(), ("", " "));
        assert_eq!(
            parse_ascii_filename("filename=\"abc\"").unwrap(),
            ("", "abc")
        );
        assert_eq!(
            parse_ascii_filename("filename=\"abc \\\" def\"").unwrap(),
            ("", r#"abc \" def"#)
        );
        assert_eq!(parse_ascii_filename("filename=''").unwrap(), ("", ""));
        assert_eq!(
            parse_ascii_filename("filename='a b c'").unwrap(),
            ("", "a b c")
        );
        assert_eq!(
            parse_ascii_filename("filename='юникод'").unwrap(),
            ("", "юникод")
        );
    }

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
            parse_content_disposition_filename("attachment; filename=\"filename.jpg\""),
            Some("filename.jpg".to_owned())
        );
        assert_eq!(
            parse_content_disposition_filename("attachment; filename=filename.jpg"),
            Some("filename.jpg".to_owned())
        );
        assert_eq!(
            parse_content_disposition_filename("attachment; filename=?"),
            Some("?".to_owned())
        );
        assert_eq!(
            parse_content_disposition_filename("attachment; filename=\"filename_with_\\\".jpg\""),
            Some("filename_with_\".jpg".to_owned())
        );
        assert_eq!(
            parse_content_disposition_filename("attachment; filename=\"filename_with_\\\\a.jpg\""),
            Some("filename_with_a.jpg".to_owned())
        );
        assert_eq!(
            parse_content_disposition_filename("attachment; filename=\"filename_with_;.jpg\""),
            Some("filename_with_;.jpg".to_owned())
        );
        assert_eq!(
            parse_content_disposition_filename("attachment; filename=\"filename=\""),
            Some("filename=".to_owned())
        );
    }
}
