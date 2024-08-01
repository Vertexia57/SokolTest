#pragma once
#include "Lost/FrameBuffer.h"

class Background
{
public:
	Background();
	virtual ~Background();

	virtual void update();
	virtual void render();

protected:
	//lost::FrameBuffer* m_Buffer;
};

