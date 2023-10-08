-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
require("math")
require("io")
require("os")

Rect = require("core.rect")
Vector = require("core.vector")
Sound = require("core.sound")
Font = require("core.font")
Draw = require("core.draw")
Screen = require("core.screen")

Target = require("target")
Player = require("player")
TorpedoGun = require("torpedo")
NavGrid = require("nav_grid")
Colors = require("colors")
Enemy = require("enemy")
Timer = require("timer")
Utils = require("utils")
Animator = require("animation")
ScrollGrid = require("scroll_grid")

-- Load
function _load()
    score = 0
    animator = Animator.new()
    enemy_grid = NavGrid.new(0, 0, Screen.width, Screen.height / 2, 64, Colors.RED)

    enemy_grid:create()
    paths = enemy_grid:find_path()

    ships = Draw.load_sprite_set("assets/ships_packed.png", 32, 32)
    tiles = Draw.load_sprite_set("assets/tiles_packed.png", 16, 16)
    map = Draw.load_sprite_set("assets/map2.png", 128, 128)
    map_tiles = {
        A = {
            Draw.new_sprite(map, 0, 0, 2, false, false),
            Draw.new_sprite(map, 2, 0, 2, false, false),
            Draw.new_sprite(map, 0, 2, 2, false, false),
            Draw.new_sprite(map, 2, 2, 2, false, false),
        },
        B = {
            Draw.new_sprite(map, 1, 0, 2, false, false),
            Draw.new_sprite(map, 3, 0, 2, false, false),
            Draw.new_sprite(map, 1, 2, 2, false, false),
            Draw.new_sprite(map, 3, 2, 2, false, false),
        },
        C = {
            Draw.new_sprite(map, 0, 1, 2, false, false),
            Draw.new_sprite(map, 2, 1, 2, false, false),
            Draw.new_sprite(map, 0, 3, 2, false, false),
            Draw.new_sprite(map, 2, 3, 2, false, false),
        },
        D = {
            Draw.new_sprite(map, 1, 1, 2, false, false),
            Draw.new_sprite(map, 3, 1, 2, false, false),
            Draw.new_sprite(map, 1, 3, 2, false, false),
            Draw.new_sprite(map, 3, 3, 2, false, false),
        }
    }
    scroll_grid = ScrollGrid.new(0, 0, Screen.width * 2, Screen.height * 2, 256, map_tiles)
    scroll_grid:create()

    fontHUD = Font.load("assets/fonts/Kenney Future Narrow.ttf", 64)
    torpedo_sfx = Sound.load_sfx("assets/sfx/laserSmall_001.ogg")
    explosion_sfx = Sound.load_sfx("assets/sfx/explosion.wav")
    level_music = Sound.load_music("assets/music/wars.wav")
    target_sprite = Draw.new_sprite(tiles, 3, 2, 4, false, false)
    torpedo_sprite = Draw.new_sprite(tiles, 1, 0, 4, false, false)
    player_sprite = Draw.new_sprite(ships, 0, 0, 4, false, false)
    ExplosionAnimSprites = {
        Draw.new_sprite(tiles, 3, 10, 4, false, false),
        Draw.new_sprite(tiles, 4, 10, 4, false, false),
        Draw.new_sprite(tiles, 5, 10, 4, false, false),
        Draw.new_sprite(tiles, 6, 10, 4, false, false),
        Draw.new_sprite(tiles, 7, 10, 4, false, false),
        Draw.new_sprite(tiles, 8, 10, 4, false, false),
        Draw.new_sprite(tiles, 9, 10, 4, false, false),
    }

    enemy_sprites = {
        Draw.new_sprite(ships, 0, 1, 4, false, true),
        Draw.new_sprite(ships, 1, 1, 4, false, true),
        Draw.new_sprite(ships, 2, 1, 4, false, true),
        Draw.new_sprite(ships, 3, 1, 4, false, true),
    }

    mouse_target = Target.new(target_sprite, 0, 0, 64)
    player = Player.new(player_sprite, 0, 0, 64)
    torpedo_gun = TorpedoGun.new(torpedo_sprite, 64, torpedo_sfx, 100)

    -- ENEMY
    animator:add_animation("enemy_explosion", ExplosionAnimSprites)
    enemies = {}
    local callback = function(tag)
        local nav = enemy_grid:find_path()
        local enemy = Enemy.new(
                Utils.random_choice(enemy_sprites),
                64,
                nav,
                explosion_sfx,
                animator
        )
        table.insert(enemies, enemy)
    end
    enemies_wave_timer = Timer.new("enemy", 2, callback)
    Sound.play_music(level_music, true)
    print('loaded')
end

-- Key Down
function _keydown(key)

end

-- Key Up
function _keyup(key)

end

-- MouseUp
function _mouseup(button, state, x, y)

end


-- Mouse Down
function _mousedown(button, state, x, y)
    if button == "Left" then
        torpedo_gun:shot(player.transform:center())
    end
end


-- Mouse Move
function _mousemove(state, x, y, relx, rely)
    mouse_target:translate(Vector.new(x, y))
end

-- Update
function _update(t)
    scroll_grid:update(t)
    player:translate(Vector.lerp(player:position(), mouse_target:position(), 0.005 * t * 500))
    torpedo_gun:update(Vector.new(0, 1), t, 800)
    enemies_wave_timer:update(t)
    for idx, e in ipairs(enemies) do
        e:update(0.05, t, 50)
        if e.live then
            torpedo_gun:check_collision(e)
        end
        if not e.live then
            score = score + 10
            table.remove(enemies, idx)
        end
    end
    animator:update(t)
end


-- Draw
function _draw()
    scroll_grid:draw()
    for idx, e in ipairs(enemies) do
        e:draw()
    end
    torpedo_gun:draw()
    animator:draw()
    player:draw()
    mouse_target:draw()
    Draw.draw_text(fontHUD, string.format("SCORE: %d", score), Vector.new(10, 10), Colors.WHITE)
end

