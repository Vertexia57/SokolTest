data:addImages(
    {
        {  
            file = "ItemData/Images/IronIngot.png",
            ID = "ironIngotItem",
            pass = "albedo"
        },
        {
            file = "ItemData/Images/FerriteSmeltCraftIcon.png",
            ID = "ferritePowderSmeltIcon",
            pass = "albedo"
        },
        {
            file = "ItemData/Images/IronSmeltCraftIcon.png",
            ID = "ironOreSmeltIcon",
            pass = "albedo"
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Ferrite Powder Smelting",

        craftingGroup = "furnace",
        ID = "ironFerriteSmeltRecipie",
        icon = "ferritePowderSmeltIcon",
        timeToCraft = 2.0,

        ingredients = {
            { item = "ferritePowder", count = 3 }
        },

        results = {
            { item = "ironIngot", count = 2 }   
        }
    }
)

data:extend(
    {
        dataType = "recipie",
        name = "Iron Ore Smelting",

        craftingGroup = "furnace",
        ID = "ironOreSmeltRecipie",
        icon = "ironOreSmeltIcon",
        timeToCraft = 2.0,

        ingredients = {
            { item = "ironOre", count = 1 }
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