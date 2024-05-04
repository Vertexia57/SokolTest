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
			randomVariant = true	
		},
		
		width = 1.0,
		height = 1.0,
		
		placementOffsetX = 0.0,
		placementOffsetY = 0.0,
		
		stable = false,
		collidable = true,
		requiresSupport = true,
		fillsLayers = { true, false, false },

        containerData = {
            slots = {
                { "storage", 5 } -- First is the type of the slot, second is the amount of slot of that type
            }
        },

        interactAction = "chestUI",
    }
)