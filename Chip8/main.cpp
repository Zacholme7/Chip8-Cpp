#include "Chip8.h"
#include "Display.h"

uint8_t keymap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

const int FPS = 60;
const int frameDelay = 1000 / FPS;


int main(int argc, char* args[])
{
	Chip8 chip8 = Chip8(); 
    chip8.loadRom("roms/INVADERS");
	Display display(10);

    int pitch = sizeof(chip8.screen[0]) * 64;
    uint32_t frameStart;
    int frameTime;

	while (true)
	{
        frameStart = SDL_GetTicks();
        // CHECK KEYPRESS/RELEASE
		SDL_Event e;
        chip8.execute();
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				exit(0);
			}
            else if (e.type == SDL_KEYDOWN)
            {
                for (int i = 0; i < 16; i++)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        chip8.setKey(i);
                    }
                }
            }
            else if (e.type == SDL_KEYUP)
            {
                for (int i = 0; i < 16; i++)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        chip8.unsetKey(i);
                    }
                }
            }
		}


        if (chip8.drawFlag)
        {
            chip8.drawFlag = false;
            display.update(reinterpret_cast<void *>(chip8.screen.data()), pitch);

        }
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        
	}

    // clean up and quit
    display.close();
}
