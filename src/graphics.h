// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "core.h"

typedef struct {
    int screen_width;
    int screen_height;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
} Graphics;

typedef struct {
    SDL_Texture *texture;
    Uint32 format;
    int access;
    int w;
    int h;
    int sprite_width;
    int sprite_height;
    int cols;
    int rows;
} SpriteSet;

typedef struct {
    SpriteSet *sprite_set;
    int col;
    int row;
    double scale;
    bool flip_h;
    bool flip_v;
} Sprite;


void graphics_init(SDL_Renderer *renderer, int screen_width, int screen_height);

void graphics_quit();

void api_graphics_open(lua_State *L);

#endif // GRAPHICS_H
