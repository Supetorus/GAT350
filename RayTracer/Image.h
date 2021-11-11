#pragma once
#include "ColorBuffer.h"
#include <string>

class Image
{
public:
	~Image();
	Image() = default;
	Image(const std::string& filename, uint8_t alpha = 255);

	bool Load(const std::string& filename, uint8_t alpha = 255);
	void Flip();
	static void saveImage(const char* filename, const ColorBuffer& colorBuffer);
	friend class Framebuffer;

public:
	ColorBuffer colorBuffer;

private:
	//uint8_t* buffer{ nullptr };
	//int width;
	//int height;
};
