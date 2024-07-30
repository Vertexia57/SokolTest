data:addImages(
    {
        {
			file = "TileEntityData/Images/Miner.png",
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
            cost = 20.0,    
			buildingGroup = "production",  
			rotationStates = 1,
            updateAction = "miner",
            updateData = {
                timePerMine = 3.0
            }
        },
		powerData = {
            powerIdleUsage = 5.0,  
            powerActiveUsage = 100.0,  
            powerProduce = 0.0
		}
    }
)