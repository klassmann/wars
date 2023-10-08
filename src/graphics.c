// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#include "graphics.h"
#include "scripting.h"
#include "fonts.h"

static Graphics *graphics;

void graphics_init(SDL_Renderer *renderer, int screen_width, int screen_height) {
    if (graphics == NULL) {
        graphics = malloc(sizeof(Graphics));
    }
    graphics->screen_width = screen_width;
    graphics->screen_height = screen_height;
    graphics->renderer = renderer;
}


void graphics_draw_line(Vector start, Vector end, SDL_Color color) {
    if (graphics == NULL)
        panic("graphics: not initialized");

    SDL_SetRenderDrawColor(
            graphics->renderer,
            color.r,
            color.g,
            color.b,
            SDL_ALPHA_OPAQUE
    );

    SDL_RenderDrawLineF(graphics->renderer, start.x, start.y, end.w, end.h);
}

void graphics_draw_rect(Rect rect, SDL_Color color) {
    if (graphics == NULL)
        panic("graphics: not initialized");

    SDL_SetRenderDrawColor(
            graphics->renderer,
            color.r,
            color.g,
            color.b,
            SDL_ALPHA_OPAQUE
    );
    SDL_FRect r = {rect.x, rect.y, rect.w, rect.h};;
    SDL_RenderDrawRectF(graphics->renderer, &r);
}

void graphics_draw_fill_rect(Rect rect, SDL_Color color) {
    if (graphics == NULL)
        panic("graphics: not initialized");

    SDL_SetRenderDrawColor(
            graphics->renderer,
            color.r,
            color.g,
            color.b,
            SDL_ALPHA_OPAQUE
    );
    SDL_FRect r = {rect.x, rect.y, rect.w, rect.h};;
    SDL_RenderFillRectF(graphics->renderer, &r);
}

SpriteSet *graphics_load_sprite_set(const char *filename, int sprite_w, int sprite_h) {
    SpriteSet *atlas = malloc(sizeof(SpriteSet));
    atlas->sprite_width = sprite_w;
    atlas->sprite_height = sprite_h;
    atlas->texture = IMG_LoadTexture(graphics->renderer, filename);

    if (atlas->texture == NULL) {
        printf("error on loading sprite atlas: %s\n", IMG_GetError());
        free(atlas);
        return NULL;
    }

    SDL_QueryTexture(atlas->texture, &atlas->format, &atlas->access, &atlas->w, &atlas->h);
    atlas->cols = atlas->w / atlas->sprite_width;
    atlas->rows = atlas->h / atlas->sprite_height;
    return atlas;
}

void graphics_free_sprite(Sprite *sprite) {
    free(sprite);
}


void graphics_draw_sprite_set(SpriteSet *atlas, Vector pos, int col, int row, float scale, bool flip_h, bool flip_v) {
    int src_col = col * atlas->sprite_width;
    int src_row = row * atlas->sprite_height;
    SDL_Rect src = {src_col, src_row, atlas->sprite_width, atlas->sprite_height};

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (flip_h)
        flip |= SDL_FLIP_HORIZONTAL;

    if (flip_v)
        flip |= SDL_FLIP_VERTICAL;

    SDL_FRect dst = {pos.x, pos.y, atlas->sprite_width * scale, atlas->sprite_height * scale};
    SDL_RenderCopyExF(graphics->renderer, atlas->texture, &src, &dst, 0, NULL, flip);
}

void graphics_draw_text(Font *f, const char *text, Vector pos, SDL_Color fg, bool shaded, SDL_Color bg) {
    SDL_Surface *sur;
    SDL_Texture *texture;
    if (shaded) {
        sur = font_render_shaded(f, text, fg, bg);
        texture = SDL_CreateTextureFromSurface(graphics->renderer, sur);
    } else {
        sur = font_render_solid(f, text, fg);
        texture = SDL_CreateTextureFromSurface(graphics->renderer, sur);
    }

    SDL_Rect dst = {pos.x, pos.y, sur->w, sur->h};
    SDL_FreeSurface(sur);

    if (texture != NULL && sur != NULL) {
        SDL_RenderCopy(graphics->renderer, texture, NULL, &dst);
        SDL_DestroyTexture(texture);
    }
}

void graphics_quit() {
    free(graphics);
}

static SDL_Color lua_read_color(lua_State *L, int idx) {
    SDL_Color c;
    if (lua_istable(L, idx)) {
        lua_getfield(L, idx, "r");
        lua_getfield(L, idx, "g");
        lua_getfield(L, idx, "b");
        c.r = lua_tointeger(L, idx + 1);
        c.g = lua_tointeger(L, idx + 2);
        c.b = lua_tointeger(L, idx + 3);
        lua_pop(L, 3);
    }
    return c;
}


int api_load_sprite_atlas(lua_State *L) {
    const char *filename = luaL_checklstring(L, 1, NULL);
    int sprite_width = luaL_checkinteger(L, 2);
    int sprite_height = luaL_checkinteger(L, 3);
    SpriteSet *atlas = graphics_load_sprite_set(filename, sprite_width, sprite_height);
    lua_pushlightuserdata(L, atlas);
    return 1;
}

