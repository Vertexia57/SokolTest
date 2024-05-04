#include "RenderExtras.h"

void lost::renderNBox(int leftSize, int rightSize, int topSize, int bottomSize, IntVector2D imageSize, Bound2D bounds, float scale, TextureID texture)
{
	NBoxData data = { leftSize, rightSize, topSize, bottomSize, imageSize, bounds, scale, texture };
	renderNBox(data);
}

void lost::renderNBox(NBoxData& nBoxData)
{
	if (nBoxData.texture != (TextureID)(-1))
		useImage(nBoxData.texture);

	// Top Left Corner
	sgp_draw_textured_rect(0,
		{
			nBoxData.bounds.x,
			nBoxData.bounds.y,
			(float)nBoxData.leftSize * nBoxData.scale,
			(float)nBoxData.topSize * nBoxData.scale
		},
		{ 0.0f, 0.0f, (float)nBoxData.leftSize, (float)nBoxData.topSize }
	);
	// Top Border
	sgp_draw_textured_rect(0,
		{
			nBoxData.bounds.x + (float)nBoxData.leftSize * nBoxData.scale,
			nBoxData.bounds.y,
			nBoxData.bounds.w - (float)(nBoxData.leftSize + nBoxData.rightSize) * nBoxData.scale,
			(float)nBoxData.topSize * nBoxData.scale
		},
		{ (float)nBoxData.leftSize, 0.0f, nBoxData.imageSize.x - (float)(nBoxData.leftSize + nBoxData.rightSize), (float)nBoxData.topSize }
	);
	// Top Right Corner
	sgp_draw_textured_rect(0,
		{
			nBoxData.bounds.x + nBoxData.bounds.w - (float)nBoxData.rightSize * nBoxData.scale,
			nBoxData.bounds.y,
			(float)nBoxData.rightSize * nBoxData.scale,
			(float)nBoxData.topSize * nBoxData.scale
		},
		{ (float)(nBoxData.imageSize.x - nBoxData.rightSize), 0.0f, (float)nBoxData.rightSize, (float)nBoxData.topSize }
	);

	// Right Border
	sgp_draw_textured_rect(0,
		{
			nBoxData.bounds.x + nBoxData.bounds.w - (float)nBoxData.rightSize * nBoxData.scale,
			nBoxData.bounds.y + (float)nBoxData.topSize * nBoxData.scale,
			(float)nBoxData.rightSize * nBoxData.scale,
			nBoxData.bounds.h - (float)(nBoxData.topSize + nBoxData.bottomSize) * nBoxData.scale
		},
		{ (float)(nBoxData.imageSize.x - nBoxData.rightSize), (float)(nBoxData.topSize), (float)nBoxData.rightSize, (float)(nBoxData.imageSize.y - nBoxData.topSize - nBoxData.bottomSize) }
	);

	// Bottom Right Corner
	sgp_draw_textured_rect(0,
		{
			nBoxData.bounds.x + nBoxData.bounds.w - (float)nBoxData.rightSize * nBoxData.scale,
			nBoxData.bounds.y + nBoxData.bounds.h - (float)nBoxData.bottomSize * nBoxData.scale,
			(float)nBoxData.rightSize * nBoxData.scale,
			(float)nBoxData.bottomSize * nBoxData.scale
		},
		{ (float)(nBoxData.imageSize.x - nBoxData.rightSize), (float)(nBoxData.imageSize.y - nBoxData.bottomSize), (float)nBoxData.rightSize, (float)nBoxData.bottomSize }
	);

	// Bottom Border
	sgp_draw_textured_rect(0,
		{
			nBoxData.bounds.x + (float)nBoxData.leftSize * nBoxData.scale,
			nBoxData.bounds.y + nBoxData.bounds.h - (float)nBoxData.bottomSize * nBoxData.scale,
			nBoxData.bounds.w - (float)(nBoxData.leftSize + nBoxData.rightSize) * nBoxData.scale,
			(float)nBoxData.bottomSize * nBoxData.scale
		},
		{ (float)nBoxData.leftSize, (float)(nBoxData.imageSize.y - nBoxData.bottomSize), nBoxData.imageSize.x - (float)(nBoxData.leftSize + nBoxData.rightSize), (float)nBoxData.bottomSize }
	);

	// Bottom Left Corner
	sgp_draw_textured_rect(0,
		{
			nBoxData.bounds.x,
			nBoxData.bounds.y + nBoxData.bounds.h - (float)nBoxData.bottomSize * nBoxData.scale,
			(float)nBoxData.leftSize * nBoxData.scale,
			(float)nBoxData.bottomSize * nBoxData.scale
		},
		{ 0.0f, (float)(nBoxData.imageSize.y - nBoxData.bottomSize), (float)nBoxData.leftSize, (float)nBoxData.bottomSize }
	);

	// Left Border
	sgp_draw_textured_rect(0,
		{
			nBoxData.bounds.x,
			nBoxData.bounds.y + (float)nBoxData.topSize * nBoxData.scale,
			(float)nBoxData.leftSize * nBoxData.scale,
			nBoxData.bounds.h - (float)(nBoxData.topSize + nBoxData.bottomSize) * nBoxData.scale
		},
		{ 0.0f, (float)(nBoxData.topSize), (float)nBoxData.leftSize, (float)(nBoxData.imageSize.y - nBoxData.topSize - nBoxData.bottomSize) }
	);

	// Center
	sgp_draw_textured_rect(0,
		{
			nBoxData.bounds.x + (float)nBoxData.leftSize * nBoxData.scale,
			nBoxData.bounds.y + (float)nBoxData.topSize * nBoxData.scale,
			nBoxData.bounds.w - (float)(nBoxData.leftSize + nBoxData.rightSize) * nBoxData.scale,
			nBoxData.bounds.h - (float)(nBoxData.topSize + nBoxData.bottomSize) * nBoxData.scale
		},
		{ (float)(nBoxData.leftSize), (float)(nBoxData.topSize), nBoxData.imageSize.x - (float)(nBoxData.leftSize + nBoxData.rightSize), (float)(nBoxData.imageSize.y - nBoxData.topSize - nBoxData.bottomSize) }
	);

	if (nBoxData.texture != (TextureID)(-1))
		lost::clearImage();
}
