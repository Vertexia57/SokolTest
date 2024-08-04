data:addImages(
    {
        {
			file = "TileEntityData/Images/SolarGenerator.png",
			ID = "solarArray",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "Solar Array",
		ID = "solarArray",
		description = "",
		
		imageData = {
			images = { "solarArray" },
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
		collidable = false,
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
					item = "copperSheet",
					count = 10
				}
			},
			buildingGroup = "electric",  
			rotationStates = 1,
            updateAction = "none",
            updateData = {
            }
        },
		powerData = {
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 200.0
		}
    }
)