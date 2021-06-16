#include "Chip8.h"
#include "Display.h"
#include <chrono>
#include <thread>

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
    chip8.loadRom("roms/MISSILE");
	Display display;
    bool quit = false;
    uint32_t pixels[2048];

    while (!quit)
    {
        SDL_Event e;
        chip8.execute();
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                for (int i = 0; i < 16; i++)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        chip8.setKey(i);
                    }
                }
                break;
            case SDL_KEYUP:
                for (int i = 0; i < 16; i++)
                {
                    if (e.key.keysym.sym == keymap[i])
                    {
                        chip8.unsetKey(i);
                    }
                }
                break;
            }
        }


        if (chip8.getDrawFlag())
        {
            chip8.resetDrawFlag();
            for (int i = 0; i < 2048; ++i)
            {
                pixels[i] = (chip8.screen[i] == 0) ? 0x000000FF : 0xFFFFFFFF;
            }

            display.update(pixels);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(2000));
    }

    display.close();
    return 0;
}
