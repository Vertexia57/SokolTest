data:addImages(
    {
        {
			file = "TileEntityData/Miner.png",
			ID = "miner",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "Miner",
		ID = "miner",
		description = "",
		
		imageData = {
			images = { "miner" },
			frames = 1,
			variants = 1,
			randomVariant = false,
			animated = false
		},
		
		width = 3.0,
		height = 3.0,
		renderLayer = 1,
		
		placementOffsetX = -1.0,
		placementOffsetY = -2.0,
		
		stable = false,
		collidable = false,
		requiresSupport = true,
		fillsLayers = { true, false, false },

        building = true,
        buildingData = {
            powerUsage = 10.0,  
            powerProduce = 0.0,  
            cost = 20.0,    
			rotationStates = 1,
            updateAction = "miner",
            updateData = {
                speed = 1.0
            }
        }
    }
)