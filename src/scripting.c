// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#include "scripting.h"

void debug_stack(lua_State *L) {
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++) {
        printf("%d\t%s\t", i, luaL_typename(L, i));
        switch (lua_type(L, i)) {
            case LUA_TNUMBER:
                printf("%g\n", lua_tonumber(L, i));
                break;
            case LUA_TSTRING:
                printf("%s\n", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
                break;
            case LUA_TNIL:
                printf("%s\n", "nil");
                break;
            default:
                printf("%p\n", lua_topointer(L, i));
                break;
        }
    }
}

int get_integer_field(lua_State *L, const char *table, const char *field) {
    lua_getglobal(L, table);
    lua_pushstring(L, field);
    lua_gettable(L, -2);
    int value = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return value;
}


Script *script_new() {
    Script *script = malloc(sizeof(size_t));
    script->L = luaL_newstate();
    luaL_openlibs(script->L);

    lua_getglobal(script->L, "package");
    lua_getfield(script->L, -1, "path");
    const char *current_path = lua_tostring(script->L, -1);
    const char *custom_path = "./scripts/?.lua;";
    char *path = NULL;
    asprintf(&path, "%s%s", custom_path, current_path);
    lua_pop(script->L, 1);
    lua_pushstring(script->L, path);
    lua_setfield(script->L, -2, "path");
    lua_pop(script->L, 1);
    return script;
}

void script_open_libraries(Script *script) {
    api_graphics_open(script->L);
    api_math_open(script->L);
    api_sound_open(script->L);
    api_font_open(script->L);
}

void script_load(Script *script, const char *filename) {
    if (luaL_dofile(script->L, filename) != LUA_OK) {
        panic(lua_tostring(script->L, lua_gettop(script->L)));
    }
}

const char *script_get_string(Script *script, const char *var) {
    lua_getglobal(script->L, var);

    if (lua_isstring(script->L, -1)) {
        const char *message = lua_tostring(script->L, -1);
        lua_pop(script->L, 1);
        return message;
    }

    return NULL;
}

int script_get_integer(Script *script, const char *var) {
    lua_getglobal(script->L, var);

    if (lua_isinteger(script->L, -1)) {
        int x = lua_tointeger(script->L, -1);
        lua_pop(script->L, 1);
        return x;
    }

    return 0;
}

bool script_get_bool(Script *script, const char *var, bool def) {
    lua_getglobal(script->L, var);

    if (lua_isboolean(script->L, -1)) {
        bool b = lua_toboolean(script->L, -1);
        lua_pop(script->L, 1);
        return b;
    }

    return def;
}

SDL_Color script_get_color(Script *script, const char *var) {
    SDL_Color color;
    color.r = get_integer_field(script->L, var, "r");
    color.g = get_integer_field(script->L, var, "g");
    color.b = get_integer_field(script->L, var, "b");
    return color;
}

void script_free(Script *script) {
    lua_close(script->L);
}