data:addImages(
    {
        {  
            file = "ItemData/Images/CopperWire.png",
            ID = "copperWireItem",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Copper Wire Extruding",

        craftingGroup = "extruder",
        ID = "copperWireRecipie",
        timeToCraft = 1.0,

        ingredients = {
            { item = "copperIngot", count = 1 }
        },

        results = {
            { item = "copperWire", count = 3 }   
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Copper Wire",
        ID = "copperWire",

        imageData = {
            randomVariant = true,
            imageID = "copperWireItem",
            variants = 1,
            frames = 1
        },

        stackCount = 24,
    }
)