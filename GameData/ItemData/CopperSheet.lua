data:addImages(
    {
        {  
            file = "ItemData/Images/CopperSheet.png",
            ID = "copperSheetItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Copper Sheet Pressing",

        craftingGroup = "press",
        ID = "copperSheetRecipie",
        timeToCraft = 1.0,

        ingredients = {
            { item = "copperIngot", count = 1 }
        },

        results = {
            { item = "copperSheet", count = 1 }   
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Copper Sheet",
        ID = "copperSheet",

        imageData = {
            randomVariant = false,
            imageID = "copperSheetItem",
            variants = 1,
            frames = 1
        },

        stackCount = 24,
    }
)