data:addImages(
    {
        {
			file = "TileEntityData/ConveyerBelt.png",
			ID = "coneyerBelt",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "ConveyerBelt",
		ID = "coneyerBelt",
		description = "",
		
		imageData = {
			images = { "coneyerBelt" },
			frames = 1,
			variants = 1,
			randomVariant = false	
		},
		
		width = 1.0,
		height = 0.625,
		
		placementOffsetX = 0.0,
		placementOffsetY = 0.0,
		
		stable = false,
		collidable = true,
		requiresSupport = false,
		fillsLayers = { true, false, false },

        building = true,
        buildingData = {
            powerUsage = 10.0,  
            cost = 5.0,    
			rotationStates = 2,
            updateAction = "conveyerBelt",
            updateData = {
                xSpeed = 1.0
            }
        }
    }
)