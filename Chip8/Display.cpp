#include "Display.h"

Display::Display()
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("chip 8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	texture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
}


void Display::update(void const* pxBuffer )
{
	SDL_UpdateTexture(texture, nullptr, pxBuffer, 64 * sizeof(Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

void Display::close()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	
	texture = NULL;
	renderer = NULL;
	window = NULL;

	SDL_Quit();
}
