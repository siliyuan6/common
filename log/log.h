#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

// log type
typedef enum {
    LOG_ERROR = 0,
    LOG_WARN,
    LOG_INFO,
    LOG_DBG
} LogLevel;

// default log level
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_DBG
#endif

// color define
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_WHITE   "\x1b[37m"
#define COLOR_RESET   "\x1b[0m"

#define LOG(level, color, stream, fmt, ...) do { \
    if (level <= LOG_LEVEL) { \
        fprintf(stream, color "[%s] " fmt COLOR_RESET , \
                (level == LOG_ERROR) ? "ERROR" : \
                (level == LOG_WARN)  ? "WARN " : \
                (level == LOG_INFO)  ? "INFO " : "DBG", \
                ##__VA_ARGS__); \
    } \
} while (0)

#define LOG_ERROR(fmt, ...) LOG(LOG_ERROR, COLOR_RED,    stderr, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt,  ...) LOG(LOG_WARN,  COLOR_YELLOW, stdout, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt,  ...) LOG(LOG_INFO,  COLOR_GREEN,  stdout, fmt, ##__VA_ARGS__)
#define LOG_DBG(fmt, ...) LOG(LOG_DBG, COLOR_WHITE,  stdout, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // LOG_H