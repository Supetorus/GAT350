#include "Tracer.h"
#include "Scene.h"
#include "Camera.h"
#include <iostream>
#include <iomanip>
#include <chrono>

void DisplayTime(std::chrono::steady_clock::rep milliseconds)
{
	std::cout << std::setw(2) << std::setfill('0') << (milliseconds / (1000 * 60 * 60));						// hours
	std::cout << ":" << std::setw(2) << std::setfill('0') << ((milliseconds % (1000 * 60 * 60)) / (1000 * 60));	// minutes
	std::cout << ":" << std::setw(2) << std::setfill('0') << ((milliseconds % (1000 * 60)) / 1000);				// seconds
	std::cout << ":" << std::setw(2) << std::setfill('0') << ((milliseconds % 1000) / 10);						// 1/10 seconds
}

void Tracer::Trace(Buffer* buffer, Scene* scene, Camera* camera, const std::string& message)
{
	float aspectRatio = buffer->width / (float)buffer->height;
	float invSamples = 1.0f / samples;
	std::chrono::steady_clock::rep totalTime = 0;

	for (int y = 0; y < buffer->height; y++)
	{
		auto start = std::chrono::steady_clock::now();
		for (int x = 0; x < buffer->width; x++)
		{
			glm::vec3 color = { 0, 0, 0 };

			for (int sample = 0; sample < samples; sample++)
			{
				glm::vec2 viewport = camera->ScreenToViewport({ x + random01(), y + random01() });
				viewport.y = 1 - (viewport.y);
				ray_t ray = camera->ViewportToRay(viewport);

				raycastHit_t hit;
				color += scene->Trace(ray, 0.001f, FLT_MAX, hit, depth);
			}

			buffer->Add(x, y, color);
		}

		// calculate time
		auto end = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		totalTime = totalTime + elapsedTime;
		std::chrono::steady_clock::rep averageTime = totalTime / (y + 1);
		std::chrono::steady_clock::rep estimatedTime = averageTime * buffer->height;

		// display time
		//system("CLS");

		//std::cout << message << std::endl;
		//std::cout << "-----------------------------------------------------------------------------------------------------" << std::endl;
		//std::cout << "| scanline |      time       |   average time  |    total time   | estimated time  | remaining time |" << std::endl;
		//std::cout << "|----------|-----------------|-----------------|-----------------|-----------------|-----------------" << std::endl;
		//std::cout << "| ";
		//std::cout << std::setw(4) << std::setfill('0') << (y + 1) << "/";
		//std::cout << std::setw(4) << std::setfill('0') << buffer->height;
		//
		//std::cout << "|   ";
		//DisplayTime(elapsedTime);

		//std::cout << "   |   ";
		//DisplayTime(averageTime);

		//std::cout << "   |   ";
		//DisplayTime(totalTime);

		//std::cout << "   |   ";
		//DisplayTime(estimatedTime);

		//std::cout << "   |   ";
		//DisplayTime(estimatedTime - totalTime);
		//
		//std::cout << "  |" << std::endl;
	}
}
