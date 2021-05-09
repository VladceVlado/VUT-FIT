use crate::kasiski::OCCURRENCE_LIMIT;
use std::collections::HashMap;

///
/// Public constants
///
pub const ALPHA_LEN: u8 = 26;
pub const ASCII_A: u8 = 97;
pub const IDX_AJ: f32 = 0.065;
pub const EN_FREQ_TABLE: [f32; ALPHA_LEN as usize] = [
    0.0805, 0.0162, 0.0320, 0.0365, 0.1231, 0.0228, 0.0161, 0.0514, 0.0718, 0.0010, 0.0052, 0.0403,
    0.0225, 0.0719, 0.0794, 0.0229, 0.0020, 0.0603, 0.0659, 0.0959, 0.0310, 0.0093, 0.0203, 0.0020,
    0.0188, 0.0009,
];

///
/// Private constants
///
const MAX_KEY_LEN: usize = 150;
const FRIEDMAN_KASISKI_THRESHOLD: usize = 10;

///
/// Compute the letter frequencies in the text
///
pub fn get_letter_frequencies(text: &Vec<u8>) -> Vec<i32> {
    let mut freqs: Vec<i32> = vec![0; ALPHA_LEN as usize];

    for ch in text.clone() {
        freqs[ch as usize] += 1;
    }

    return freqs;
}

///
/// Get text representation as u8 (0 - 25)
///
pub fn get_text_as_u8(text: &String) -> Vec<u8> {
    let mut text_u8: Vec<u8> = Vec::with_capacity(text.len());

    for ch in text.chars() {
        text_u8.push((ALPHA_LEN + ch as u8 - ASCII_A) % ALPHA_LEN);
    }

    return text_u8;
}

///
/// Compute the key length according to the Kasiski and Friedman examinations
///
pub fn get_key_len(kasiski_map: &HashMap<usize, usize>, friedman: f32) -> usize {
    let mut key_len: usize = extract_key_len(kasiski_map);

    // Compute the Kasiski / Friedman ratio
    let ratio: usize = if key_len > friedman as usize {
        if friedman as usize == 0 {
            return 0;
        }
        key_len / friedman as usize
    } else {
        if key_len == 0 {
            return 0;
        }
        friedman as usize / key_len
    };

    if ratio > FRIEDMAN_KASISKI_THRESHOLD {
        let mut new_kasiski_map: HashMap<usize, usize> = kasiski_map.clone();

        // Remove the elements under occurence limit
        for (key, val) in new_kasiski_map.clone() {
            if val < OCCURRENCE_LIMIT + 1 {
                new_kasiski_map.remove_entry(&key);
            }
        }

        key_len = extract_key_len(&new_kasiski_map);
    }

    return key_len;
}

///
/// Compute the GCD
///
fn gcd(first: usize, second: usize) -> usize {
    let mut max: usize = first;
    let mut min: usize = second;

    if min > max {
        let val: usize = max;
        max = min;
        min = val;
    }

    loop {
        let res: usize = max % min;
        if res == 0 {
            return min;
        }

        max = min;
        min = res;
    }
}

///
/// Extract the best key length according to the Kasiski examination
///
fn extract_key_len(kasiski_map: &HashMap<usize, usize>) -> usize {
    if kasiski_map.is_empty() {
        return 0;
    }

    // Buffer of numbers to be computed key length based on
    let mut buffer = kasiski_map.keys();

    // Compute the key length as a GCD of numbers in buffer
    let mut key_len: usize = *buffer.next().unwrap();
    for num in buffer {
        key_len = gcd(key_len, *num)
    }

    // Keys longer than MAX_KEY_LEN are not expected
    if key_len > MAX_KEY_LEN {
        for i in (1..MAX_KEY_LEN).rev() {
            if key_len % i == 0 {
                key_len = i;
                break;
            }
        }
    }

    return key_len;
}
