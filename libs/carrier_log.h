#ifndef CARRIER_LOG_H
#define CARRIER_LOG_H

#include <stdio.h>
#include <string.h>
#include <time.h>

typedef enum {
    CR_ERROR = 0,
    CR_WARNING,
    CR_SUCCESS,
} clog_type;

#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_RED     "\033[31m"

static inline void cr_log(clog_type type, const char* message) {
    const char* color = NULL;

    switch (type) {
        case CR_ERROR: color = COLOR_RED; break;
        case CR_WARNING: color = COLOR_YELLOW; break;
        default: color = COLOR_CYAN; break;
    }

    time_t now = time(NULL);
    char* timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0';

    char trimmed_message[256];
    strncpy(trimmed_message, message, sizeof(trimmed_message) - 1);
    trimmed_message[sizeof(trimmed_message) - 1] = '\0';

    char* newline_pos = strchr(trimmed_message, '\n');
    if (newline_pos) { *newline_pos = '\0'; }

    fprintf(stderr, "%s[CARRIER]: '%s' [%s]%s\n", color, trimmed_message, timestamp, COLOR_RESET);
}

#endif // CARRIER_LOG_H
