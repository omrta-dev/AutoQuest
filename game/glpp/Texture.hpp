//
// Created by omar on 3/6/20.
//

#pragma once
#include <string>

namespace aik
{
    // C++ OpenGL Texture Wrapper
    class Texture
    {
    public:
        static Texture LoadTexture(const std::string& directory, const std::string& file, const std::string& type);
        bool operator==(const std::string& right) const;
        unsigned int id_;
        std::string type_;
        std::string path_;
    };
}
