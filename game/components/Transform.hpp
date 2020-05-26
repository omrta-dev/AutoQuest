//
// Created by omar on 5/25/20.
//
#pragma once
#include <glm/vec2.hpp>

namespace aik::Component
{
    class Transform
    {
    public:
        glm::vec2 position;
        glm::vec2 rotation = glm::vec2(0.0f);
        glm::vec2 scale = glm::vec2(1.0f);
    };
}