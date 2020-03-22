//
// Created by omar on 3/17/20.
//
#pragma once
#include <vulkan/vulkan.hpp>
#include "vk/Vulkanize.hpp"
#include <SDL2/SDL.h>

namespace aik
{
    class Game
    {
    public:
        void startGame();
    private:
        void initializeResources();
        void gameLoop();
        void processInput();
        void update();
        void renderGraphics();

        SDL_Window * window_;
        bool isWindowOpen_;
        aik::Vulkanize vulkanize_;
    };
}
