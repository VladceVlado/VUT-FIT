use crate::utils::{get_letter_frequencies, ALPHA_LEN, ASCII_A, EN_FREQ_TABLE, IDX_AJ};

///
/// Compute the coincidence index for provided English text
///
fn get_index_coincidence(text: &Vec<u8>) -> f32 {
    let mut result: f32 = 0.0;
    let freqs: Vec<i32> = get_letter_frequencies(text);

    for i in 0..ALPHA_LEN as usize {
        result += EN_FREQ_TABLE[i] * freqs[i] as f32 / text.len() as f32;
    }

    return result;
}

///
/// Decrypt text encrypted by Caesar cipher using brute force
///
fn decrypt_caesar_cipher(encrypted_text: &String) -> char {
    // Length of the encrypted text
    let length: usize = encrypted_text.chars().count();

    // Array of score for every possible key
    let mut scores: [f32; ALPHA_LEN as usize] = [0.0; ALPHA_LEN as usize];

    // Compute the score (EN index coincidence - text index coincidence) for every possible encryption
    for shift in 0..ALPHA_LEN {
        let mut plaintext: Vec<u8> = Vec::with_capacity(length);

        // Decryption
        for ch in encrypted_text.chars() {
            plaintext.push((ALPHA_LEN + ch as u8 - shift - ASCII_A) % ALPHA_LEN);
        }

        let score: f32 = (IDX_AJ - get_index_coincidence(&plaintext)).abs();

        scores[shift as usize] = score;
    }

    let mut best: (usize, f32) = (0, 1.0);

    // Choose the best fitting key
    for i in 0..ALPHA_LEN as usize {
        if scores[i] < best.1 {
            best = (i, scores[i]);
        }
    }

    let subkey: char = (best.0 as u8 + ASCII_A) as char;

    return subkey;
}

///
/// Returns key according to the provided ciphered text and key length
///
pub fn decrypt(encrypted_text: &String, key_len: usize) -> String {
    // Initialize the vector of substrings for every possible key length
    let mut subtexts: Vec<String> = Vec::with_capacity(key_len);

    // Fill the vector - every substring is encrypted by Caesar cipher
    for step in 0..key_len {
        let mut subtext: String = String::with_capacity(encrypted_text.len() / key_len);

        for ch in encrypted_text[step..].chars().step_by(key_len) {
            subtext.push(ch);
        }

        subtexts.push(subtext.clone());
    }

    // Initialize the key
    let mut key: String = String::with_capacity(key_len);

    // Get key chars by decripting Caesar cipher for every substring
    for subtext in subtexts {
        key.push(decrypt_caesar_cipher(&subtext));
    }

    return key;
}
