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
		
		name = "Crusher",
		ID = "crusher",
		description = "",
		
		imageData = {
			images = { "placeholder" },
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
            cost = 50.0,    
			rotationStates = 1,
            updateAction = "factory",
            updateData = {
				craftingGroup = "crusher",
				speed = 1.0
            }
		},
		powerData = {
            powerIdleUsage = 10.0,  
            powerActiveUsage = 50.0,  
            powerProduce = 0.0
		}
    }
)