#include "Item.h"
#include "Random.h"

Item::Item()
{
	empty = true;
}

Item::Item(ItemRefStruct* refStruct_)
{
	refStruct = refStruct_;
	textureID = refStruct->textureID;
	if (refStruct->randomVariant)
		variant = random(0, refStruct->variants - 1);
	itemName = refStruct->name;
	itemID = refStruct->itemID;
	maxStackSize = refStruct->maxStack;
}

Item::~Item()
{
}
