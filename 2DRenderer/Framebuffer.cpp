#include "Framebuffer.h"

Framebuffer::Framebuffer(Renderer* renderer, int width, int height)
{
    this->width = width;
    this->height = height;
}

Framebuffer::~Framebuffer()
{
    SDL_DestroyTexture(texture);
    delete[] buffer;
}

void Framebuffer::Update()
{
}

void Framebuffer::Clear(const color_t& color)
{
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
}

void Framebuffer::DrawRect(int x, int y, int width, int height, const color_t& color)
{
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{

}

void Framebuffer::DrawCircle(int x, int y, int radius, const color_t& color)
{
}
