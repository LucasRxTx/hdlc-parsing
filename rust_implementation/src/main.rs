extern crate hdlc;

use hdlc::{SpecialChars, decode};
use std::io::prelude::*;
use std::fs::File;


struct Player {
    x: u8,
    y: u8,
}


fn main() {
    let result = File::open("transmission.bin");
    let mut file = match result {
        Ok(file) => {
            file
        }
        Err(e) => {
            panic!("Error: {}", e);
        }
    };

    // read the file into a buffer
    let mut buffer: Vec<u8> = Vec::new();
    let result = file.read_to_end(&mut buffer);
    match result {
        Ok(_) => {
            println!("Read {} bytes", buffer.len());
        }
        Err(e) => {
            panic!("Error: {}", e);
        }
    }

    // split the buffer into frames
    let mut frames: Vec<Vec<u8>> = Vec::new();
    let mut frame: Vec<u8> = Vec::new();
    let special_chars = SpecialChars::default();
    for byte in buffer {
        if byte == special_chars.fend as u8 {
            if frame.len() > 0 {
                // last flag
                frame.push(byte);
                frames.push(frame);
                frame = Vec::new();
            } else {
                // first flag
                frame.push(byte);
            }
        } else {
            // all other bytes
            frame.push(byte);
        }
    }

    // decode the frames and extract the moves
    let mut moves: Vec<u8> = Vec::new();
    for frame in frames {
        let result = decode(&frame, special_chars);
        match result {
            Ok(decoded) => {
                moves.push(decoded[2]);
            }
            Err(e) => {
                panic!("Error: {}", e);
            }
        }
    }

    // remove moves that are greater than 4
    let mut filtered_moves: Vec<u8> = Vec::new();
    for move_ in moves {
        if move_ > 0x04 {
            continue;
        }
        filtered_moves.push(move_);
    }

    // remove moves that occure more than 3 times in a row
    let mut final_moves: Vec<u8> = Vec::new();
    let mut last_move: u8 = 0;
    let mut move_count: u8 = 0;
    for move_ in filtered_moves {
        if move_count > 0 && move_ == last_move {
            last_move = move_;
            move_count += 1;
            if move_count >= 3 {
                move_count = 0;
            }
            continue;
        }

        for _ in 0..move_count {
            final_moves.push(last_move);
        }

        last_move = move_;
        move_count = 1;
    }

    for _ in 0..move_count {
        final_moves.push(last_move);
    }

    let mut player = Player {
        x: 0,
        y: 4,
    };

    // apply the moves
    for m in final_moves {
        if player.x + m < 5 {
            player.x += m;
        } else if m <= player.x {
            player.x -= m;
        } else if player.y + m < 5 {
            player.y += m;
        } else if m <= player.y {
            player.y -= m;
        } else {
            panic!("Invalid move: {:02x}", m);
        }

        print!("player moved {} to ({}, {})\n", m, player.x, player.y);
    }

    print!("Final position ({}, {})\n", player.x, player.y);
}
