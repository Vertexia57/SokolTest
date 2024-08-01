data:addImages(
    {
        {
			file = "TileEntityData/Images/Junction.png",
			ID = "junction",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "Junction",
		ID = "junction",
		description = "",
		
		imageData = {
			images = { "junction" },
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
			buildingGroup = "logistics",  
			rotationStates = 0,
            updateAction = "junction",
            updateData = {
                speed = 160.0 -- How many units per second the item would move if it were a conveyerBelt
            }
        },
		powerData = {
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 0.0
		}
    }
)