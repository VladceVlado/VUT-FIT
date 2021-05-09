use std::collections::HashMap;

mod decryption;
mod friedman;
mod input;
mod kasiski;
mod utils;

fn main() {
    let encrypted_text: String = input::get_alphabetic_input();
    let encrypted_text_u8: Vec<u8> = utils::get_text_as_u8(&encrypted_text);
    let letter_frequencies: Vec<i32> = utils::get_letter_frequencies(&encrypted_text_u8);
    let encrypted_text_len: usize = encrypted_text.len();

    // Friedman test
    let friedman: f32 = friedman::friedman_test(&encrypted_text_len, &letter_frequencies);
    print!("{};", friedman);

    // Kasiski test
    let kasiski_map: HashMap<usize, usize> = kasiski::kasiski_test(&encrypted_text);

    // Get the key length
    let key_len: usize = utils::get_key_len(&kasiski_map, friedman);
    print!("{};{};", key_len, key_len);

    // Get the key
    let key: String = decryption::decrypt(&encrypted_text, key_len);
    print!("{}", key);

    println!();
}
