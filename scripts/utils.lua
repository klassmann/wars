-- Copyright 2023 Lucas Klassmann
-- License: Apache License 2.0
Utils = {}
Utils.__index = Utils
math.randomseed(os.time())

function Utils.random_choice(tb)
    return tb[math.random(#tb)]
end

return Utils