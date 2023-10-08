# Copyright 2023 Lucas Klassmann
# License: Apache License 2.0
cmake_minimum_required(VERSION 2.8.12)
project(wars)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -ggdb ${CMAKE_CXX_FLAGS}")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(Lua REQUIRED)


include_directories(
        ${PROJECT_SOURCE_DIR}
        ${SDL2_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        ${LUA_INCLUDE_DIR}
)


set(SOURCE_FILES
        src/main.c
        src/error.c
        src/graphics.c
        src/graphics.h
        src/scripting.c
        src/scripting.h
        src/level.c
        src/level.h
        src/core.h
        src/fonts.c
        src/fonts.h
        src/sound.c
        src/sound.h
        src/game_math.c
        src/game_math.h
        src/utils.c
        src/utils.h
)

# LUA SCRIPTS
configure_file("scripts/game.lua" "scripts/game.lua")
configure_file("scripts/settings.lua" "scripts/settings.lua")
configure_file("scripts/animation.lua" "scripts/animation.lua")
configure_file("scripts/colors.lua" "scripts/colors.lua")
configure_file("scripts/enemy.lua" "scripts/enemy.lua")
configure_file("scripts/nav_grid.lua" "scripts/nav_grid.lua")
configure_file("scripts/player.lua" "scripts/player.lua")
configure_file("scripts/target.lua" "scripts/target.lua")
configure_file("scripts/torpedo.lua" "scripts/torpedo.lua")
configure_file("scripts/utils.lua" "scripts/utils.lua")
configure_file("scripts/timer.lua" "scripts/timer.lua")
configure_file("scripts/scroll_grid.lua" "scripts/scroll_grid.lua")

# ASSETS
configure_file("assets/ships_packed.png" "assets/ships_packed.png" COPYONLY)
configure_file("assets/tiles_packed.png" "assets/tiles_packed.png" COPYONLY)
configure_file("assets/map2.png" "assets/map2.png" COPYONLY)
configure_file("assets/fonts/Kenney Future Narrow.ttf" "assets/fonts/Kenney Future Narrow.ttf" COPYONLY)
configure_file("assets/sfx/laserSmall_001.ogg" "assets/sfx/laserSmall_001.ogg" COPYONLY)
configure_file("assets/sfx/explosion.wav" "assets/sfx/explosion.wav" COPYONLY)
configure_file("assets/music/wars.wav" "assets/music/wars.wav" COPYONLY)

add_executable(wars ${SOURCE_FILES})
target_link_libraries(wars
        ${SDL2_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${LUA_LIBRARIES}
)