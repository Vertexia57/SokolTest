data:addImages(
    {
        {  
            file = "ItemData/Images/CopperPowder.png",
            ID = "copperPowderItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Copper Powder Crushing",
        
        craftingGroup = "crusher",
        ID = "copperCrushRecipie",
        timeToCraft = 4.0,

        ingredients = {
            { item = "copperOre", count = 1 }
        },

        results = {
            { item = "copperPowder", count = 2 }   
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Copper Powder",
        ID = "copperPowder",

        imageData = {
            randomVariant = false,
            imageID = "copperPowderItem",
            variants = 1,
            frames = 1
        },

        stackCount = 24,
    }
)