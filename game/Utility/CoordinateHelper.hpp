//
// Created by omar on 6/1/20.
//
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace aik::utility
{
    class CoordinateHelper
    {
    public:
        static bool pointIntersectsBox(glm::vec2 point, glm::vec4 box);
    };
};
