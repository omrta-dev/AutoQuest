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
        static inline std::vector<glm::vec4> getVertices() { return {{.5f, .5f, 0., 0. },
                                                                     {.5f, -.5f, 0., 1.},
                                                                     {-.5f, -.5f, 1., 1.},
                                                                     {-.5f, .5f, 1., 0}}; }
        static inline std::vector<unsigned int> getIndices() { return {0, 1, 3, 1, 2, 3}; }
    };
}