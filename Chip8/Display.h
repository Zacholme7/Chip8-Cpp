#pragma once
#include <SDL.h>
#include <string>
#include <iostream>

class Display
{
public:
	Display(int scale);
	void update(void const* pxBuffer, int pitch);
	void close();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	
};

