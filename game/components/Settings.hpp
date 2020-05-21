//
// Created by omar on 5/20/20.
//
#pragma once

namespace aik
{
    // defines the settings the game will globally have
    struct Settings
    {
        int antiAliasing = 2;
        int width = 1080;
        int height = 1920;
        int depth = 4;
        int stencil = 0;
        int fps = 60;
        unsigned int style = 0; // Equivalent to sf::Style::None
    };
}