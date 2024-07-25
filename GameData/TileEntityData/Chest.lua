data:addImages(
    {
        {
			file = "TileEntityData/Chest.png",
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
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 0.0,  
            cost = 10.0,    
			rotationStates = 1,
            updateAction = "chest",
            updateData = {
                invWidth = 5,
				invHeight = 2
            }
		},

        containerData = {
            slots = {
                { "storage", 10 } -- First is the type of the slot, second is the amount of slot of that type
            }
        }
    }
)