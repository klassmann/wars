-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
Rect = require("core.rect")
Sound = require("core.sound")
Draw = require("core.draw")

Torpedo = {}
Torpedo.__index = Torpedo

function Torpedo.new(sprite, position, size)
    local self = setmetatable({}, Torpedo)
    self.sprite = sprite
    self.transform = Rect.new(position:x(), position:y(), size, size)
    self.live = true
    return self
end

function Torpedo:collide(transform)
    self.live = false
end

TorpedoGun = {}
TorpedoGun.__index = TorpedoGun

function TorpedoGun.new(sprite, size, sfx, max_rate)
    local self = setmetatable({}, TorpedoGun)
    self.sprite = sprite
    self.sfx = sfx
    self.size = size
    self.torpedos = {}
    self.max_rate = max_rate
    return self
end

function TorpedoGun:shot(position)
    Sound.play_sfx(self.sfx)
    table.insert(self.torpedos, Torpedo.new(self.sprite, position, self.size))
end

function TorpedoGun:update(direction, t, speed)
    for i, torpedo in ipairs(self.torpedos) do
        torpedo.transform:position(torpedo.transform:position() - direction * (t * speed))
        if not torpedo.live or torpedo.transform:position():y() < 0 then
            table.remove(self.torpedos, i)
        else
            self.torpedos[i] = torpedo
        end
    end
end

function TorpedoGun:draw()
    for i, torpedo in ipairs(self.torpedos) do
        Draw.draw_sprite(torpedo.sprite, torpedo.transform:position())
    end
end

function TorpedoGun:check_collision(gameobject)
    for i, torpedo in ipairs(self.torpedos) do
        is_colliding, side = torpedo.transform:overlaps(gameobject.transform)
        if is_colliding then
            gameobject:collide("bullet")
            torpedo:collide("enemy")
        end
    end
end

return TorpedoGun
