#pragma once
#include "Types.h"

struct Buffer
{
	Buffer() = default;
	Buffer(int width, int height)
	{
		this->width = width;
		this->height = height;
		data = new glm::vec3[width * height];
		std::fill(data, &data[width * height], glm::vec3{ 0, 0, 0 });
	}
	Buffer(const Buffer& other)
	{
		width = other.width;
		height = other.height;

		if (other.data)
		{
			data = new glm::vec3[width * height];
			memcpy(data, other.data, width * height * sizeof(glm::vec3));
		}
	}
	Buffer& operator = (const Buffer& other)
	{
		if (data == nullptr)
		{
			width = other.width;
			height = other.height;
			data = new glm::vec3[width * height];
		}

		memcpy(data, other.data, width * height * sizeof(glm::vec3));

		return *this;
	}

	~Buffer() { delete[] data; }

	inline void Add(int x, int y, const glm::vec3& v) const;
	inline void Set(int x, int y, const glm::vec3& v) const;
	inline void Copy(const ColorBuffer& colorBuffer) const;
	inline void Process(int samples) const;
	

	glm::vec3* data{ nullptr };

	int width = 0;
	int height = 0;
};

inline void Buffer::Add(int x, int y, const glm::vec3& v) const
{
	if (x < 0 || x >= width || y < 0 || y >= height) return;

	data[x + y * width] += v;
}

inline void Buffer::Set(int x, int y, const glm::vec3& v) const
{
	if (x < 0 || x >= width || y < 0 || y >= height) return;

	data[x + y * width] = v;
}

inline void Buffer::Copy(const ColorBuffer& colorBuffer) const
{
	for (int i = 0; i < width * height; i++)
	{
		glm::vec3 v = data[i];

		int x = i % width;
		int y = i / width;
		colorBuffer.SetColor(x, y, Vec3ToColor(v));
	}
}

inline void Buffer::Process(int samples) const
{
	float invSamples = 1.0f / samples;

	for (int i = 0; i < width * height; i++)
	{
		glm::vec3& v = data[i];
		v.r = sqrt(v.r * invSamples);
		v.g = sqrt(v.g * invSamples);
		v.b = sqrt(v.b * invSamples);
	}
}

