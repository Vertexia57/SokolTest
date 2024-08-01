data:addImages(
	{
		{
			file = "TileEntityData/Images/Platform.png",
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
		renderLayer = 1,
		
		placementOffsetX = 0.0,
		placementOffsetY = 0.0,

		stable = true,
		collidable = true,
		requiresSupport = false,
		fillsLayers = { true, false, false },

        building = true,
		buildingData = {
            cost = {
				{
					item = "ironIngot",
					count = 5
				}
			},  
			buildingGroup = "logistics",  
			rotationStates = 1,
            updateAction = "none",
            updateData = {
            }
		}
	}
)
