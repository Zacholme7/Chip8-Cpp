#include "Chip8.h"
#include "Display.h"

const unsigned int videoWidth = 512;
const unsigned int videoHeight = 256;
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

int main(int argc, char* args[])
{
	Chip8 chip8 = Chip8(); 
	Display display(videoWidth, videoHeight);

	while (true)
	{
        // CHECK KEYPRESS/RELEASE
		SDL_Event e;
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


        }

        
	}

    // clean up and quit
    display.close();
}
