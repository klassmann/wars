// Copyright 2023 Lucas Klassmann
// License: Apache License 2.0
#include "core.h"
#include "scripting.h"
#include "level.h"

SDL_Window *window;
SDL_Renderer *renderer;
int screen_width = 0;
int screen_height = 0;

typedef enum {
    GAME_RUNNING, GAME_PAUSE, GAME_QUIT
} GameState;

GameState state = GAME_RUNNING;

int main(int argc, char **argv) {
    Script *settings = script_new();
    script_load(settings, "scripts/settings.lua");

    const char *title = script_get_string(settings, "title");
    screen_width = script_get_integer(settings, "screen_width");
    screen_height = script_get_integer(settings, "screen_height");
    const bool show_cursor = script_get_bool(settings, "show_cursor", true);
    const bool quality_linear = script_get_bool(settings, "quality_linear", false);
    const bool full_screen = script_get_bool(settings, "full_screen", false);
    const bool mouse_grab = script_get_bool(settings, "mouse_grab", false);
    SDL_Color background = script_get_color(settings, "background");

    ////////////// INIT

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        panic("Could not initialize SDL_Init: %s\n", SDL_GetError());
    }

    Uint32 window_flags = full_screen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN;
    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen_width,
                              screen_height,
                              window_flags);

    if (window == NULL) {
        panic("Could not initialize Window: %s\n", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        panic("Could not initialize Renderer: %s\n", SDL_GetError());
    }

    if (quality_linear)
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    SDL_GetRendererOutputSize(renderer, &screen_width, &screen_height);
    graphics_init(renderer, screen_width, screen_height);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        panic("Could not initialize image system: %s\n", IMG_GetError());
    }

    if (TTF_Init() == -1) {
        panic("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        panic("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    SDL_ShowCursor(show_cursor ? 1 : 0);
    SDL_SetWindowMouseGrab(window, mouse_grab ? SDL_TRUE : SDL_FALSE);

    Script *level1 = script_new();
    script_open_libraries(level1);

    script_load(level1, "scripts/game.lua");

    Level *level = NULL;
    level = level_new(level1);

    SDL_Event ev;

    double deltaTime = 0.0;
    double lastTime = (double) SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
    double currentTime;

    level_load(level);

    while (state != GAME_QUIT) {
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                state = GAME_QUIT;
                continue;
            } else if (ev.type == SDL_KEYUP) {
                if (state == GAME_RUNNING)
                    level_keyup(level, SDL_GetScancodeName(ev.key.keysym.scancode));
            } else if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    state = GAME_QUIT;
                    continue;
                } else if (ev.key.keysym.scancode == SDL_SCANCODE_F5) {
                    if (state == GAME_RUNNING)
                        state = GAME_PAUSE;
                    else
                        state = GAME_RUNNING;
                }

                if (state == GAME_RUNNING)
                    level_keydown(level, SDL_GetScancodeName(ev.key.keysym.scancode));
            } else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                int x = ev.button.x;
                int y = ev.button.y;

                if (state == GAME_RUNNING)
                    level_mousedown(level, get_mouse_button(ev.button), get_mouse_state(ev.button), x, y);
            } else if (ev.type == SDL_MOUSEBUTTONUP) {
                int x = ev.button.x;
                int y = ev.button.y;

                if (state == GAME_RUNNING)
                    level_mouseup(level, get_mouse_button(ev.button), get_mouse_state(ev.button), x, y);
            } else if (ev.type == SDL_MOUSEMOTION) {
                int x = ev.motion.x;
                int y = ev.motion.y;
                int relx = ev.motion.xrel;
                int rely = ev.motion.yrel;

                if (state == GAME_RUNNING)
                    level_mousemove(level, get_mouse_state(ev.button), x, y, relx, rely);
            }
        }

        if (state == GAME_RUNNING) {
            currentTime = (double) SDL_GetPerformanceCounter() / (double) SDL_GetPerformanceFrequency();
            deltaTime = currentTime - lastTime;
            lastTime = currentTime;
            level_update(level, deltaTime);

            SDL_SetRenderDrawColor(
                    renderer,
                    background.r,
                    background.g,
                    background.b,
                    0xFF
            );

            SDL_RenderClear(renderer);
            level_draw(level);
            SDL_RenderPresent(renderer);
        }
    }

    level_free(level);
    script_free(level1);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    graphics_quit();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}

