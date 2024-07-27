data:addImages(
	{
		{
			file = "TileEntityData/Platform.png",
			ID = "platform",
			pass = "albedo"
		}
	}
)

data:extend(
	{
		dataType = "tileEntity",
		
		name = "Platform",
		ID = "platform",
		description = "",
		
		imageData = {
			images = { "platform" },
			frames = 1,
			variants = 1,
			randomVariant = false,
			animated = false
		},
		
		width = 1.0,
		height = 1.0,
		renderLayer = 0,
		
		placementOffsetX = 0.0,
		placementOffsetY = 0.0,

		stable = true,
		collidable = true,
		requiresSupport = false,
		fillsLayers = { true, false, false },

        building = true,
		buildingData = {
            cost = 10.0,    
			rotationStates = 1,
            updateAction = "none",
            updateData = {
            }
		}
	}
)
