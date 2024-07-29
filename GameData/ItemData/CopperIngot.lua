data:addImages(
    {
        {  
            file = "ItemData/Images/CopperIngot.png",
            ID = "copperIngotItem",
            pass = "albedo"
        },
        {
            file = "ItemData/Images/CopperPowderSmeltCraftIcon.png",
            ID = "copperPowderSmeltIcon",
            pass = "albedo"
        },
        {
            file = "ItemData/Images/CopperOreSmeltCraftIcon.png",
            ID = "copperOreSmeltIcon",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Copper Powder Smelting",

        craftingGroup = "furnace",
        ID = "copperPowderSmeltRecipie",
        icon = "copperPowderSmeltIcon",
        timeToCraft = 2.0,

        ingredients = {
            { item = "copperPowder", count = 3 }
        },

        results = {
            { item = "copperIngot", count = 2 }   
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Copper Ore Smelting",

        craftingGroup = "furnace",
        ID = "copperOreSmeltRecipie",
        icon = "copperOreSmeltIcon",
        timeToCraft = 2.0,

        ingredients = {
            { item = "copperOre", count = 2 }
        },

        results = {
            { item = "copperIngot", count = 1 }   
        }
    }
)

data:extend(
    {
        dataType = "item",

        name = "Copper Ingot",
        ID = "copperIngot",

        imageData = {
            randomVariant = false,
            imageID = "copperIngotItem",
            variants = 1,
            frames = 1
        },

        stackCount = 24,
    }
)