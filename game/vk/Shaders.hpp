//
// Created by omar on 4/30/20.
//
#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace aik
{
    class Shaders
    {
    public:
        static std::vector<char> loadShader(std::string shaderFile);
        static vk::UniqueShaderModule createShaderModule(vk::Device device, const std::vector<char>& shaderCode);
    private:
    };
}
