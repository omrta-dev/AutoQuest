//
// Created by omar on 4/30/20.
//
#include "Shaders.hpp"
#include <fstream>
#include <iostream>

// loads a shaderFile into a char * buffer
std::vector<char> aik::Shaders::loadShader(std::string shaderFile)
{
    std::ifstream file(shaderFile, std::ios::ate | std::ios::binary);
    if(!file.is_open())
    {
        std::cout << "failed to read shader: " << shaderFile << "\n";
        return std::vector<char>();
    }
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

vk::UniqueShaderModule aik::Shaders::createShaderModule(vk::Device device, const std::vector<char> &shaderCode)
{
    return device.createShaderModuleUnique(vk::ShaderModuleCreateInfo({}, shaderCode.size(), reinterpret_cast<const uint32_t*>(shaderCode.data())));
}
