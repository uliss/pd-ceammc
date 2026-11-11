#[derive(Debug)]
pub enum Error {
    Error(String),
    Post(String),
    Debug(String),
    Log(String),
}
