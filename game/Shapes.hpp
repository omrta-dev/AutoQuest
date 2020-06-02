//
// Created by omar on 5/22/20.
//
#pragma once
#include <vector>
#include <glm/vec4.hpp>

namespace aik::Shape
{
    struct Square
    {
        static inline std::vector<glm::vec4> getVertices() { return {{1.f, 1.f, 0., 0. },
                                                                     {1.f, .0f, 0., 1.},
                                                                     {0.f, 0.f, 1., 1.},
                                                                     {0.f, 1.f, 1., 0}}; }
        static inline std::vector<unsigned int> getIndices() { return {0, 1, 3, 1, 2, 3}; }
    };
}