-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
Draw = require("core.draw")
Rect = require("core.rect")
Vector = require("core.vector")
Colors = require("colors")
Utils = require("utils")

ScrollGrid = {}
ScrollGrid.__index = ScrollGrid

function ScrollGrid.new(x, y, width, height, size, tiles)
    local self = setmetatable({}, ScrollGrid)
    self.x = x
    self.y = y
    self.width = width
    self.height = height
    self.size = size
    self.tiles = tiles
    self.rects = {}
    self.rows = 0
    return self
end

function ScrollGrid:create()
    local cols = math.ceil(self.width / self.size)
    local rows = math.ceil(self.height / self.size)

    local cols_m = math.fmod(cols, 4)
    local rows_m = math.fmod(rows, 4)
    if cols_m ~= 0.0 then
        cols = cols + cols_m
    end
    if rows_m ~= 0.0 then
        rows = rows + rows_m
    end

    local size = self.size
    local x = self.x
    local y = self.y - self.height / 2

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
            if blockx <= self.width and blocky <= self.height then
                blocks[row][col] = { x = newx, y = newy }
            end
        end
    end

    pos_x = 0
    pos_y = 0

    for row = 1, #blocks do
        self.rects[row] = {}
        for col = 1, #blocks[row] do
            local block = blocks[row][col]
            local sprite = nil

            if math.fmod(col, 2) ~= 0.0 and math.fmod(row, 2) ~= 0.0 then
                sprite = Utils.random_choice(self.tiles.A)
            elseif math.fmod(col, 2) == 0.0 and math.fmod(row, 2) ~= 0.0 then
                sprite = Utils.random_choice(self.tiles.B)
            elseif math.fmod(col, 2) ~= 0.0 and math.fmod(row, 2) == 0.0 then
                sprite = Utils.random_choice(self.tiles.C)
            elseif math.fmod(col, 2) == 0.0 and math.fmod(row, 2) == 0.0 then
                sprite = Utils.random_choice(self.tiles.D)
            end

            self.rects[row][col] = {
                rect = Rect.new(pos_x + block.x, pos_y + block.y, size, size),
                sprite = sprite
            }
        end
    end
    self.rows = #self.rects
end

function ScrollGrid:update(t)
    for row = 1, #self.rects do
        for col = 1, #self.rects[row] do
            r = self.rects[row][col].rect
            r:position(r:position() + Vector.new(0, 1) * t * 200)

            local next = self.rows-row + 1
            local next_block = self.rects[next][col].rect

            if r:position():y() > self.height then
                r:position(Vector.new(next_block:position():x(), self.size * -1))
            end
        end
    end
end

function ScrollGrid:draw()
    for row = 1, #self.rects do
        for col = 1, #self.rects[row] do
            local block = self.rects[row][col]
            Draw.draw_sprite(block.sprite, block.rect:position())
        end
    end
end

return ScrollGrid