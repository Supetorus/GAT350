#include "Framebuffer.h"

//#define SLOPE
#define DDA
//#define BRESENHAM

Framebuffer::Framebuffer(Renderer* renderer, int width, int height)
{
    this->width = width;
    this->height = height;

    texture = SDL_CreateTexture(renderer->renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING, width, height);

    pitch = width * sizeof(color_t);
    buffer = new uint8_t[pitch * height];
}

Framebuffer::~Framebuffer()
{
    SDL_DestroyTexture(texture);
    delete[] buffer;
}

void Framebuffer::Update()
{
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
}

void Framebuffer::Clear(const color_t& color)
{
    for (int i = 0; i < width * height; i++)
    {
        ((color_t*)(buffer))[i] = color;
    }
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    ((color_t*)(buffer))[x + y * width] = color;
}

void Framebuffer::DrawRect(int x, int y, int rect_width, int rect_height, const color_t& color)
{
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    for (int sy = y; sy < y + rect_height; sy++)
    {
        for (int sx = x; sx < x + rect_width; sx++)
        {
            DrawPoint(sx, sy, color);
        }
    }
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

#if defined(SLOPE)
    if (dx == 0)
    {
        if (y1 > y2) std::swap(y1, y2);
        for (int y = y1; y <= y2; y++)
        {
            DrawPoint(x1, y, color);
        }
    }
    else
    {
        float m = dy / (float)dx;
        float b = y1 - (m * x1);

        if (std::abs(dx) > std::abs(dy))
        {
            if (x1 > x2) std::swap(x1, x2);
            for (int x = x1; x <= x2; x++)
            {
                int y = (int)round((m * x) + b);
                DrawPoint(x, y, color);
            }
        }
        else
        {
            if (y1 > y2) std::swap(y1, y2);
            for (int y = y1; y <= y2; y++)
            {
                int x = (int)round((y - b) / m); // y=mx+b == (y-b)/m = x
                DrawPoint(x, y, color);
            }
        }
    }
#elif defined(DDA)
    int steps = std::max(std::abs(dx), std::abs(dy));
    float ddx = dx / (float)steps;
    float ddy = dy / (float)steps;

    float x = (float)x1;
    float y = (float)y1;

    for (int i = 0; i < steps; i++)
    {
        DrawPoint((int)x, (int)y, color);
        x += ddx;
        y += ddy;
    }
//#elif defined(BRESENHAM)
#endif
}

void Framebuffer::DrawCircle(int x, int y, int radius, const color_t& color)
{
}

void Framebuffer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x2, y2, x3, y3, color);
    DrawLine(x3, y3, x1, y1, color);
}

void Framebuffer::DrawSimpleCurve(int x1, int y1, int x2, int y2, int steps, const color_t& color)
{
    float dt = 1.0f / steps;
    for (int i = 0; i < steps; i++)
    {
        float t1 = i * dt;
        float t2 = (i+1) * dt;
        int sx1 = Lerp(x1, x2, t1);
        int sy1 = Lerp(y1, y2, t1);
        int sx2 = Lerp(x1, x2, t2);
        int sy2 = Lerp(y1, y2, t2);
        DrawLine(sx1, sy1, sx2, sy2, color);
    }
}

void Framebuffer::DrawQuadraticCurve(int x1, int y1, int x2, int y2, int x3, int y3, int steps, const color_t& color)
{
    float dt = 1.0f / steps;
    for (int i = 0; i < steps; i++)
    {
        float t1 = i * dt;
        float t2 = (i + 1) * dt;

        float a = (float)pow((1.0f - t1), 2.0f);
        float b = 2.0f * (1.0f - t1) * t1;
        float c = (float)pow(t1, 2.0f);

        int sx = (int)(a * x1 + b * x2 + c * x3);
        int sy = (int)(a * y1 + b * y2 + c * y3);

        DrawPoint(sx, sy, color);
    }
}

int Framebuffer::Lerp(int a, int b, float t)
{
    return (int)(a + ((b - a) * t));
}
