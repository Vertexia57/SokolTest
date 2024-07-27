data:addImages(
    {
        {  
            file = "ItemData/IronIngot.png",
            ID = "ironIngotItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        craftingGroup = "furnace",
        ID = "ironSmeltRecipie",
        timeToCraft = 4.0,

        ingredients = {
            { item = "ferritePowder", count = 3 }
        },

        results = {
            { item = "ironIngot", count = 1 }   
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Iron Ingot",
        ID = "ironIngot",

        imageData = {
            randomVariant = false,
            imageID = "ironIngotItem",
            variants = 1,
            frames = 1
        },

        stackCount = 24,
    }
)