#include "ImageManager.h"

ImageManager::ImageManager()
{
	sg_sampler_desc sampler_desc = {};

	// Creates a sampler with linear sampling
	sampler_desc.min_filter = SG_FILTER_LINEAR;
	sampler_desc.mag_filter = SG_FILTER_LINEAR;
	sampler_desc.wrap_u = SG_WRAP_CLAMP_TO_BORDER;
	sampler_desc.wrap_v = SG_WRAP_CLAMP_TO_BORDER;
	samplers[0] = sg_make_sampler(&sampler_desc);

	// Creates a sampler with nearest neighbor sampling
	sampler_desc.min_filter = SG_FILTER_NEAREST;
	sampler_desc.mag_filter = SG_FILTER_NEAREST;
	sampler_desc.wrap_u = SG_WRAP_CLAMP_TO_BORDER;
	sampler_desc.wrap_v = SG_WRAP_CLAMP_TO_BORDER;
	samplers[1] = sg_make_sampler(&sampler_desc);
}

ImageManager::~ImageManager()
{
	// Destroys all heap allocated data
	for (Image* image : m_Images)
		delete image;
	for (sg_sampler sampler : samplers)
		sg_destroy_sampler(sampler);
}

TextureID ImageManager::addImageToQueue(const ImageLoadStruct& loadStruct)
{
	m_LoadQueue.push_back(loadStruct);
	m_ImageMap[loadStruct.imageName] = m_ImagesAllocated;

	m_ImagesAllocated++;

	return m_ImagesAllocated - 1;
}

void ImageManager::loadImageQueue(float& percentageReturn)
{
	TextureID startingSize = static_cast<TextureID>(m_Images.size());

	size_t imagesToLoad = m_LoadQueue.size();

	for (int i = 0; i < imagesToLoad; i++)
	{
		Image* image = new Image(m_LoadQueue[i].fileName.c_str());
		image->id = startingSize + i;

		m_Images.push_back(image);

		percentageReturn = (float)i / imagesToLoad;
	}
	m_LoadQueue.clear();
}

Image* ImageManager::getImage(TextureID id)
{
	return m_Images[id];
}

Image* ImageManager::getImage(std::string id)
{
	return m_Images[m_ImageMap[id]];
}

bool ImageManager::imageAdded(std::string id)
{
	return m_ImageMap.count(id);
}

TextureID ImageManager::getImageID(std::string id)
{
	return m_ImageMap[id];
}

sg_sampler& ImageManager::getSampler(int id)
{
	return samplers[id];
}

void ImageManager::reload()
{
	for (Image* image : m_Images)
		image->reload();
}

namespace lost
{

	::ImageManager* imageManager = nullptr;

	void createImageManager()
	{
		imageManager = new ::ImageManager();
	}

	void destroyImageManager()
	{
		delete imageManager;
	}

	void loadImageQueue(float& percentageReturn)
	{
		percentageReturn = 0.0f;
		imageManager->loadImageQueue(percentageReturn);
	}

	void loadImageQueue()
	{
		float emptyFloat = 0;
		imageManager->loadImageQueue(emptyFloat);
	}

	TextureID loadImage(const char* fileName, const char* imageName)
	{
		ImageLoadStruct loadStruct = { fileName, imageName };
		return imageManager->addImageToQueue(loadStruct);
	}

	TextureID loadImage(std::string fileName, std::string imageName)
	{
		ImageLoadStruct loadStruct = { fileName.c_str(), imageName.c_str() };
		return imageManager->addImageToQueue(loadStruct);
	}

	Image* getImage(TextureID id)
	{
		return imageManager->getImage(id);
	}

	Image* getImage(std::string id)
	{
		return imageManager->getImage(id);
	}

	bool imageAdded(std::string id)
	{
		return imageManager->imageAdded(id);
	}

	TextureID getImageID(std::string id)
	{
		return imageManager->getImageID(id);
	}

	void useImage(TextureID id, uint32_t slot)
	{
		imageManager->getImage(id)->useImage(slot);
	}

	void useImage(std::string id, uint32_t slot)
	{
		imageManager->getImage(id)->useImage(slot);
	}

	void clearImage(uint32_t slot)
	{
		sgp_reset_image(slot);
	}

	sg_sampler& getSampler(int samplerID)
	{
		return imageManager->getSampler(samplerID);
	}

	void reloadImages()
	{
		imageManager->reload();
	}

}