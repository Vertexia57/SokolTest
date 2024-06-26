local perlin = assert(loadfile(localDir .. "\\Perlin.lua"))()

tileAtlas = { "air", "stone", "ironOre" }
tileEntityAtlas = { "petrifiedTree" }

tileArray = { }
tileEntities = { }

math.randomseed(generatorSeed + chunkX)

function getNoise(x, scale)
    return perlin:noise(x * scale, 0, generatorSeed)
end

function detailNoise(x, y, z, scale, detail)
    local Total = 0
    for i = 0, detail, 1 do
        Total = Total + perlin:noise(x * math.pow(2, i / 2.0) * scale, y * math.pow(2, i / 2.0) * scale, generatorSeed + z) / math.pow(2, i)
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
    local xValue = math.sin((x - 1 + chunkX * chunkWidth) / worldWidth * 2.0 * math.pi) * 10.0
    local yValue = math.cos((x - 1 + chunkX * chunkWidth) / worldWidth * 2.0 * math.pi) * 10.0
    noiseMap[x] = detailNoise(xValue, yValue, 0, 0.2, 2)
    -- cPrint(tostring(x - 1 + chunkX * chunkWidth))
end

local ironOreMap = {}
for y = 1, chunkHeight, 1 do
    ironOreMap[y] = {}
    for x = 1, chunkWidth, 1 do
        local distFromSurface = math.max((y - 1) - (noiseMap[x] * 20.0 + 30.0), 0)
        local depleetion = math.abs(distFromSurface - 5) * 0.05

        xValue = math.sin((x - 1 + chunkX * chunkWidth) / worldWidth * 2.0 * math.pi) * 10.0
        yValue = math.cos((x - 1 + chunkX * chunkWidth) / worldWidth * 2.0 * math.pi) * 10.0
        ironOreMap[y][x] = detailNoise(xValue, yValue, (y - 1) / worldWidth * 40.0 * math.pi, 1.1, 1) - depleetion -- magical fucking value that works for some reason okay thanks maths, somewhere there's 2^2
    end
end

-- Generate terrain
local tileEntityCount = 1

for y = 1, chunkHeight, 1 do
    for x = 1, chunkWidth, 1 do

        actualX = x - 1
        actualY = y - 1

        if (actualY > noiseMap[x] * 20.0 + 30.0) then
            if (ironOreMap[y][x] > 0.85) then
                tileArray[y][x] = 2
            else
                tileArray[y][x] = 1
            end
        else
            tileArray[y][x] = 0
        end
    end
end

for y = 1, chunkHeight, 1 do
    for x = 1, chunkWidth, 1 do

        actualX = x - 1
        actualY = y - 1
        if (math.random() <= 0.3) then
            if (tileArray[y][x] == 0 and tileArray[y + 1][x] == 1 and (tileArray[y][x - 1] == 0 and tileArray[y + 1][x - 1] == 1 or x == 1) and (tileArray[y + 1][x + 1] == 1 and tileArray[y][x + 1] == 0 or x == chunkWidth)) then
                tileEntities[tileEntityCount] = {}
                tileEntities[tileEntityCount][0] = actualY
                tileEntities[tileEntityCount][1] = 0
                tileEntities[tileEntityCount][2] = actualX
                tileEntityCount = tileEntityCount + 1
            end
        end
    end
end

return tileEntities, tileEntityAtlas, tileArray, tileAtlas