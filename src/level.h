// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#ifndef LEVEL_H
#define LEVEL_H

#include "core.h"
#include "scripting.h"


typedef struct {
    Script *script;
} Level;


Level *level_new(Script *script);

void level_free(Level *level);

void level_load(Level *level);

void level_update(Level *level, double dt);

void level_draw(Level *level);

void level_keyup(Level *level, const char *key);

void level_keydown(Level *level, const char *key);

void level_mouseup(Level *level, const char *button, const char *state, int x, int y);

void level_mousedown(Level *level, const char *button, const char *state, int x, int y);

void level_mousemove(Level *level, const char *state, int x, int y, int relx, int rely);

#endif // LEVEL_H
