data:addImages(
    {
        {  
            file = "ItemData/Images/StoneOre.png",
            ID = "stoneOreItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Stone Ore",
        ID = "stoneOre",

        imageData = {
            randomVariant = true,
            imageID = "stoneOreItem",
            variants = 2,
            frames = 1
        },

        stackCount = 12,
        placesTile = "stone"
    }
)