#include "CImageLoader.h"

#include <cstdio>

struct SBitmapInfo
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
    char *data;
};

#ifdef _WIN32
#pragma warning(disable: 4996)
#endif

CImage * const CImageLoader::loadImage(std::string const & fileName)
{
    FILE * file;
    char temp;
    long i;

    SBitmapInfo infoheader;

    if( (file = fopen(fileName.c_str(), "rb"))==NULL)
        return 0; // Open the file for reading

    fseek(file, 18, SEEK_CUR);  /* start reading width & height */
    fread(&infoheader.biWidth, sizeof(int), 1, file);

    fread(&infoheader.biHeight, sizeof(int), 1, file);

    fread(&infoheader.biPlanes, sizeof(short int), 1, file);
    if (infoheader.biPlanes != 1) {
        printf("Planes from %s is not 1: %u\n", fileName.c_str(), infoheader.biPlanes);
        return 0;
    }

    // read the bpp
    fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
    if (infoheader.biBitCount != 24) {
        printf("Bpp from %s is not 24: %d\n", fileName.c_str(), infoheader.biBitCount);
        return 0;
    }

    fseek(file, 24, SEEK_CUR);

    // read the data.
    infoheader.data = new char[infoheader.biWidth * infoheader.biHeight * 3];
    if (infoheader.data == NULL) {
        printf("Error allocating memory for color-corrected image data\n");
        return 0;
    }

    if ((i = fread(infoheader.data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", fileName.c_str());
        return 0;
    }

    for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = infoheader.data[i];
        infoheader.data[i] = infoheader.data[i+2];
        infoheader.data[i+2] = temp;
    }


    fclose(file); // Closes the file stream

    CImage * Image = new CImage(infoheader.data, infoheader.biWidth, infoheader.biHeight);

    return Image;
}

