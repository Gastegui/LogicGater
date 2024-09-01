//
// Created by julen on 15/08/24.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
namespace SND
{
    Mix_Music* music {nullptr};


    bool init()
    {
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
        {
            std::cerr << "No se ha podido abrir el audio. Error: " << SDL_GetError() << std::endl;
            return false;
        }

        return true;
    }

    void deInit()
    {
        Mix_HaltChannel(-1);
        //Descargar todos lo sonidos que hayan cargados
        Mix_Quit();
    }
}