// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#ifndef SCRIPTING_H
#define SCRIPTING_H

#include "core.h"
#include "utils.h"
#include "graphics.h"
#include "fonts.h"
#include "sound.h"
#include "game_math.h"

typedef struct {
    lua_State *L;
} Script;


Script *script_new();

void script_open_libraries(Script *script);

void script_load(Script *script, const char *filename);

const char *script_get_string(Script *script, const char *var);

int script_get_integer(Script *script, const char *var);

bool script_get_bool(Script *script, const char *var, bool def);

SDL_Color script_get_color(Script *script, const char *var);

void script_free(Script *script);

int get_integer_field(lua_State *L, const char *table, const char *field);

void debug_stack(lua_State *L);

#endif // SCRIPTING_H