int api_new_sprite(lua_State *L) {
    SpriteSet *set = NULL;
    int num_args = lua_gettop(L);
    int col = 0;
    int row = 0;
    lua_Number scale = 1;
    bool flip_h = false;
    bool flip_v = false;

    if (num_args > 0)
        set = lua_touserdata(L, 1);

    if (num_args > 1)
        col = luaL_checkinteger(L, 2);

    if (num_args > 2)
        row = luaL_checkinteger(L, 3);

    if (num_args > 3)
        scale = luaL_checknumber(L, 4);

    if (num_args > 4)
        flip_h = lua_toboolean(L, 5);

    if (num_args > 5)
        flip_v = lua_toboolean(L, 6);

    Sprite *sprite = lua_newuserdata(L, sizeof(Sprite));
    sprite->sprite_set = set;
    sprite->col = col;
    sprite->row = row;
    sprite->scale = scale;
    sprite->flip_h = flip_h;
    sprite->flip_v = flip_v;

    return 1;
}

int api_draw_sprite_set(lua_State *L) {
    SpriteSet *atlas = NULL;
    int num_args = lua_gettop(L);
    Vector *position;
    int col = 0;
    int row = 0;
    lua_Number scale = 1;
    bool flip_h = false;
    bool flip_v = false;

    if (num_args > 0)
        atlas = lua_touserdata(L, 1);

    if (num_args > 1)
        position = luaL_checkudata(L, 2, "Vector");

    if (num_args > 2)
        col = luaL_checkinteger(L, 3);

    if (num_args > 3)
        row = luaL_checkinteger(L, 4);

    if (num_args > 4)
        scale = luaL_checknumber(L, 5);

    if (num_args > 5)
        flip_h = lua_toboolean(L, 6);

    if (num_args > 6)
        flip_v = lua_toboolean(L, 7);

    graphics_draw_sprite_set(atlas, *position, col, row, scale, flip_h, flip_v);
    return 0;
}


int api_draw_sprite(lua_State *L) {
    Sprite *sprite = NULL;
    int num_args = lua_gettop(L);
    Vector *position;

    if (num_args > 0)
        sprite = lua_touserdata(L, 1);

    if (num_args > 1)
        position = luaL_checkudata(L, 2, "Vector");

    graphics_draw_sprite_set(
            sprite->sprite_set,
            *position,
            sprite->col,
            sprite->row,
            sprite->scale,
            sprite->flip_h,
            sprite->flip_v
    );

    return 0;
}

int api_draw_text(lua_State *L) {
    Font *font = NULL;
    int num_args = lua_gettop(L);
    Vector *position;
    SDL_Color fg;
    SDL_Color bg;
    bool shaded = false;
    const char *text = NULL;

    if (num_args > 0)
        font = lua_touserdata(L, 1);

    if (num_args > 1)
        text = luaL_checkstring(L, 2);

    if (num_args > 2)
        position = luaL_checkudata(L, 3, "Vector");

    if (num_args > 3)
        fg = lua_read_color(L, 4);

    if (num_args > 4)
        shaded = lua_toboolean(L, 5);

    if (num_args > 5)
        bg = lua_read_color(L, 6);

    graphics_draw_text(font, text, *position, fg, shaded, bg);
    return 0;
}

int api_draw_rect(lua_State *L) {
    Rect *r = luaL_checkudata(L, 1, "Rect");
    SDL_Color color = lua_read_color(L, 2);
    graphics_draw_rect(*r, color);
    return 0; // Successful
}

int api_draw_fill_rect(lua_State *L) {
    Rect *r = luaL_checkudata(L, 1, "Rect");
    SDL_Color color = lua_read_color(L, 2);
    graphics_draw_fill_rect(*r, color);
    return 0; // Successful
}

static const struct luaL_Reg drawing_funcs[] = {
        {"load_sprite_set", api_load_sprite_atlas},
        {"draw_sprite_set", api_draw_sprite_set},
        {"new_sprite",      api_new_sprite},
        {"draw_sprite",     api_draw_sprite},
        {"draw_text",       api_draw_text},
        {"draw_rect",       api_draw_rect},
        {"draw_fill_rect",  api_draw_fill_rect},
        {NULL, NULL}
};

int module_draw(lua_State *L) {
    lua_newtable(L);
    luaL_setfuncs(L, drawing_funcs, 0);
    return 1;
}

int module_screen(lua_State *L) {
    lua_newtable(L);
    int pos = lua_gettop(L);
    lua_pushinteger(L, graphics->screen_width);
    lua_setfield(L, pos, "width");
    lua_pushinteger(L, graphics->screen_height);
    lua_setfield(L, pos, "height");

    return 1;
}

void api_graphics_open(lua_State *L) {
    luaL_requiref(L, "core.draw", module_draw, 0);
    lua_pop(L, 1);

    luaL_requiref(L, "core.screen", module_screen, 0);
    lua_pop(L, 1);
}

