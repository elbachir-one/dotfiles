#ifndef LOG_H_
#define LOG_H_

enum LogLevel { LOG_INFO, LOG_ERROR };

void bar_log(enum LogLevel level, const char *fmt, ...);
void log_destroy(void);
int setup_log(void);

#endif // LOG_H_
