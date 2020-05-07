//
// Created by omar on 3/17/20.
//

#include "Game.hpp"
#include <iostream>

#include <SDL2/SDL_vulkan.h>

void aik::Game::startGame()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
    }
    window_ = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1440, 1080,
            SDL_WINDOW_VULKAN | SDL_WINDOW_BORDERLESS);


    if(!window_)
    {
        std::cout << "Failed to create window\n";
        std::cout << "SDL2 Error: " << SDL_GetError() << "\n";
        std::exit(-1);
    }
    isWindowOpen_ = true;
    initializeResources();
    gameLoop();
}

void aik::Game::initializeResources()
{
    if(!vulkanize_.setup(window_))
    {
        std::cout << "Failed to setup vulkan" << std::endl;
        std::exit(-1);
    }
}

void aik::Game::gameLoop()
{
    while(isWindowOpen_)
    {
        processInput();
        update();
        renderGraphics();
    }
    vulkanize_.idleDevice();
}

void aik::Game::processInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                isWindowOpen_ = false;
                break;
        }
    }
}

void aik::Game::update()
{

}

void aik::Game::renderGraphics()
{
    vulkanize_.renderScene();
}
