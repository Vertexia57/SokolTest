data:addImages(
	{
		{
			file = "TileData/IronOre.png",
			ID = "ironOre",
			pass = "albedo"
		}
	}
)

data:extend(
	{         
		dataType = "tile",
		
		name = "IronOre",
		ID = "ironOre",
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
			images = { "ironOre" },
			frames = 1,
			variants = 47
		},

		fillsLayers = { true, true, true },

		extraData = {
			mineProduce = "ironOre",
			exhaustTile = "stone"
		},

		extraLocalData = {
			holdingOre = 100,
		}
	}
)
