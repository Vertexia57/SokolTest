data:addImages(
    {
        {
			file = "TileEntityData/Distributor.png",
			ID = "distributor",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "Distributor",
		ID = "distributor",
		description = "",
		
		imageData = {
			images = { "distributor" },
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
		collidable = true,
		requiresSupport = false,
		fillsLayers = { true, false, false },

        building = true,
        buildingData = {
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 0.0,  
            cost = 10.0,    
			rotationStates = 0,
            updateAction = "distributor",
            updateData = {
                speed = 160.0 -- How many units per second the item would move if it were a conveyerBelt
            }
        }
    }
)