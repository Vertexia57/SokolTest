data:addImages(
	{
		{
			file = "TileEntityData/Images/Pebbles.png",
			ID = "pebbles",
			pass = "albedo"
		}
	}
)

data:extend(
	{
		dataType = "tileEntity",
		
		name = "Pebbles",
		ID = "pebbles",
		description = "",
		
		imageData = {
			images = { "pebbles" },
			frames = 1,
			variants = 4,
			randomVariant = true,
			animated = false
		},
		
		width = 1.0,
		height = 1.0,
		renderLayer = 0,
		
		placementOffsetX = 0.0,
		placementOffsetY = 0.0,

		stable = false,
		collidable = false,
		requiresSupport = true,
		fillsLayers = { false, true, false },

        building = false
	}
)
