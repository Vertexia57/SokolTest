data:addImages(
    {
        {  
            file = "ItemData/Images/CopperOre.png",
            ID = "copperOreItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Copper Ore",
        ID = "copperOre",

        imageData = {
            randomVariant = true,
            imageID = "copperOreItem",
            variants = 2,
            frames = 1
        },

        stackCount = 12,
        placesTile = "stone"
    }
)