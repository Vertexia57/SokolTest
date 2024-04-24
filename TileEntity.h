#pragma once
#include "Lost.h"

class TileEntity
{
public:
	TileEntity(lost::Vector2D position_, TextureID tileTexture, lost::Bound2D hitbox);
	~TileEntity();

	// Sets the hitbox for the tile, with a max of 64 x 64
	void setHitbox(lost::Bound2D hitbox);
	lost::Bound2D getHitbox();

	void render();
	void renderHitbox();

	bool fillsLayers[3] = {};

	lost::Vector2D position;
private:
	lost::Bound2D m_Hitbox;
	TextureID m_TileTexture;
};

