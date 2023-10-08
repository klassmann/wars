// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#include "sound.h"

SoundEffect *sound_load_effect(const char *filename) {
    SoundEffect *sfx = malloc(sizeof(SoundEffect));
    sfx->chunk = Mix_LoadWAV(filename);
    return sfx;
}


SoundMusic *sound_load_music(const char *filename) {
    SoundMusic *music = malloc(sizeof(SoundMusic));
    music->music = Mix_LoadMUS(filename);
    return music;
}

void sound_music_play(SoundMusic *music, bool loop) {
    Mix_PlayMusic(music->music, loop ? -1 : 0);
}

void sound_sfx_play(SoundEffect *sfx, bool loop) {
    Mix_PlayChannel(-1, sfx->chunk, loop ? -1 : 0);
}


int api_load_effect(lua_State *L) {
    const char *filename = luaL_checklstring(L, 1, NULL);
    SoundEffect *sfx = sound_load_effect(filename);
    lua_pushlightuserdata(L, sfx);
    return 1;
}

int api_play_effect(lua_State *L) {
    SoundEffect *sfx = NULL;
    int num_args = lua_gettop(L);
    bool loop = false;

    if (num_args > 0)
        sfx = lua_touserdata(L, 1);

    if (num_args > 1)
        loop = lua_toboolean(L, 8);

    sound_sfx_play(sfx, loop);
    return 0;
}

int api_load_music(lua_State *L) {
    const char *filename = luaL_checklstring(L, 1, NULL);
    SoundMusic *music = sound_load_music(filename);
    lua_pushlightuserdata(L, music);
    return 1;
}

int api_play_music(lua_State *L) {
    SoundMusic *music = NULL;
    int num_args = lua_gettop(L);
    bool loop = false;

    if (num_args > 0)
        music = lua_touserdata(L, 1);

    if (num_args > 1)
        loop = lua_toboolean(L, 2);

    sound_music_play(music, loop);
    return 0;
}

static const struct luaL_Reg sound_funcs[] = {
        {"load_sfx",   api_load_effect},
        {"load_music", api_load_music},
        {"play_sfx",   api_play_effect},
        {"play_music", api_play_music},
        {NULL, NULL}
};

int module_sound(lua_State *L) {
    lua_newtable(L);
    luaL_setfuncs(L, sound_funcs, 0);
    return 1;
}


void api_sound_open(lua_State *L) {
    luaL_requiref(L, "core.sound", module_sound, 0);
    lua_pop(L, 1);
}