data:addImages(
    {
        {  
            file = "ItemData/FerritePowder.png",
            ID = "ferritePowderItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        craftingGroup = "crusher",
        ID = "ferriteCrushRecipie",

        ingredients = {
            { item = "ironOre", count = 1 }
        },

        results = {
            { item = "ferritePowder", count = 2 }   
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Ferrite Powder",
        ID = "ferritePowder",

        imageData = {
            randomVariant = false,
            imageID = "ferritePowderItem",
            variants = 1,
            frames = 1
        },

        stackCount = 24,
    }
)