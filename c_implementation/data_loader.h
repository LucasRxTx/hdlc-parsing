#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <stdio.h>
#include <stdlib.h>


int read_frames_from_file(char *path, char *buffer, size_t buffer_max_size) {
    FILE *file = fopen(path, "rb");
    if (!file) 
        return -1;

    size_t bytes_read = fread(buffer, 1, buffer_max_size, file);
    fclose(file);
    return bytes_read;

}

size_t get_file_size(char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) 
        return -1;

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fclose(file);
    return file_size;
}

#endif // DATA_LOADER_H
