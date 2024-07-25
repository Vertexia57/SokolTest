data:addImages(
    {
        {
			file = "TileEntityData/PowerConduit.png",
			ID = "powerConduit",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "PowerConduit",
		ID = "powerConduit",
		description = "",
		
		imageData = {
			images = { "powerConduit" },
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
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 0.0,  
            cost = 5.0,    
			rotationStates = 1,
            updateAction = "powerConduit",
            updateData = {
				powerRange = 5,
                range = 16
            }
        }
    }
)