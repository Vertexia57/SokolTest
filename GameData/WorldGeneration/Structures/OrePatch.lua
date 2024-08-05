-- spawnPos is set outside of the file

radius = 12
minX = spawnPos.x - radius
maxX = spawnPos.x + radius
math.randomseed(spawnPos.x)

AwaitChunkGeneration(minX, maxX)

-- Create Map

map = {}
for y = 1, radius * 2 + 1, 1 do
    map[y] = {}
    for x = 1, radius * 2 + 1, 1 do
        map[y][x] = 0.0
    end
end

-- Generate Circles

circles = 10
for i = 1, circles, 1 do
    x = math.tointeger(math.random(5, radius * 2 + 1 - 5))
    y = math.tointeger(math.random(10, radius * 2 + 1 - 10))  
    ballRadius = math.max(8.0 - math.sqrt(math.pow(radius + 1 - y, 2) + math.pow(radius + 1 - x, 2)), 3.0) 
    for yA = y - 3, y + 3, 1 do
        for xA = x - 3, x + 3, 1 do
            if not (map[yA] == nil) then
                if not (map[yA][xA] == nil) then
                    -- Tile is in bounds

                    if (math.sqrt(math.pow(yA - y, 2) + math.pow(xA - x, 2)) <= ballRadius) then
                        map[yA][xA] = 1.0
                    end

                end
            end
        end
    end
end

-- Smooth circles

newMap = {}
for y = 1, radius * 2 + 1, 1 do
    newMap[y] = {}
    for x = 1, radius * 2 + 1, 1 do
        newMap[y][x] = 0.0
    end
end

for y = 2, radius * 2, 1 do
    for x = 2, radius * 2, 1 do

        local sum = 0.0

        for yA = y - 1, y + 1, 1 do
            for xA = x - 1, x + 1, 1 do
                if not (yA == 0 or xA == 0) then
                    sum = sum + map[yA][xA]
                end
            end
        end

        if (sum / 9.0 > 0.3) then
            newMap[y][x] = 1.0
        else
            newMap[y][x] = 0.0
        end
    end
end

-- Set Tiles

for x = 1, radius * 2 + 1, 1 do
    for y = 1, radius * 2 + 1, 1 do
        if (newMap[y][x] == 1.0) then
            SetTile(extraData.ore, spawnPos.x + x - radius - 1, spawnPos.y + y - radius - 1)
        end
    end
end
