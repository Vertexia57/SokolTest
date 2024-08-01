#pragma once
#include "Collider.h"

class PhaseCollider : public Collider
{
public:
	PhaseCollider(lost::Bound2D bound);
	virtual ~PhaseCollider();

	virtual void update();
protected:
	virtual void m_CheckCollissions();
	virtual void m_CheckEntityCollisions(std::vector<TileEntity*>& tileEntities);
};

