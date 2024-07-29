data:addImages(
	{
		{
			file = "TileData/CopperOre.png",
			ID = "copperOre",
			pass = "albedo"
		}
	}
)

data:extend(
	{         
		dataType = "tile",
		
		name = "Copper Ore",
		ID = "copperOre",
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
			images = { "copperOre" },
			frames = 1,
			variants = 47
		},

		fillsLayers = { true, true, true },

		extraData = {
			mineProduce = "copperOre",
			exhaustTile = "stone"
		},

		extraLocalData = {
			holdingOre = 100,
		}
	}
)
