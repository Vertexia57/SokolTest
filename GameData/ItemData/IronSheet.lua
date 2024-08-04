data:addImages(
    {
        {  
            file = "ItemData/Images/IronSheet.png",
            ID = "ironSheetItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Iron Sheet Pressing",

        craftingGroup = "press",
        ID = "ironSheetRecipie",
        timeToCraft = 1.0,

        ingredients = {
            { item = "ironIngot", count = 1 }
        },

        results = {
            { item = "ironSheet", count = 1 }   
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Iron Sheet",
        ID = "ironSheet",

        imageData = {
            randomVariant = false,
            imageID = "ironSheetItem",
            variants = 1,
            frames = 1
        },

        stackCount = 24,
    }
)