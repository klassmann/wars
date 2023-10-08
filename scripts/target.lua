-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
Rect = require("core.rect")
Draw = require("core.draw")

Target = {}
Target.__index = Target

function Target.new(sprite, x, y, size)
    local self = setmetatable({}, Target)
    self.sprite = sprite
    self.transform = Rect.new(x, y, size, size)
    return self
end

function Target:translate(position)
    self.transform:position(position)
end

function Target:position()
    return self.transform:position()
end

function Target:draw()
    Draw.draw_sprite(self.sprite, self.transform:center())
end

return Target