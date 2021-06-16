#pragma once
#include <SDL.h>

class Display
{
public:
	Display();
	void update(void const* pxBuffe);
	void close();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};

