data:addImages(
    {
        {
			file = "ExtraData/Images/LogisticIcon.png",
			ID = "logisticIcon",
			pass = "albedo"
        },
        {
			file = "ExtraData/Images/ElectricIcon.png",
			ID = "electricIcon",
			pass = "albedo"
        },
        {
			file = "ExtraData/Images/ProductionIcon.png",
			ID = "productionIcon",
			pass = "albedo"
        }
    }
)

data:createBuildingGroup(
    {
        ID = "logistics",
        iconName = "logisticIcon",
        groupName = "Logistics"
    }
)

data:createBuildingGroup(
    {
        ID = "electric",
        iconName = "electricIcon",
        groupName = "Electric"
    }
)

data:createBuildingGroup(
    {
        ID = "production",
        iconName = "productionIcon",
        groupName = "Production"
    }
)