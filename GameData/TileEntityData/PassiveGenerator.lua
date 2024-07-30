data:addImages(
	{
		{
			file = "TileEntityData/Placeholder.png",
			ID = "placeholder",
			pass = "albedo"
		}
	}
)

data:extend(
	{
		dataType = "tileEntity",
		
		name = "Passive Generator",
		ID = "passiveGenerator",
		description = "",
		
		imageData = {
			images = { "placeholder" },
			frames = 1,
			variants = 1,
			randomVariant = false,
			animated = false
		},
		
		width = 3.0,
		height = 4.0,
		renderLayer = 1,
		
		placementOffsetX = -1.0,
		placementOffsetY = -3.0,

		stable = false,
		collidable = true,
		requiresSupport = true,
		fillsLayers = { true, false, false },

        building = false,
        powerData = {
            powerIdleUsage = 0.0,  
            powerActiveUsage = 0.0,  
            powerProduce = 400.0,  
		}
	}
)
