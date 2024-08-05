#pragma once
#include "TileEntity.h"
#include "DroneEntity.h"

class DroneBay : public TileEntity
{
public:
	DroneBay(TileEntityStruct* tileEntityRef_, uint32_t rotation = 0);
	virtual ~DroneBay();

	virtual void init();

	virtual void update() override;

	// Uses base tile entities rendering
	//virtual void render() override;
	//virtual void renderAt(lost::Vector2D pos) override;

	virtual void mouseInteractFunction() override;

	inline const std::string& getName() const { return m_Name; };
	inline bool isSending() const { return m_Sending; };

	virtual void insertItem(Item& item);
	bool canRecieve(Item& item) const;

	void addIncomingDrone(DroneEntity* entity);
	void removeIncomingDrone(DroneEntity* entity);
protected:
	bool m_Sending = false;
	DroneBay* m_Target = nullptr;

	std::vector<DroneEntity*> m_IncomingDrones;
	float m_IdleTime = 0.0f;

	std::string m_Name;
};

