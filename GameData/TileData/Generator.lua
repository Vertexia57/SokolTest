local perlin = assert(loadfile(localDir .. "\\Perlin.lua"))()

tileAtlas = { "air", "stone" }
tileEntityAtlas = { "petrifiedTree" }

tileArray = { }
tileEntities = { }

math.randomseed(generatorSeed + chunkX)

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
local tileEntityCount = 1

for y = 1, chunkHeight, 1 do
    for x = 1, chunkWidth, 1 do

        actualX = x - 1
        actualY = y - 1

        if (actualY > noiseMap[x] * 20.0 + 30.0) then
            tileArray[y][x] = 1

            if (math.random() >= 0.5) then
                tileEntities[tileEntityCount] = {}
                tileEntities[tileEntityCount][0] = actualY - 1
                tileEntities[tileEntityCount][1] = 0
                tileEntities[tileEntityCount][2] = actualX
                tileEntityCount = tileEntityCount + 1
            end
        else
            tileArray[y][x] = 0
        end
    end
end

return tileEntities, tileEntityAtlas, tileArray, tileAtlas