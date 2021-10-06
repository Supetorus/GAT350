#include "Renderer.h"
#include "Framebuffer.h"

#include <iostream>
#include <SDL.h>

int main(int, char**)
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize(WIDTH, HEIGHT);

	std::unique_ptr<Framebuffer> frameBuffer = std::make_unique<Framebuffer>(renderer.get(), renderer->width, renderer->height);

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

		//for (int i = 0; i < 100; i++)
		//{
		//	frameBuffer->DrawPoint(rand() % frameBuffer->width, rand() % frameBuffer->height, { 255, 255, 255, 255 });
		//}

		color_t color = { rand() % 255, rand() % 255, rand() % 255, 255 };
		frameBuffer->DrawRect(20, 20, 20, 60, color);
		frameBuffer->DrawRect(60, 60, 20, 20, color);
		frameBuffer->DrawRect(100, 20, 20, 60, color);
		frameBuffer->DrawRect(20, 80, 100, 20, color);

		frameBuffer->DrawRect(140, 20, 20, 20, color);
		frameBuffer->DrawRect(140, 60, 20, 40, color);

		frameBuffer->DrawRect(180, 20, 20, 80, color);

		frameBuffer->DrawRect(220, 20, 20, 80, color);

		frameBuffer->Update();

		renderer->CopyBuffer(frameBuffer.get());

		renderer->Present();
	}

	SDL_Quit();

	return 0;
}
