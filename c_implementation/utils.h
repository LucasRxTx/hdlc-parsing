#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * Print a message to stderr and exit with a non-zero status.
 * 
 * @param pattern The format string.
 * @param ... The arguments to the format string.
*/
void die(const char *pattern, ...) {
    va_list args;
    va_start(args, pattern);
    vfprintf(stderr, pattern, args);
    va_end(args);
    exit(1);
}

#endif // UTILS_H
