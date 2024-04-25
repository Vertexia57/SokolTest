#pragma once
#include "Lost.h"
#include "TileEntity.h"

enum
{
	TILE_LAYER_FOREGROUND,
	TILE_LAYER_BACKGROUND,
	TILE_LAYER_DETAIL
};

class Tile
{
public:
	Tile(lost::IntVector2D position, TextureID tileTexture);
	~Tile();

	void addTileEntityRef(TileEntity* ref);
	void removeTileEntityRef(TileEntity* ref);

	void render();

	bool filledLayers[3] = { false, false, false };

	lost::IntVector2D pos;
	std::vector<TileEntity*> tileEntitiesWithin;
private:
	TextureID m_TileTexture;
};