#pragma once
#include "ColorBuffer.h"
#include <string>

class Image
{
public:
	~Image();

	bool Load(const std::string& filename, uint8_t alpha = 255);
	void Flip();

	friend class Framebuffer;

public:
	ColorBuffer colorBuffer;

private:
	//uint8_t* buffer{ nullptr };
	//int width;
	//int height;
};
