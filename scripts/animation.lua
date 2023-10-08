-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
Draw = require("core.draw")

Animation = {}
Animation.__index = Animation

function Animation.new(position, frames)
    local self = setmetatable({}, Animation)
    self.position = position
    self.frames = frames
    self.current_frame = 1
    self.max_frames = #frames
    return self
end

function Animation:update(t)
    self.current_frame = self.current_frame + 8 * t
end

function Animation:draw()
    frame = self.frames[math.floor(self.current_frame)]
    Draw.draw_sprite(frame, self.position)
end

Animator = {}
Animator.__index = Animator

function Animator.new()
    local self = setmetatable({}, Animator)
    self.animations = {}
    self.playing = {}
    return self
end

function Animator:add_animation(id, sprites)
    self.animations[id] = sprites
end

function Animator:spawn(id, position)
    local sprites = self.animations[id]
    local anim = Animation.new(position, sprites)
    table.insert(self.playing, anim)
end

function Animator:update(t)
    for idx, anim in ipairs(self.playing) do
        anim:update(t)
        if anim.current_frame >= anim.max_frames then
            table.remove(self.playing, idx)
        end
    end
end

function Animator:draw()
    for idx, anim in ipairs(self.playing) do
        anim:draw()
    end
end

return Animator