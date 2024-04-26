local perlin = assert(loadfile(localDir .. "\\Perlin.lua"))()

tileArray = { }
tileAtlas = { "air", "stone" }

function getNoise(x, scale)
    return perlin:noise(x * scale, 0, generatorSeed)
end

function detailNoise(x, scale, detail)
    local Total = 0
    for i = 0, detail, 1 do
        Total = Total + perlin:noise(x * math.pow(2, i / 2.0) * scale, 0, generatorSeed) / math.pow(2, i)
    end
    return Total
end

-- Create arrays
for y = 1, chunkHeight, 1 do
    tileArray[y] = {}
    for x = 1, chunkWidth, 1 do
        tileArray[y][x] = 0
    end
end

-- Noise Maps
local noiseMap = {}
for x = 1, chunkWidth, 1 do
    noiseMap[x] = detailNoise((x - 1 + chunkX * chunkWidth), 0.05, 2)
    -- cPrint(tostring(x - 1 + chunkX * chunkWidth))
end

-- Generate terrain
for y = 1, chunkHeight, 1 do
    for x = 1, chunkWidth, 1 do

        local actualX = x - 1
        local actualY = y - 1

        if (actualY > noiseMap[x] * 20.0 + 30.0) then
            tileArray[y][x] = 1
        else
            tileArray[y][x] = 0
        end
    end
end

return tileArray, tileAtlas