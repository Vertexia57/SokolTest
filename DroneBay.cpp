#include "DroneBay.h"
#include "World.h"

DroneBay::DroneBay(TileEntityStruct* tileEntityRef_, uint32_t rotation)
	: TileEntity(tileEntityRef_, rotation)
{
	updates = true;
	interactable = true;
	m_Storage = new Container(1);
	m_Name = "group";
	g_World->addDroneBay(this);
}

DroneBay::~DroneBay()
{
	g_World->removeDroneBay(this);
	for (DroneEntity* e : m_IncomingDrones)
		e->die();
}

void DroneBay::init()
{
	TileEntity::init();
	//g_World->addEntity(new DroneEntity({ m_Hitbox.x * 32.0f, m_Hitbox.y * 32.0f }, this));
}

void DroneBay::update()
{
	if (m_Storage->getItem(0)->StackSize > 0 && m_Sending)
	{
		m_IdleTime += lost::deltaTime;
		if (m_IdleTime >= 3000.0f || m_Storage->getItem(0)->StackSize == m_Storage->getItem(0)->maxStackSize)
		{
			m_IdleTime = 0.0f;
			DroneBay* other = g_World->getTransferBay(this);
			if (other)
			{
				DroneEntity* entity = new DroneEntity({ m_Hitbox.x * 32.0f + m_Hitbox.w * 32.0f / 2.0f, m_Hitbox.y * 32.0f }, other);
				entity->insertItem(*m_Storage->getItem(0));
				m_Storage->removeItem(0);
				g_World->addEntity(entity);
			}
		}
	}
}

void DroneBay::mouseInteractFunction()
{
	if (lost::mouseTapped(0))
		m_Sending = !m_Sending;
}

void DroneBay::insertItem(Item& item)
{
	if (hasInventory() && !item.empty)
		m_Storage->addItem(item);
	m_IdleTime = 0.0f;
}

bool DroneBay::canRecieve(Item& item) const
{
	if (m_Storage->getItem(0)->empty)
		return true;
	if (m_Storage->findAccessableSlot(item) == -1)
		return false;
	return m_Storage->getItem(0)->StackSize + item.StackSize <= m_Storage->getItem(0)->maxStackSize;
}

void DroneBay::addIncomingDrone(DroneEntity* entity)
{
	m_IncomingDrones.push_back(entity);
}

void DroneBay::removeIncomingDrone(DroneEntity* entity)
{
	for (int i = 0; i < m_IncomingDrones.size(); i++)
	{
		if (m_IncomingDrones[i] == entity)
		{
			m_IncomingDrones.erase(m_IncomingDrones.begin() + i);
			break;
		}
	}
}
