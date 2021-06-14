#pragma once
#include <SDL.h>
#include <string>
#include <iostream>

class Display
{
public:
	Display(int width, int height);
	void update();
	void close();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	
};

