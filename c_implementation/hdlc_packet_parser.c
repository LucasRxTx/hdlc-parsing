#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "yahdlc/C/yahdlc.h"
#include "utils.h"
#include "player.h"
#include "data_loader.h"
#include "player_driver.h"

#define MAX_FRAME_BUFFER_SIZE 2048
#define MAX_SAME_MOVE_COUNT 3
#define MAX_FILTERED_MOVES 255

int main() {
    char *path = "transmission.bin";

    size_t file_size = get_file_size(path);
    if (file_size > MAX_FRAME_BUFFER_SIZE) 
        die("File size is too large\n");


    char frame_buffer[MAX_FRAME_BUFFER_SIZE] = {0};
    size_t bytes_read = read_frames_from_file(path, frame_buffer, MAX_FRAME_BUFFER_SIZE);
    if (bytes_read < 0) 
        die("Could not read file %s\n", path);


    yahdlc_control_t control;
    char frame_data[MAX_FRAME_BUFFER_SIZE] = {0};
    int data_length = 0;
    int state = 0;

    size_t move_buffer_index = 0;
    char move_buffer[255] = {0};

    while (1) {
        int ret = yahdlc_get_data(&control, frame_buffer + state, bytes_read - state, frame_data, &data_length);
        if (ret < 0) {
            break;
        }

        if (control.frame == YAHDLC_FRAME_DATA) 
            move_buffer[move_buffer_index++] = frame_data[0];

        // Move to the next frame
        state += ret;
    }

    player_t player = {{0, 4}, 'x'};
    int filtered_moves[MAX_FILTERED_MOVES];
    int filtered_moves_index = 0;

    int same_move_buffer[MAX_SAME_MOVE_COUNT] = {0};
    int same_move_buffer_index = 0;
    int move = 0;

    // filter out moves that occured 3 times in a row
    for (size_t i = 0; i < move_buffer_index; i++) {
        move = move_buffer[i];
        if (is_same_move(move)) {
            // Add to same move buffer
            same_move_buffer[same_move_buffer_index++] = move;
            if (same_move_buffer_index == MAX_SAME_MOVE_COUNT) {
                // if bufer is full, reset it
                same_move_buffer_index = 0;
            }
            continue;
        } 

        // Exaust same moves buffer
        for (size_t j = 0; j < same_move_buffer_index; j++) {
            filtered_moves[filtered_moves_index++] = same_move_buffer[j];
        }

        // Add current move to same move buffer
        same_move_buffer_index = 0;
        same_move_buffer[same_move_buffer_index++] = move;
    }

    // Consume last moves
    for (size_t j = 0; j < same_move_buffer_index; j++) {
        filtered_moves[filtered_moves_index++] = same_move_buffer[j];
    }

    // Move the player
    for (size_t i = 0; i < filtered_moves_index; i++) {
        player_move(&player, filtered_moves[i]);
    }

    printf("Final player position: (%i, %i)\n", player.position.x, player.position.y);
    return 0;
}