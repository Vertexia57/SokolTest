data:addImages(
    {
        {
			file = "TileEntityData/ConveyerBelt.png",
			ID = "coneyerBelt",
			pass = "albedo"
        }
    }
)

data:extend(
    {
		dataType = "tileEntity",
		
		name = "ConveyerBelt",
		ID = "coneyerBelt",
		description = "",
		
		imageData = {
			images = { "coneyerBelt" },
			frames = 1,
			variants = 32,
			randomVariant = false,
			animated = false,
			animationData = {
				frameRate = 0.03
			}
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
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 0.0,  
            cost = 5.0,    
			rotationStates = 4,
			rotationVariants = { 24, 0, 16, 8 },
            updateAction = "conveyerBelt",
            updateData = {
                xSpeed = 160.0,
                ySpeed = 160.0
            }
        }
    }
)