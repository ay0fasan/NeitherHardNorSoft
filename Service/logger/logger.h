#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <string.h>
#include "nhns_status_codes.h"

// --- Defines ---
#define LOGGER_LOG(format, ...) Logger_Log(__FILE__, __func__, __LINE__, format, ##__VA_ARGS__)

// --- Functions ---
// TODO: Documentation

nhns_status_t Logger_Init(void);
nhns_status_t Logger_DeInit(void);
void Logger_Log(const char *file, const char *function, int line, const char *format, ...);

#endif    // LOGGER_H
