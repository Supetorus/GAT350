#include "Renderer.h"
#include "Framebuffer.h"
#include "Image.h"
#include "PostProcess.h"
#include <iostream>
#include <SDL.h>

int main(int, char**)
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize(WIDTH, HEIGHT);

	std::unique_ptr<Framebuffer> frameBuffer = std::make_unique<Framebuffer>(renderer.get(), renderer->width, renderer->height);

	std::unique_ptr<Image> image = std::make_unique<Image>();
	image->Load("../resources/flower.bmp", 192);
	image->Flip();

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

		frameBuffer->Clear({ 0, 0, 0, 0 });

		for (int i = 0; i < 50; i++)
		{
			frameBuffer->DrawRect(rand() % renderer->width, rand() % renderer->height, 50, 50,
				{ (uint8_t)((rand() % 2) * 255), (uint8_t)((rand() % 2) * 255) , (uint8_t)((rand() % 2) * 255) , (uint8_t)(rand() % 255) });
		}

		frameBuffer->DrawImage(300, 50, image.get());

		std::unique_ptr<Image> image1 = std::make_unique<Image>(*image.get());
		PostProcess::BoxBlur(image1->colorBuffer);
		frameBuffer->DrawImage(0, 300, image1.get());

		std::unique_ptr<Image> image2 = std::make_unique<Image>(*image.get());
		PostProcess::GaussianBlur(image2->colorBuffer);
		frameBuffer->DrawImage(200, 300, image2.get());

		std::unique_ptr<Image> image3 = std::make_unique<Image>(*image.get());
		PostProcess::Sharpen(image3->colorBuffer);
		frameBuffer->DrawImage(400, 300, image3.get());

		std::unique_ptr<Image> image4 = std::make_unique<Image>(*image.get());
		PostProcess::Monochrome(image4->colorBuffer);
		PostProcess::Edge(image4->colorBuffer, 0);
		frameBuffer->DrawImage(600, 300, image4.get());

		//for (int i = 0; i < 50; i++)
		//{
		//	frameBuffer->DrawRect(rand() % renderer->width, rand() % renderer->height, 50, 50,
		//		{(uint8_t)((rand() % 2) * 255), (uint8_t)((rand() % 2) * 255) , (uint8_t)((rand() % 2) * 255) , (uint8_t)((rand() % 255)) });
		//}
		//frameBuffer->DrawCircle(300, 400, 100, { 255, 255, 255,255 });

		//PostProcess::Invert(frameBuffer->colorBuffer);
		//PostProcess::Monochrome(frameBuffer->colorBuffer);
		//PostProcess::Noise(frameBuffer->colorBuffer, 100);
		//PostProcess::Brightness(frameBuffer->colorBuffer, -100);
		//PostProcess::ColorBalance(frameBuffer->colorBuffer, 0, 0, 100);
		//PostProcess::Threshold(frameBuffer->colorBuffer, 200);

		frameBuffer->Update();

		renderer->CopyBuffer(frameBuffer.get());

		renderer->Present();
	}

	SDL_Quit();

	return 0;
}
