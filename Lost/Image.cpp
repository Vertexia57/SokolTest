#include "Image.h"
#include "ImageManager.h"

Image::Image()
{
}

Image::Image(const char* fileName)
{
    loadImage(fileName);
}

Image::~Image()
{
    sg_destroy_image(image);
}

void Image::loadImage(const char* fileName)
{
    int channels;
    uint8_t* data = stbi_load(fileName, &width, &height, &channels, 4);
    image = { SG_INVALID_ID };

    sg_image_desc image_desc = { 0 };
    image_desc.width = width;
    image_desc.height = height;
    image_desc.data.subimage[0][0].ptr = data;
    image_desc.data.subimage[0][0].size = (size_t)(width * height * 4); // Specifies the number of bytes used for each image RGBA * Width * Height

    image = sg_make_image(&image_desc);
    stbi_image_free(data);

    if (sg_query_image_state(image) != SG_RESOURCESTATE_VALID)
        fprintf(stderr, "[Image::loadImage] Failed to load image: %s\n", fileName);
    else
        fprintf(stdout, "[Image::loadImage] Loaded Image: %s\n", fileName);
}

void Image::useImage(uint32_t slot)
{
    sgp_set_image(slot, image);
    sgp_set_sampler(slot, lost::getSampler(samplerID));
}
