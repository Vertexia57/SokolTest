data:addImages(
    {
        {  
            file = "ItemData/IronOre.png",
            ID = "ironOreItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Iron Ore",
        ID = "ironOre",

        imageData = {
            randomVariant = true,
            imageID = "ironOreItem",
            variants = 2,
            frames = 1
        },

        stackCount = 12,
        placesTile = "stone"
    }
)