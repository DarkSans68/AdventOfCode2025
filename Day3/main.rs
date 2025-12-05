
use std::env;
use std::fs;

fn main() {
    
    let file_path: String = env::args().nth(1).expect("Please provide a file path as an argument");

    println!("In file {file_path}");

    let contents = fs::read_to_string(file_path)
        .expect("Should have been able to read the file");

    let parts = contents.split_whitespace().collect::<Vec<&str>>();

    let mut g_h = 0;

    for p in parts {
        let chars: Vec<char> = p.chars().collect();

        let mut highest = 0;
        let mut highest_a_pos = 0;
        let mut highest_b_pos = 0;

        for i in 0..chars.len() {
            if let Some(tens) = chars[i].to_digit(10) {
                for j in i + 1..chars.len() {
                    if let Some(ones) = chars[j].to_digit(10) {
                        let candidate = tens * 10 + ones;
                        if candidate > highest {
                            highest = candidate;
                            highest_a_pos = i + 1;
                            highest_b_pos = j + 1; 
                        }
                    }
                }
            }
        }

        g_h += highest;

        println!("In part {p}, highest combined number is {highest} (positions {highest_a_pos}, {highest_b_pos})");
    }

    print!("Grand highest combined number is {g_h}\n");
}
