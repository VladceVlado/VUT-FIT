use std::collections::HashMap;

pub const OCCURRENCE_LIMIT: usize = 3;
const TRIGRAM_LIMIT: usize = 2000;

///
/// Get the length of the trigram according to the text length
///
fn get_trigram_length(text_len: usize) -> usize {
    if text_len < 5000 {
        return 3;
    } else if text_len < 25000 {
        return 4;
    } else {
        return 5;
    }
}

///
/// Get distances among trigrams in the encrypted text
///
fn get_trigram_distances(encrypted_text: &String, trigram: &String) -> Vec<usize> {
    let mut distances: Vec<usize> = Vec::new();

    let mut chars = encrypted_text.chars();

    let trigram_len: usize = get_trigram_length(encrypted_text.len());

    // Variable for temporary storing trigrams
    let mut str: String = String::new();
    for _ in 0..(trigram_len - 1) {
        str.push(chars.next().unwrap());
    }

    for (i, ch) in chars.enumerate() {
        str.push(ch);

        if str == *trigram {
            distances.push(i);
        }

        str.remove(0);
    }

    let mut res: Vec<usize> = Vec::new();

    for i in 1..distances.len() {
        res.push(distances[i] - distances[i - 1]);
    }

    return res;
}

///
/// Kasiski's test
///
pub fn kasiski_test(encrypted_text: &String) -> HashMap<usize, usize> {
    // HashMap<distance, number of occurrences>
    let mut map: HashMap<usize, usize> = HashMap::new();

    let trigram_len: usize = get_trigram_length(encrypted_text.len());

    if encrypted_text.len() <= trigram_len {
        return map;
    }

    // Vector of already seen trigrams
    let mut seen_trigrams: Vec<String> = Vec::new();

    // Encrypted text as chars
    let mut chars = encrypted_text.chars();

    // Variable for temporary storing trigrams
    let mut trigram: String = String::new();
    for _ in 0..(trigram_len - 1) {
        trigram.push(chars.next().unwrap());
    }

    // Iterate through whole encrypted text
    for ch in chars {
        trigram.push(ch);

        // If the trigram was not seen yet, let's investigate
        if !seen_trigrams.contains(&trigram) {
            let distances: Vec<usize> = get_trigram_distances(encrypted_text, &trigram);

            // Update the map for every distance
            for dist in distances {
                let val: &mut usize = map.entry(dist).or_insert(0);
                *val += 1;
            }

            seen_trigrams.push(trigram.clone());
        }

        trigram.remove(0);

        // Seen enough
        if seen_trigrams.len() > TRIGRAM_LIMIT {
            break;
        }
    }

    // Remove the elements under occurence limit
    for (key, val) in map.clone() {
        if val < OCCURRENCE_LIMIT {
            map.remove_entry(&key);
        }
    }

    return map;
}
