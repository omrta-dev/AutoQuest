//
// Created by omar on 5/20/20.
//
#pragma once
#include <glm/vec2.hpp>

namespace aik::Component
{
    // defines the settings the game will globally have
    struct Settings
    {
        template<typename Archive>
        void serialize(Archive &archive)
        {
            archive(antiAliasing, width, height, depth, stencil, fps, style, position.x, position.y);
        }

        int antiAliasing = 2;
        unsigned int width = 1920;
        unsigned int height = 1080;
        int depth = 4;
        int stencil = 0;
        int fps = 60;
        unsigned int style = 0; // Equivalent to sf::Style::None
        glm::ivec2 position = {500,500};
    };
}