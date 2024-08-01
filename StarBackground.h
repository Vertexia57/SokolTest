#pragma once
#include "Background.h"
#include "Lost.h"
#include "Lost/FrameBuffer.h"

class StarBackground : public Background
{
private:

	struct Star
	{
		lost::Vector2D position = { 0.0f, 0.0f };
		float depth = 1.0f;
		float rotationSpeed = 0.0f;

		lost::Vector2D renderPosition = { 0.0f, 0.0f };
		float rotation = 0.0f;

		void update();
	};

public:
	StarBackground();
	virtual ~StarBackground();

	virtual void update();
	virtual void render();

protected:
	int m_StarCount = 100;
	Star* m_Stars;
	sgp_textured_rect* m_StarRects;

	ShaderID m_PixelateShaderID;
	lost::FrameBuffer* m_FrameBuffer;
};

