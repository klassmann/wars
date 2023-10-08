// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#ifndef SOUND_H
#define SOUND_H

#include "core.h"

typedef struct {
    Mix_Chunk *chunk;
} SoundEffect;

typedef struct {
    Mix_Music *music;
} SoundMusic;


void api_sound_open(lua_State *L);

#endif // SOUND_H
