// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#include "fonts.h"


Font *font_load(const char *filename, int size) {
    Font *f = malloc(sizeof(Font));
    f->font = TTF_OpenFont(filename, size);
    f->height = TTF_FontHeight(f->font);
    return f;
}

SDL_Surface *font_render_shaded(Font *f, const char *text, SDL_Color fg, SDL_Color bg) {
    return TTF_RenderText_Shaded(f->font, text, fg, bg);
}

SDL_Surface *font_render_solid(Font *f, const char *text, SDL_Color fg) {
    return TTF_RenderText_Solid(f->font, text, fg);
}

int api_font_load(lua_State *L) {
    const char *filename = luaL_checklstring(L, 1, NULL);
    int font_size = luaL_checkinteger(L, 2);

    Font *font = font_load(filename, font_size);

    lua_pushlightuserdata(L, font);
    return 1;
}

static const struct luaL_Reg font_funcs[] = {
        {"load", api_font_load},
        {NULL, NULL}
};

int module_font(lua_State *L) {
    lua_newtable(L);
    luaL_setfuncs(L, font_funcs, 0);
    return 1;
}

void api_font_open(lua_State *L) {
    luaL_requiref(L, "core.font", module_font, 0);
    lua_pop(L, 1);
}