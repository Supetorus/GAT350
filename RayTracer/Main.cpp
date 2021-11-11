#include "Renderer.h"
#include "Framebuffer.h"
#include "PostProcess.h"
#include "Image.h"
#include "Tracer.h"
#include "Scene.h"
#include "Camera.h"
#include "Buffer.h"

#include <SDL.h>
#include <iostream>
#include <string>
#include <fstream>

int main(int, char**)
{
	srand((unsigned int)time(nullptr));

	const int WIDTH = 800;
	const int HEIGHT = 600;
	int samples = 0;
	std::string fileName;
	int sceneNumber;
	Image image;
	std::ifstream fileIn;
	std::ofstream fileOut;

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize(WIDTH, HEIGHT);

	std::unique_ptr<Framebuffer> framebuffer = std::make_unique<Framebuffer>(renderer.get(), renderer->width, renderer->height);
	std::unique_ptr<Buffer> accumBuffer = std::make_unique<Buffer>(renderer->width, renderer->height);
	std::unique_ptr<Buffer> buffer = std::make_unique<Buffer>(renderer->width, renderer->height);

	// ray tracer
	std::unique_ptr<Tracer> tracer = std::make_unique<Tracer>();

	// samplers
	std::vector<std::shared_ptr<Sampler>> samplers;

	samplers.push_back(std::make_unique<TextureSampler>(std::make_unique<Image>("../resources/earth.bmp")));
	samplers.push_back(std::make_unique<TextureSampler>(std::make_unique<Image>("../resources/metal.bmp")));
	samplers.push_back(std::make_unique<TextureSampler>(std::make_unique<Image>("../resources/planet.bmp")));
	samplers.push_back(std::make_unique<TextureSampler>(std::make_unique<Image>("../resources/wood.bmp")));
	samplers.push_back(std::make_unique<TextureSampler>(std::make_unique<Image>("../resources/lava.bmp")));
	samplers.push_back(std::make_unique<ColorSampler>(glm::vec3{ 1.0f, 0.2f, 0.2f }));
	samplers.push_back(std::make_unique<ColorSampler>(glm::vec3{ 0.2f, 1.0f, 0.2f }));
	samplers.push_back(std::make_unique<ColorSampler>(glm::vec3{ 0.2f, 0.2f, 1.0f }));
	samplers.push_back(std::make_unique<ColorSampler>(glm::vec3{ 0.2f, 1.0f, 1.0f }));
	samplers.push_back(std::make_unique<ColorSampler>(glm::vec3{ 1.0f, 1.0f, 0.2f }));
	samplers.push_back(std::make_unique<ColorSampler>(glm::vec3{ 1.0f, 0.2f, 1.0f }));
	samplers.push_back(std::make_unique<ColorSampler>(glm::vec3{ 0.8f, 0.8f, 0.8f }));

	std::shared_ptr<CheckerSampler> black_checker = std::make_unique<CheckerSampler>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 1, 1, 1 }, 5.0f);
	std::shared_ptr<CheckerSampler> red_checker = std::make_unique<CheckerSampler>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 1, 0, 0 }, 3.0f);

	// scene
	std::unique_ptr<Scene> scene = std::make_unique<Scene>();

	std::string load;
	std::cout << "Load previous? y/n >> ";
	std::cin >> load;
	if (load == "y" || load == "Y")
	{
		float x, y, z, radius, fuzz, refractionIndex;
		int samplerNumber;
		std::string objType;

		std::cout << "Enter scene Number >> ";
		std::cin >> sceneNumber;
		std::cout << "Enter samples completed >> ";
		std::cin >> samples;
		if (samples == 0)
		{
			framebuffer->Clear({ 0, 0, 0, 0 });
			samples++;
		}
		else
		{
			if (!image.Load("../input/Scene" + std::to_string(sceneNumber) + "-samples" + std::to_string(samples) + ".bmp"))
				image.Load("../input/Scene" + std::to_string(sceneNumber) + "-latest.bmp");
			image.Flip();
			samples++;
			framebuffer->colorBuffer.data = image.colorBuffer.data;
		}

		fileIn.open("../input/scene" + std::to_string(sceneNumber) + ".txt");
		while (!fileIn.eof())
		{
			fileIn >> x;
			fileIn >> y;
			fileIn >> z;
			fileIn >> radius;
			fileIn >> samplerNumber;
			fileIn >> objType;

			std::shared_ptr<Material> material;
			std::shared_ptr<Sampler> sampler = samplers[samplerNumber];

			if (objType == "lambertian")
			{
				material = std::make_shared<Lambertian>(sampler);
			}
			else if (objType == "metal")
			{
				fileIn >> fuzz;
				material = std::make_shared<Metal>(sampler, fuzz);
			}
			else if (objType == "dielectric")
			{
				fileIn >> refractionIndex;
				material = std::make_shared<Dielectric>(sampler, refractionIndex);
			}
			scene->Add(std::move(std::make_unique<Sphere>(glm::vec3{ x, y, z }, radius, material)));
		}
		fileIn.close();
		framebuffer->Update();
		renderer->CopyBuffer(framebuffer.get());
		renderer->Present();
	}
	else
	{
		fileIn.open("../input/sceneNumber.txt");
		fileIn >> sceneNumber;
		fileIn.close();

		std::cout << "Scene Number is " << sceneNumber << std::endl;

		fileOut.open("../input/sceneNumber.txt");
		sceneNumber;
		fileOut << sceneNumber + 1 << std::endl;
		fileOut.close();

		fileOut.open("../output/scene" + std::to_string(sceneNumber) + ".txt");
		for (int x = -10; x < 10; x++)
		{
			for (int z = -10; z < 10; z++)
			{
				glm::vec3 position{ x + random(0.1f, 0.9f), 0.2f, z + random(0.1f, 0.9f) };
				float radius = random(0.2f, 0.3f);

				std::shared_ptr<Material> material;
				int samplerNumber = rand() % samplers.size();
				std::shared_ptr<Sampler> sampler = samplers[samplerNumber];


				fileOut << position.x << std::endl;
				fileOut << position.y << std::endl;
				fileOut << position.z << std::endl;
				fileOut << radius << std::endl;
				fileOut << samplerNumber << std::endl;

				float r = random01();
				if (r < 0.8f)
				{
					fileOut << "lambertian" << std::endl;
					material = std::make_shared<Lambertian>(sampler);
				}
				else if (r < 0.95f)
				{
					fileOut << "metal" << std::endl;
					float fuzz = random(0.0f, 0.5f);
					material = std::make_shared<Metal>(sampler, fuzz);
					fileOut << fuzz << std::endl;
				}
				else
				{
					fileOut << "dielectric" << std::endl;
					float refractionIndex = random(1.0f, 2.5f);
					fileOut << refractionIndex << std::endl;
					material = std::make_shared<Dielectric>(sampler, refractionIndex);
				}

				scene->Add(std::move(std::make_unique<Sphere>(position, radius, material)));
			}
		}
		fileOut.close();
		framebuffer->Clear({ 0, 0, 0, 0 });
	}

	scene->Add(std::move(std::make_unique<Plane>(glm::vec3{ 0, -0.01f, 0 }, glm::vec3{ 0, 1, 0 }, std::make_shared<Lambertian>(black_checker))));
	scene->Add(std::move(std::make_unique<Sphere>(glm::vec3{ 0, 1, 0 }, 1.0f, std::make_shared<Metal>(glm::vec3{ 0.8f, 0.8f, 0.8f }, 0.0f))));
	scene->Add(std::move(std::make_unique<Sphere>(glm::vec3{ 4, 1, 0 }, 1.0f, std::make_shared<Dielectric>(glm::vec3{ 1, 1, 1 }, 1.5f))));


	scene->Add(std::move(std::make_unique<Sphere>(glm::vec3{ 0, 30, 0 }, 10.0f, std::make_shared<Emissive>(glm::vec3{ 10, 10, 10 }))));

	// camera
	glm::vec3 eye{ 13, 3, 3 };
	glm::vec3 lookAt{ 0, 0, 0 };
	float focalLength = glm::length(eye - lookAt);
	std::unique_ptr<Camera> camera = std::make_unique<Camera>(eye, lookAt, glm::vec3{ 0, 1, 0 }, 20.0f, glm::ivec2{ framebuffer->colorBuffer.width, framebuffer->colorBuffer.height }, 0.01f, focalLength);

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		// render to accumulation buffer
		samples += tracer->samples;
		std::string message = "Sample # " + std::to_string(samples);
		tracer->Trace(accumBuffer.get(), scene.get(), camera.get(), message);

		// copy accumulation buffer to buffer
		*buffer.get() = *accumBuffer.get();
		// process buffer values (average + sqrt)
		buffer->Process(samples);

		// copy buffer to frame buffer
		buffer->Copy(framebuffer->colorBuffer);
		framebuffer->Update();

		// copy frame buffer to renderer
		renderer->CopyBuffer(framebuffer.get());
		renderer->Present();
		//std::cout << "Samples " << samples << std::endl;

		//tracer->Trace(framebuffer->colorBuffer, scene.get(), camera.get(), samples);
		if (samples % 100 == 0 || samples == 1) fileName = "../output/Scene" + std::to_string(sceneNumber) + "-samples" + std::to_string(samples) + ".bmp";
		else fileName = "../output/Scene" + std::to_string(sceneNumber) + "-latest.bmp";
		Image::saveImage(fileName.c_str(), framebuffer->colorBuffer);
	}

	SDL_Quit();

	return 0;
}
