-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
Rect = require("core.rect")
Vector = require("core.vector")
Draw = require("core.draw")
Sound = require("core.sound")
Colors = require("colors")

Navigation = {}
Navigation.__index = Navigation

function Navigation.new(enemy, paths)
    local self = setmetatable({}, Navigation)
    self.enemy = enemy
    self.paths = paths
    self.current = 1
    self.max = #self.paths
    return self
end

function Navigation:next()
    if self.current + 1 <= self.max then
        self.current = self.current + 1
    end
end

function Navigation:update()
    local a = self.enemy.transform:position()
    local b = self:path_position()
    local d = Vector.distance(b, a)
    if math.type(d) == "float" and d <= 100.0 then
        self:next()
    elseif math.type(d) ~= "float" then
        self:next()
    end
end

function Navigation:path_position()
    if self.current > 0 and self.current <= self.max then
        local rect = self.paths[self.current]
        return rect:position()
    end
    return Vector.new(0, 0)
end

Enemy = {}
Enemy.__index = Enemy

function Enemy.new(sprite, size, paths, sfx, animator)
    local self = setmetatable({}, Enemy)
    self.sprite = sprite
    self.transform = Rect.new(0, 0, size, size)
    self.transform:position(paths[1]:position())
    self.nav = Navigation.new(self, paths)
    self.nav.current = 2
    self.live = true
    self.sfx = sfx
    self.animator = animator
    return self
end

function Enemy:collide(tag)
    if tag == "bullet" then
        self.animator:spawn("enemy_explosion", self.transform:position())
        Sound.play_sfx(self.sfx)
        self.live = False
    end
end

function Enemy:update(interpolation, t, speed)
    if self.live then
        self.nav:update()
        self.transform:position(Vector.lerp(self.transform:position(), self.nav:path_position(), interpolation * t * speed))
    end
end

function Enemy:draw()
    if self.live then
        Draw.draw_sprite(self.sprite, self.transform:position())
    end
end

return Enemy