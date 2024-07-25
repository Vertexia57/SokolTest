#pragma once
#include "TileEntity.h"

class PowerConduit : public TileEntity
{
public:
	PowerConduit(TileEntityStruct* tileEntityRef_, uint32_t rotation = 0);
	virtual ~PowerConduit();

	virtual void init();

	// Uses base tile entities updating
	//virtual void update() override;
	virtual void tileUpdate() override;

	void setNode(int nodeID);
	void setTileCircuitReferences();
	inline std::vector<uint32_t>& getNodeConnections() { return m_ConnectedNodes; };
	inline uint32_t getNodeID() const { return m_NodeGraphID; };

	// Uses base tile entities rendering
	//virtual void render() override;
	//virtual void renderAt(lost::Vector2D pos) override;
	virtual void renderForeground();
	virtual void renderForegroundAt(lost::Vector2D pos);

	//virtual void mouseInteractFunction() override;
protected:
	int powerRange = 0;
	int connectionRange = 0;

	std::vector<PowerConduit*> m_ConnectedConduits;
	std::vector<uint32_t> m_ConnectedNodes;
	uint32_t m_NodeGraphID = 0;
};