data:addImages(
    {
        {  
            file = "ItemData/Images/StoneSubstrate.png",
            ID = "stoneSubstrateItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Stone Substrate Pressing",

        craftingGroup = "press",
        ID = "stoneSubstrateRecipie",
        timeToCraft = 1.0,

        ingredients = {
            { item = "stoneOre", count = 1 }
        },

        results = {
            { item = "stoneSubstrate", count = 1 }   
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Stone Substrate",
        ID = "stoneSubstrate",

        imageData = {
            randomVariant = false,
            imageID = "stoneSubstrateItem",
            variants = 1,
            frames = 1
        },

        stackCount = 24,
    }
)