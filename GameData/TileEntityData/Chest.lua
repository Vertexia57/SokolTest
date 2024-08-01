data:addImages(
    {
        {
			file = "TileEntityData/Images/Chest.png",
			ID = "chest",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "Chest",
		ID = "chest",
		description = "",
		
		imageData = {
			images = { "chest" },
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
		requiresSupport = true,
		fillsLayers = { true, false, false },

        building = true,
		buildingData = {
            cost = {
				{
					item = "ironIngot",
					count = 5
				}
			},  
			buildingGroup = "logistics",  
			rotationStates = 1,
            updateAction = "chest",
            updateData = {
                invWidth = 5,
				invHeight = 2
            }
		},
		powerData = {
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 0.0
		}
    }
)