//
// Created by omar on 4/30/20.
//
#pragma once
#include <string>
namespace aik
{
    class Shaders
    {
    public:
        Shaders() = default;
        ~Shaders() = default;
        bool loadShader(std::string shaderFile);
    private:
    };
}
