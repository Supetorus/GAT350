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

void Image::saveImage(const char* filename, const ColorBuffer& colorBuffer) {
    int WIDTH = colorBuffer.width;
    int HEIGHT = colorBuffer.height;

    unsigned int headers[13];
    FILE* outfile;
    int extrabytes;
    int paddedsize;
    int x; int y; int n;
    int red, green, blue;

    extrabytes = 4 - ((WIDTH * 3) % 4);                 // How many bytes of padding to add to each
                                                        // horizontal line - the size of which must
                                                        // be a multiple of 4 bytes.
    if (extrabytes == 4)
        extrabytes = 0;

    paddedsize = ((WIDTH * 3) + extrabytes) * HEIGHT;

    // Headers...
    // Note that the "BM" identifier in bytes 0 and 1 is NOT included in these "headers".

    headers[0] = paddedsize + 54;      // bfSize (whole file size)
    headers[1] = 0;                    // bfReserved (both)
    headers[2] = 54;                   // bfOffbits
    headers[3] = 40;                   // biSize
    headers[4] = WIDTH;  // biWidth
    headers[5] = HEIGHT; // biHeight

    // Would have biPlanes and biBitCount in position 6, but they're shorts.
    // It's easier to write them out separately (see below) than pretend
    // they're a single int, especially with endian issues...

    headers[7] = 0;                    // biCompression
    headers[8] = paddedsize;           // biSizeImage
    headers[9] = 0;                    // biXPelsPerMeter
    headers[10] = 0;                    // biYPelsPerMeter
    headers[11] = 0;                    // biClrUsed
    headers[12] = 0;                    // biClrImportant

    outfile = fopen(filename, "wb");

    //
    // Headers begin...
    // When printing ints and shorts, we write out 1 character at a time to avoid endian issues.
    //

    fprintf(outfile, "BM");

    for (n = 0; n <= 5; n++)
    {
        fprintf(outfile, "%c", headers[n] & 0x000000FF);
        fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
        fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
        fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
    }

    // These next 4 characters are for the biPlanes and biBitCount fields.

    fprintf(outfile, "%c", 1);
    fprintf(outfile, "%c", 0);
    fprintf(outfile, "%c", 24);
    fprintf(outfile, "%c", 0);

    for (n = 7; n <= 12; n++)
    {
        fprintf(outfile, "%c", headers[n] & 0x000000FF);
        fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
        fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
        fprintf(outfile, "%c", (headers[n] & (unsigned int)0xFF000000) >> 24);
    }

    //
    // Headers done, now write the data...
    //

    for (y = HEIGHT - 1; y >= 0; y--)     // BMP image format is written from bottom to top...
    {
        for (x = 0; x <= WIDTH - 1; x++)
        {

            red = colorBuffer.GetColor(x, y).r;
            green = colorBuffer.GetColor(x, y).g;
            blue = colorBuffer.GetColor(x, y).b;

            if (red > 255) red = 255; if (red < 0) red = 0;
            if (green > 255) green = 255; if (green < 0) green = 0;
            if (blue > 255) blue = 255; if (blue < 0) blue = 0;

            // Also, it's written in (b,g,r) format...

            fprintf(outfile, "%c", blue);
            fprintf(outfile, "%c", green);
            fprintf(outfile, "%c", red);
        }
        if (extrabytes)      // See above - BMP lines must be of lengths divisible by 4.
        {
            for (n = 1; n <= extrabytes; n++)
            {
                fprintf(outfile, "%c", 0);
            }
        }
    }

    fclose(outfile);
    return;
}