data:addImages(
    {
        {  
            file = "ItemData/Images/FerritePowder.png",
            ID = "ferritePowderItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Ferrite Powder Crushing",

        craftingGroup = "press",
        ID = "ferriteCrushRecipie",
        timeToCraft = 4.0,

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