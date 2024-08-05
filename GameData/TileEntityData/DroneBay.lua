data:addImages(
    {
        {
			file = "Placeholder.png",
			ID = "placeholder",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "Drone Bay",
		ID = "droneBay",
		description = "",
		
		imageData = {
			images = { "placeholder" },
			frames = 1,
			variants = 1,
			randomVariant = false,
			animated = false
		},
		
		width = 3.0,
		height = 4.0,
		renderLayer = 1,
		
		placementOffsetX = -1.0,
		placementOffsetY = -3.0,
		
		stable = false,
		collidable = true,
		requiresSupport = true,
		fillsLayers = { true, false, false },

        building = true,
		buildingData = {
            cost = {
				{
					item = "ironIngot",
					count = 10
				}
			},
			buildingGroup = "logistics",  
			rotationStates = 1,
            updateAction = "droneBay",
            updateData = {
            }
		},
		powerData = {
            powerIdleUsage = 5.0,  
            powerActiveUsage = 40.0,  
            powerProduce = 0.0
		}
    }
)