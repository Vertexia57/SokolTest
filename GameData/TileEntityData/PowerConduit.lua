data:addImages(
    {
        {
			file = "TileEntityData/Images/PowerConduit.png",
			ID = "powerConduit",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "Power Conduit",
		ID = "powerConduit",
		description = "",
		
		imageData = {
			images = { "powerConduit" },
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
		collidable = false,
		requiresSupport = false,
		fillsLayers = { true, false, false },

        building = true,
        buildingData = {
            cost = {
				{
					item = "ironIngot",
					count = 5
				}
			},
			buildingGroup = "electric",  
			rotationStates = 1,
            updateAction = "powerConduit",
            updateData = {
				powerRange = 5,
                range = 16
            }
        },
		powerData = {
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 0.0
		}
    }
)