#include "PostProcess.h"

namespace PostProcess
{
    void Invert(const ColorBuffer& colorBuffer)
    {
        for (int i = 0; i < colorBuffer.width * colorBuffer.height; i++)
        {
            color_t& color = ((color_t*)colorBuffer.data)[i];

            color.r = 255 - color.r;
            color.g = 255 - color.g;
            color.b = 255 - color.b;
        }
    }

    void Monochrome(const ColorBuffer& colorBuffer)
    {
        for (int i = 0; i < colorBuffer.width * colorBuffer.height; i++)
        {
            color_t& color = ((color_t*)colorBuffer.data)[i];
            uint16_t sum = color.r + color.g + color.b;
            uint8_t average = sum / 3;
            
            color.r = average;
            color.g = average;
            color.b = average;
        }
    }

    void ColorBalance(const ColorBuffer& colorBuffer, int16_t ro, int16_t go, int16_t bo)
    {
        for (int i = 0; i < colorBuffer.width * colorBuffer.height; i++)
        {
            color_t& color = ((color_t*)colorBuffer.data)[i];
            
            int16_t r = color.r + ro;
            int16_t g = color.g + go;
            int16_t b = color.b + bo;

            color.r = ((r < 0) ? 0 : (r > 255) ? 255 : r);
            color.g = ((g < 0) ? 0 : (g > 255) ? 255 : g);
            color.b = ((b < 0) ? 0 : (b > 255) ? 255 : b);
        }
    }

    void Brightness(const ColorBuffer& colorBuffer, int16_t brightness)
    {
        for (int i = 0; i < colorBuffer.width * colorBuffer.height; i++)
        {
            color_t& color = ((color_t*)colorBuffer.data)[i];

            int16_t r = color.r + brightness;
            int16_t g = color.g + brightness;
            int16_t b = color.b + brightness;

            color.r = ((r < 0) ? 0 : (r > 255) ? 255 : r);
            color.g = ((g < 0) ? 0 : (g > 255) ? 255 : g);
            color.b = ((b < 0) ? 0 : (b > 255) ? 255 : b);
        }
    }

    void Noise(const ColorBuffer& colorBuffer, uint8_t noise)
    {
        for (int i = 0; i < colorBuffer.width * colorBuffer.height; i++)
        {
            color_t& color = ((color_t*)colorBuffer.data)[i];
            int16_t offset = (rand() % ((noise << 1) + 1)) - noise;

            int16_t r = color.r + offset;
            int16_t g = color.g + offset;
            int16_t b = color.b + offset;

            color.r = ((r < 0) ? 0 : (r > 255) ? 255 : r);
            color.g = ((g < 0) ? 0 : (g > 255) ? 255 : g);
            color.b = ((b < 0) ? 0 : (b > 255) ? 255 : b);
        }
    }

    void Threshold(const ColorBuffer& colorBuffer, uint8_t threshold)
    {
        for (int i = 0; i < colorBuffer.width * colorBuffer.height; i++)
        {
            color_t& color = ((color_t*)colorBuffer.data)[i];

            color.r = (color.r >= threshold) ? color.r : 0;
            color.g = (color.g >= threshold) ? color.g : 0;
            color.b = (color.b >= threshold) ? color.b : 0;
        }
    }
}
