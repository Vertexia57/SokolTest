data:addImages(
    {
        {  
            file = "ItemData/Images/Components.png",
            ID = "componentItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Component Assembling",

        craftingGroup = "assembler",
        ID = "componentRecipie",
        timeToCraft = 2.0,

        ingredients = {
            { item = "copperWire", count = 2 },
            { item = "stoneSubstrate", count = 1}
        },

        results = {
            { item = "component", count = 1 }   
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Component",
        ID = "component",

        imageData = {
            randomVariant = true,
            imageID = "componentItem",
            variants = 1,
            frames = 1
        },

        stackCount = 24,
    }
)