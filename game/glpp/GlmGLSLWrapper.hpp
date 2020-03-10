//
// Created by omar on 2/25/20.
//

#pragma once

#include <SFML/Graphics/Glsl.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>


// Glm to sfml GLSL wrapper matrices
namespace aik
{
    class GlmGLSLWrapper
    {
    public:
        inline static sf::Glsl::Mat4 GlmToGlslMat4(const glm::mat4& glmmat4)
        {
            return sf::Glsl::Mat4(glm::value_ptr(glmmat4));
        }
    };
}
