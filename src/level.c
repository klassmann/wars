// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#include "level.h"

Level *level_new(Script *script) {
    Level *l = malloc(sizeof(Level));
    l->script = script;
    return l;
}


void level_free(Level *level) {
    free(level);
}

void level_load(Level *level) {
    lua_State *L = level->script->L;
    lua_getglobal(L, "_load");
    lua_pcall(L, 0, 0, 0);
}

void level_update(Level *level, double dt) {
    lua_State *L = level->script->L;
    lua_getglobal(L, "_update");
    lua_pushnumber(level->script->L, dt);
    lua_pcall(L, 1, 0, 0);
}

void level_draw(Level *level) {
    lua_State *L = level->script->L;
    lua_getglobal(L, "_draw");
    lua_pcall(L, 0, 0, 0);
}

void level_keyup(Level *level, const char *key) {
    lua_State *L = level->script->L;
    lua_getglobal(L, "_keyup");
    lua_pushstring(level->script->L, key);
    lua_pcall(L, 1, 0, 0);
}

void level_keydown(Level *level, const char *key) {
    lua_State *L = level->script->L;
    lua_getglobal(L, "_keydown");
    lua_pushstring(level->script->L, key);
    lua_pcall(L, 1, 0, 0);
}

void level_mouseup(Level *level, const char *button, const char *state, int x, int y) {
    lua_State *L = level->script->L;
    lua_getglobal(L, "_mouseup");
    lua_pushstring(level->script->L, button);
    lua_pushstring(level->script->L, state);
    lua_pushinteger(level->script->L, x);
    lua_pushinteger(level->script->L, y);
    lua_pcall(L, 4, 0, 0);
}

void level_mousedown(Level *level, const char *button, const char *state, int x, int y) {
    lua_State *L = level->script->L;
    lua_getglobal(L, "_mousedown");
    lua_pushstring(level->script->L, button);
    lua_pushstring(level->script->L, state);
    lua_pushinteger(level->script->L, x);
    lua_pushinteger(level->script->L, y);
    lua_pcall(L, 4, 0, 0);
}

void level_mousemove(Level *level, const char *state, int x, int y, int relx, int rely) {
    lua_State *L = level->script->L;
    lua_getglobal(L, "_mousemove");
    lua_pushstring(level->script->L, state);
    lua_pushinteger(level->script->L, x);
    lua_pushinteger(level->script->L, y);
    lua_pushinteger(level->script->L, relx);
    lua_pushinteger(level->script->L, rely);
    lua_pcall(L, 5, 0, 0);
}