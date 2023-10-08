-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
Timer = {}
Timer.__index = Timer

function Timer.new(tag, max_time, callback)
    local self = setmetatable({}, Timer)
    self.tag = tag
    self.current = 0.0
    self.max_time = max_time
    self.callback = callback
    return self
end

function Timer:update(t)
    self.current = self.current + t
    if self.current >= self.max_time then
        self.callback(self.tag)
        self.current = 0.0
    end
end

return Timer