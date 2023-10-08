-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
Draw = require("core.draw")
Rect = require("core.rect")
Vector = require("core.vector")
Colors = require("colors")
Utils = require("utils")

NavGrid = {}
NavGrid.__index = NavGrid

function NavGrid.new(x, y, width, height, size, color)
    local self = setmetatable({}, NavGrid)
    self.x = x
    self.y = y
    self.width = width
    self.height = height
    self.size = size
    self.color = color
    self.rects = {}
    return self
end

function NavGrid:create()
    local cols = math.ceil(self.width / self.size)
    local rows = math.ceil(self.height / self.size)
    local width_pad = math.fmod(self.width, self.size)
    local height_pad = math.fmod(self.height, self.size)
    local size = self.size
    local x = self.x
    local y = self.y

    blocks = {}

    for row = 1, rows do
        blocks[row] = {}
        for col = 1, cols do
            local _row = row - 1
            local _col = col - 1
            local newx = x + (_col * size)
            local newy = y + (_row * size)
            local blockx = newx + size
            local blocky = newy + size
            if blockx < self.width and blocky < self.height then
                blocks[row][col] = { x = newx, y = newy }
            else
                if width_pad == 0 and blockx < self.width then
                    width_pad = blockx
                elseif height_pad == 0 and blocky < self.height then
                    height_pad = blocky
                end
            end
        end
    end
    pos_x = 0
    pos_y = 0

    if width_pad > 0 then
        pos_x = width_pad / 2
    end

    if height_pad > 0 then
        pos_y = height_pad / 2
    end

    for row = 1, #blocks do
        self.rects[row] = {}
        for col = 1, #blocks[row] do
            block = blocks[row][col]
            self.rects[row][col] = Rect.new(pos_x + block.x, pos_y + block.y, size, size)
        end
    end
end

function NavGrid:draw_grid()
    for row = 1, #self.rects do
        for col = 1, #self.rects[row] do
            r = self.rects[row][col]
            Draw.draw_rect(r, self.color)
        end
    end
end

function NavGrid:find_path()
    local paths = {}

    table.insert(paths, Rect.new(math.random(self.width), math.random(self.height / 2) * -1, self.size, self.size))
    for idx, row in ipairs(self.rects) do
        if #row > 1 then
            table.insert(paths, Utils.random_choice(row))
        end
    end
    table.insert(paths, Rect.new(math.random(self.width), (math.random(self.height / 2) + 100) * -1, self.size, self.size))
    return paths
end

return NavGrid