use crate::utils::ALPHA_LEN;

///
/// Compute the parameter ko
///
fn get_ko(text_len: &usize, letter_frequencies: &Vec<i32>) -> f32 {
    let mut result: i32 = 0;

    for i in 0..ALPHA_LEN as usize {
        result += letter_frequencies[i] * (letter_frequencies[i] - 1);
    }

    return result as f32 / (text_len * (text_len - 1)) as f32;
}

///
/// Parameter kp is the same letter probability, returns constant value for English.
///
fn get_kp() -> f32 {
    return 0.067;
}

///
/// Parameter kr is the coincidence probability, returns constant value for English.
///
fn get_kr() -> f32 {
    return 0.0385;
}

///
/// Friedman's test
///
pub fn friedman_test(text_len: &usize, letter_frequencies: &Vec<i32>) -> f32 {
    if *text_len <= 3 {
        return 0.0;
    }

    let ko: f32 = get_ko(text_len, letter_frequencies);
    let kp: f32 = get_kp();
    let kr: f32 = get_kr();

    return (kp - kr) / (ko - kr);
}
