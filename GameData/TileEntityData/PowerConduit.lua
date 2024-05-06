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
			randomVariant = false	
		},
		
		width = 1.0,
		height = 1.0,
		
		placementOffsetX = 0.0,
		placementOffsetY = 0.0,
		
		stable = false,
		collidable = false,
		requiresSupport = false,
		fillsLayers = { true, false, false },

        building = true,
        buildingData = {
            powerUsage = 0.1,  
            cost = 5.0,    
            updateAction = "powerConduit",
            updateData = {
                range = 10
            }
        }
    }
)