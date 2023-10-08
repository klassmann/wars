// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#include "utils.h"

static int vscprintf(const char *format, va_list ap) {
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int retval = vsnprintf(NULL, 0, format, ap_copy);
    va_end(ap_copy);
    return retval;
}


static int vasprintf(char **strp, const char *format, va_list ap) {
    int len = vscprintf(format, ap);
    if (len == -1)
        return -1;
    char *str = (char *) malloc((size_t) len + 1);
    if (!str)
        return -1;
    int retval = vsnprintf(str, len + 1, format, ap);
    if (retval == -1) {
        free(str);
        return -1;
    }
    *strp = str;
    return retval;
}


int asprintf(char **strp, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    int retval = vasprintf(strp, format, ap);
    va_end(ap);
    return retval;
}

const char *get_mouse_button(SDL_MouseButtonEvent ev) {
    if (ev.button == SDL_BUTTON_RIGHT) {
        return "Right";
    } else if (ev.button == SDL_BUTTON_MIDDLE) {
        return "Middle";
    }
    return "Left";
}

const char *get_mouse_state(SDL_MouseButtonEvent ev) {
    if (ev.button == SDL_PRESSED) {
        return "Pressed";
    }
    return "Released";
}