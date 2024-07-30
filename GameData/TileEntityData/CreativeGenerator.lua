data:addImages(
    {
        {
			file = "TileEntityData/Images/CreativeGenerator.png",
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
            cost = 0.0,    
			buildingGroup = "electric",  
			rotationStates = 1,
            updateAction = "powerConduit",
            updateData = {
				powerRange = 10,
                range = 21
            }
        },
		powerData = {
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 1000000000.0
		}
    }
)