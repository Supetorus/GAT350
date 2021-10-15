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
	image->Load("../resources/cat.bmp");
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

		frameBuffer->DrawImage(30, 30, image.get());

		frameBuffer->DrawCircle(300, 400, 100, { 255, 255, 255,255 });

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
