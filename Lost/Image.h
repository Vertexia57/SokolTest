#pragma once
#include "../SokolReference.h"
#include "../stb_image.h"

typedef uint32_t TextureID;

enum
{
	LOST_TEXTURE_ALBEDO,
	LOST_TEXTURE_2
};

enum
{
	LOST_SAMPLER_LINEAR,
	LOST_SAMPLER_NEAREST
};

class Image
{
public:
	Image();
	Image(const char* fileName);
	~Image();
	
	// Loads the image with the set filename
	void loadImage(const char* fileName);

	// Sets the sampler with this images samplerID within the shader to use this image
	void useImage(uint32_t slot);

	sg_image image = { SG_INVALID_ID };

	int width = 0;
	int height = 0;
	TextureID id = 0;

	uint32_t samplerID = LOST_SAMPLER_NEAREST;
private:

};

