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
		
		name = "Extruder",
		ID = "extruder",
		description = "",
		
		imageData = {
			images = { "placeholder" },
			frames = 1,
			variants = 1,
			randomVariant = false,
			animated = false
		},
		
		width = 3.0,
		height = 2.0,
		renderLayer = 1,
		
		placementOffsetX = -1.0,
		placementOffsetY = -1.0,
		
		stable = false,
		collidable = true,
		requiresSupport = true,
		fillsLayers = { true, false, false },

        building = true,
		buildingData = {
            cost = 50.0,    
			buildingGroup = "production",  
			rotationStates = 1,
            updateAction = "factory",
            updateData = {
				craftingGroup = "extruder",
				speed = 1.0
            }
		},
		powerData = {
            powerIdleUsage = 5.0,  
            powerActiveUsage = 40.0,  
            powerProduce = 0.0
		}
    }
)