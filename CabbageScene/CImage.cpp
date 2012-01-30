#include "CImage.h"

CImage::CImage(char * const imageData, int const width, int const height)
    : ImageData(imageData), Width(width), Height(height)
{}

CImage::~CImage()
{
    delete ImageData;
}

int const CImage::getWidth()
{
    return Width;
}

int const CImage::getHeight()
{
    return Height;
}

char const * const CImage::getImageData()
{
    return ImageData;
}
