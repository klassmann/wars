-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
Rect = require("core.rect")
Draw = require("core.draw")

Player = {}
Player.__index = Player

function Player.new(sprite, x, y, size)
    local self = setmetatable({}, Player)
    self.sprite = sprite
    self.transform = Rect.new(x, y, size, size)
    return self
end

function Player:translate(position)
    self.transform:position(position)
end

function Player:position()
    return self.transform:position()
end

function Player:draw()
    Draw.draw_sprite(self.sprite, self.transform:position())
end

return Player
