data:addImages(
    {
        {
			file = "TileEntityData/CreativeGenerator.png",
			ID = "creativeGenerator",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "CreativeGenerator",
		ID = "creativeGenerator",
		description = "",
		
		imageData = {
			images = { "creativeGenerator" },
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
		
		stable = false,
		collidable = false,
		requiresSupport = false,
		fillsLayers = { true, false, false },

        building = true,
        buildingData = {
            powerUsage = 0.0,  
            powerProduce = 100000000.0,  
            cost = 0.0,    
			rotationStates = 1,
            updateAction = "powerConduit",
            updateData = {
				powerRange = 10,
                range = 21
            }
        }
    }
)