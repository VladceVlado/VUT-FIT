use std::{
    io::{self, Read},
    process::exit,
};

///
/// Read the whole input from STDIN and return it as a String
///
fn read_input() -> String {
    let mut text = String::new();

    match io::stdin().read_to_string(&mut text) {
        Ok(_) => {
            return text;
        }
        Err(e) => {
            eprintln!("Error: {}", e);
            exit(1);
        }
    }
}

///
/// Filter all non-alphabetic characters
///
fn get_only_alphabetic(text: &String) -> String {
    let mut processed = String::new();

    for ch in text.to_ascii_lowercase().chars() {
        if ch.is_alphabetic() {
            processed.push(ch);
        }
    }

    return processed;
}

///
/// Get the input from STDIN without non alphabetic characters
///
pub fn get_alphabetic_input() -> String {
    return get_only_alphabetic(&read_input());
}
