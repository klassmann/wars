// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#ifndef FONTS_H
#define FONTS_H

#include "core.h"

typedef struct {
    TTF_Font *font;
    int height;
} Font;


Font *font_load(const char *filename, int size);

SDL_Surface *font_render_shaded(Font *f, const char *text, SDL_Color fg, SDL_Color bg);

SDL_Surface *font_render_solid(Font *f, const char *text, SDL_Color fg);

void api_font_open(lua_State *L);


#endif // FONTS_H
