#pragma once
#include "ColorBuffer.h"

namespace PostProcess
{
	void Invert(const ColorBuffer& colorBuffer);
	void Monochrome(const ColorBuffer& colorBuffer);
	void ColorBalance(const ColorBuffer& colorBuffer, int16_t ro, int16_t go, int16_t bo);
	void Brightness(const ColorBuffer& colorBuffer, int16_t brightness);
	void Noise(const ColorBuffer& colorBuffer, uint8_t noise);
	void Threshold(const ColorBuffer& colorBuffer, uint8_t threshold);
}
