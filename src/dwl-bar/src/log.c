#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

FILE *log_file = NULL;

void bar_log(enum LogLevel level, const char *fmt, ...) {
    if (!log_file)
        return;

    va_list ap;
    va_start(ap, fmt);
    switch (level) {
        case LOG_INFO:
            fprintf(log_file, "[dwl-bar] info: ");
            break;
        case LOG_ERROR:
            fprintf(log_file, "[dwl-bar] error: ");
            break;
    }
    vfprintf(log_file, fmt, ap);
    va_end(ap);
    fputc('\n', log_file);

    fflush(log_file);
}

void log_destroy(void) {
    if (!log_file)
        return;

    fclose(log_file);
}

int setup_log(void) {
    log_file = fopen("bar.log", "w");
    if (!log_file)
        return 0;

    bar_log(LOG_INFO, "Setup Logging");

    return 1;
}
