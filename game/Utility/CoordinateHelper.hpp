//
// Created by omar on 6/1/20.
//
#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <systems/Camera.hpp>

namespace aik::utility
{
    class CoordinateHelper
    {
    public:
        // returns true if the point is contained in the box after camera transformations ( screen to world interesection )
        static bool pointIntersectsBox(glm::vec2 point, glm::vec4 box, aik::Camera& camera);
    };
};
