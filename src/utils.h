// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#ifndef UTILS_H
#define UTILS_H

#include "core.h"

const char *get_mouse_button(SDL_MouseButtonEvent ev);

const char *get_mouse_state(SDL_MouseButtonEvent ev);

int asprintf(char **strp, const char *format, ...);


#endif // UTILS_H
