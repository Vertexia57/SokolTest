#pragma once

#include "ImageManager.h"
#include "Transform2D.h"
#include "../SokolReference.h"
#include <string>

namespace lost
{

	struct NBoxData
	{
		int leftSize;
		int rightSize;
		int topSize;
		int bottomSize;

		IntVector2D imageSize;
		Bound2D bounds;
		float scale = 1.0f;

		TextureID texture = (TextureID)(-1);
	};

	void renderNBox(int leftSize, int rightSize, int topSize, int bottomSize, IntVector2D imageSize, Bound2D bounds, float scale = 1.0f, TextureID texture = -1);
	void renderNBox(NBoxData& nBoxData);

	//std::string formatFloat(float val);

}