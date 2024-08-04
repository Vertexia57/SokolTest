-- spawnPos is set outside of the file

minX = spawnPos.x - 5
maxX = spawnPos.x + 5

AwaitChunkGeneration(minX, maxX)

for x = 0, 11, 1 do
    for y = 0, 11, 1 do
        if (x == 0 or x == 11 or y == 0 or y == 11) then
            SetTile(extraData.ore, spawnPos.x + x, spawnPos.y + y)
        end
    end
end
