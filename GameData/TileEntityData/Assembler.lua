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
		
		name = "Assembler",
		ID = "assembler",
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
		collidable = true,
		requiresSupport = true,
		fillsLayers = { true, false, false },

        building = true,
		buildingData = {
            cost = {
				{
					item = "ironSheet",
					count = 20
				},
				{
					item = "copperSheet",
					count = 15
				}
			}, 
			buildingGroup = "production",  
			rotationStates = 1,
            updateAction = "factory",
            updateData = {
				craftingGroup = "assembler",
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