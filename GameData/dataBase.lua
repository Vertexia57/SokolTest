data = {}
data.tileEntityData = {}
data.tileData = {}
data.imageData = {}
data.itemData = {}
data.itemRecipies = {}

function printTable(table, indent)
	for k, v in pairs(table) do
		for i = 0, indent - 1, 1 do
			cPrint("	")
		end

		if (type(v) == "table") then
			cPrint(tostring(k) .. " :\n")
			printTable(v, indent + 1)
		else	
			cPrint(tostring(k) .. " : " .. tostring(v) .. "\n")
		end
	end
end

function data:addImages(images)
	for i = 1, #images, 1 do
		if (self.imageData[images[i]["ID"]] == nil) then
			self.imageData[images[i]["ID"]] = {}
			self.imageData[images[i]["ID"]]["imageFile"] = images[i]["file"]
			self.imageData[images[i]["ID"]]["pass"] = images[i]["pass"]
		end	
	end
end

function data:extend(object)
	if (object["dataType"] == "tileEntity") then
		self.tileEntityData[object["ID"]] = object
	elseif (object["dataType"] == "tile") then
		self.tileData[object["ID"]] = object
	elseif (object["dataType"] == "recipie") then
		self.itemRecipies[object["ID"]] = object
	elseif (object["dataType"] == "item") then
		self.itemData[object["ID"]] = object
	end
end