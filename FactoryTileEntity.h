#pragma once
#include "TileEntity.h"
#include "ItemManager.h"
#include "RecipieSelectWindow.h"

class FactoryTileEntity : public TileEntity
{
public:
	FactoryTileEntity(TileEntityStruct* tileEntityRef_, uint32_t rotation = 0);
	virtual ~FactoryTileEntity();

	//virtual void init();

	virtual void update() override;
	virtual void tileUpdate() override;

	virtual void render() override;
	virtual void renderAt(lost::Vector2D pos) override;

	void setRecipie(RecipieRefStruct* recipie);
	RecipieRefStruct* getRecipie() const;
	void checkRecipie();

	virtual Item extractItem(int count, bool output = true);
	virtual void insertItem(Item& item);

	inline float getCompletionPercentage() const { return m_CraftingTime / m_TimeToCraft; };
	inline float getFactorySpeed() const { return m_Speed; };

	virtual void mouseInteractFunction() override;
protected:
	RecipieRefStruct* m_SetRecipie = nullptr;
	TextureID recipieIconID = -1;
	std::string m_CraftingGroup = "base";

	RecipieSelectWindow* m_RecipieWindow = nullptr;

	float m_Speed = 0.0f;
	float m_CraftingTime = 0.0f;
	float m_TimeToCraft = 0.0f;

	bool m_CanCraft = false;
};

