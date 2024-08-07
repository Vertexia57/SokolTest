data:addImages(
	{
		{
			file = "TileEntityData/Images/PetrifiedTree.png",
			ID = "petrifiedTree",
			pass = "albedo"
		}
	}
)

data:extend(
	{
		dataType = "tileEntity",
		
		name = "Petrified Tree",
		ID = "petrifiedTree",
		description = "",
		
		imageData = {
			images = { "petrifiedTree" },
			frames = 1,
			variants = 2,
			randomVariant = true,
			animated = false
		},
		
		width = 3.0,
		height = 8.0,
		renderLayer = 0,
		
		placementOffsetX = -1.0,
		placementOffsetY = -7.0,

		stable = false,
		collidable = false,
		requiresSupport = true,
		fillsLayers = { false, true, false },

        building = false
	}
)
