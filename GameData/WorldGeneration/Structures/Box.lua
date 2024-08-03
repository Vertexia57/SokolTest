-- spawnPos is set outside of the file

minX = spawnPos.x - 3
maxX = spawnPos.x + 3

AwaitChunkGeneration(minX, maxX)

for x = 0, 6, 1 do
    for y = 0, 6, 1 do
        if (x == 0 or x == 6 or y == 0 or y == 6) then
            SetTile("ironOre", spawnPos.x + x, spawnPos.y + y)
        end
    end
end
