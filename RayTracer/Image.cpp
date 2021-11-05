#include "Image.h"
#include "Renderer.h"
#include <fstream>
#include <iostream>

Image::~Image()
{
    //delete[] colorBuffer.data;
}

Image::Image(const std::string& filename, uint8_t alpha)
{
    Load(filename, alpha);
}

bool Image::Load(const std::string& filename, uint8_t alpha)
{
    // Open bmp file and set it to read in the file as binary data (not text)
    std::ifstream stream(filename, std::ios::binary);
    if (stream.fail())
    {
        std::cout << "Error: Cannot open file: " << filename << std::endl;
        return false;
    }

    // Read the bmp header as an array of bytes
    uint8_t header[54];
    stream.read((char*)header, 54);

    // Check for valid bmp file
    uint16_t id = *((uint16_t*)(header));
    if (id != 'MB')
    {
        std::cout << "Error: Invalid file format: " << filename << std::endl;
        return false;
    }

    // Get width and height
    colorBuffer.width = *((int*)(&header[18]));
    colorBuffer.height = *((int*)(&header[22]));

    // Set the pitch (the number of bytes per horizontal line)
    colorBuffer.pitch = colorBuffer.width * sizeof(color_t);

    // Create the image buffer, this will hold the image data to display
    colorBuffer.data = new uint8_t[colorBuffer.width * colorBuffer.pitch];

    // Get the number of bits per pixel of the .bmp
    uint16_t bitsPerPixel = *((uint16_t*)(&header[28]));

    // Calculate bytes per pixel
    uint16_t bytesPerPixel = bitsPerPixel / 8;

    // Calculate the size (in bytes) of the .bmp image data to read
    size_t size = colorBuffer.width * colorBuffer.height * bytesPerPixel;

    // Create a data buffer to read the .bmp image data into
    uint8_t* data = new uint8_t[size];

    // Read the image file data into the .bmp image data buffer
    stream.read((char*)data, size);

    // Set the .bmp image data into the Image class buffer
    for (int i = 0; i < colorBuffer.width * colorBuffer.height; i++)
    {
        color_t color;

        // colors in bmp data are stored (BGR)
        int index = i * bytesPerPixel;
        color.b = data[index];
        color.g = data[index + 1];
        color.r = data[index + 2];
        color.a = alpha;

        ((color_t*)(colorBuffer.data))[i] = color;
    }

    // Release the data buffer that was used to read the .bmp image data into
    delete[] data;

    stream.close();
    return true;
}

void Image::Flip()
{
    // set the pitch (width * number of bytes per pixel)
    int pitch = colorBuffer.width * sizeof(color_t);

    // create temporary line to store data
    uint8_t* temp = new uint8_t[pitch];

    for (int i = 0; i < colorBuffer.height / 2; i++)
    {
        uint8_t* line1 = &((colorBuffer.data)[i * pitch]);
        uint8_t* line2 = &((colorBuffer.data)[((colorBuffer.height - 1) - i) * pitch]);
        memcpy(temp, line1, pitch);
        memcpy(line1, line2, pitch);
        memcpy(line2, temp, pitch);
    }
    delete[] temp;

}
