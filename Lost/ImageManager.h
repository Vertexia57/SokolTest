#pragma once
#include "Image.h"

#include <vector>
#include <map>
#include <string>

struct ImageLoadStruct
{
	std::string fileName;
	std::string imageName;
};

class ImageManager
{
public:
	ImageManager();
	~ImageManager();

	TextureID addImageToQueue(const ImageLoadStruct& loadStruct);

	// Loads all of the images withing m_LoadQueue, percentageReturn is useful with multithreading, and can be used for loading bars
	void loadImageQueue(float& percentageReturn);

	// Returns the image with the id specified
	Image* getImage(TextureID id);
	// Returns the image with the name specified
	Image* getImage(std::string id);

	// Returns if an image with the name has been added or not
	bool imageAdded(std::string id);

	// Returns the imageID with the name specified
	TextureID getImageID(std::string id);

	// Returns the sampler with samplerID
	sg_sampler& getSampler(int id);

	// Reloads all images within the imageManager
	void reload();

private:
	uint32_t m_ImagesAllocated = 0; // This is used to assign TextureID

	std::vector<ImageLoadStruct> m_LoadQueue;
	std::vector<Image*> m_Images;
	std::map<std::string, TextureID> m_ImageMap;

	sg_sampler samplers[2];
};

namespace lost
{

	extern ::ImageManager* imageManager;
	
	// These are ran by the system
	extern void createImageManager();
	extern void destroyImageManager();

	// Loads all of the images within the image managers image queue, returns the percentage of completetion
	extern void loadImageQueue(float& percentageReturn);

	// Loads all of the images within the image managers image queue, can return the percentage of completetion
	extern void loadImageQueue();

	// Loads an image into the image manager load queue 
	extern TextureID loadImage(const char* fileName, const char* imageName);
	// Loads an image into the image manager load queue 
	extern TextureID loadImage(std::string fileName, std::string imageName);

	// Returns the image with the id specified
	extern Image* getImage(TextureID id);
	// Returns the image with the name specified
	extern Image* getImage(std::string id);

	// Returns if an image with the name has been added or not
	extern bool imageAdded(std::string id);

	// Returns the image with the name specified
	extern TextureID getImageID(std::string id);

	// Sets the image with id into it's set image slot, "slot" defaults to LOST_TEXTURE_ALBEDO
	extern void useImage(TextureID id, uint32_t slot = LOST_TEXTURE_ALBEDO);
	// Sets the image with id into it's set image slot, "slot" defaults to LOST_TEXTURE_ALBEDO
	extern void useImage(std::string id, uint32_t slot = LOST_TEXTURE_ALBEDO);

	// Clears the image from the sampler with the slot specified
	extern void clearImage(uint32_t slot = LOST_TEXTURE_ALBEDO);

	// Returns the sampler with samplerID
	extern sg_sampler& getSampler(int samplerID);

	// Reloads all loaded images
	extern void reloadImages();

}