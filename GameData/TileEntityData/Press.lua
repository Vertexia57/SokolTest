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
		
		name = "Mechanical Press",
		ID = "mechanicalPress",
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
					item = "ironIngot",
					count = 15
				},
				{
					item = "copperIngot",
					count = 5
				}
			},
			buildingGroup = "production",  
			rotationStates = 1,
            updateAction = "factory",
            updateData = {
				craftingGroup = "press",
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