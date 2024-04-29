data:addImages(
	{
		{
			file = "TileData/TestConnections.png",
			ID = "stone",
			pass = "albedo"
		}
	}
)

data:extend(
	{         
		dataType = "tile",
		
		name = "Stone",
		ID = "stone",
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
			images = { "stone" },
			frames = 1,
			variants = 47
		},

		fillsLayers = { true, true, true }
	}
)
