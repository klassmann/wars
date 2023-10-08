// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#include "error.h"


void panic(const char *fmt, ...) {
    va_list valist;
    vfprintf(stderr, fmt, valist);
    exit(1);
}