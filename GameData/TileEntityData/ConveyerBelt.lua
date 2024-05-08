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
		
		width = 3.0,
		height = 3.0,
		
		placementOffsetX = -1.0,
		placementOffsetY = -2.0,
		
		stable = false,
		collidable = false,
		requiresSupport = true,
		fillsLayers = { true, false, false },

        building = true,
        buildingData = {
            powerUsage = 10.0,  
            cost = 5.0,    
            updateAction = "converyerBelt",
            updateData = {
                xSpeed = 1.0
            }
        }
    }
)