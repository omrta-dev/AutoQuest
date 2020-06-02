//
// Created by omar on 6/1/20.
//
#include "CoordinateHelper.hpp"

bool aik::utility::CoordinateHelper::pointIntersectsBox(glm::vec2 point, glm::vec4 box)
{
    return (point.x > box.x) && (point.x < box.x + box.z) &&
           (point.y > box.y) && (point.y < box.y + box.w);
}
