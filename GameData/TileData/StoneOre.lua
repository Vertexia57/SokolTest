data:addImages(
	{
		{
			file = "TileData/StoneOre.png",
			ID = "stoneOre",
			pass = "albedo"
		}
	}
)

data:extend(
	{         
		dataType = "tile",
		
		name = "Stone Ore",
		ID = "stoneOre",
		description = "",
		
        empty = false,
		stable = true,
		collidable = true,

        connectionType = "base",
        connectionData = {
            connectionGroup = "BaseMod:Ground",
            connectsTo = { "BaseMod:Ground" }
        },

		imageData = {
			images = { "stoneOre" },
			frames = 1,
			variants = 47
		},

		fillsLayers = { true, true, true },

		extraData = {
			mineProduce = "stoneOre",
			exhaustTile = "stone"
		},

		extraLocalData = {
			holdingOre = 100,
		}
	}
)
