//
// Created by omar on 6/1/20.
//
#include "CoordinateHelper.hpp"

bool aik::utility::CoordinateHelper::pointIntersectsBox(glm::vec2 point, glm::vec4 box, aik::Camera& camera)
{
    return true;//(point.x > box.x + cameraPos.x) && (point.x < box.x + cameraPos.x + box.z) &&
           //(point.y > box.y + cameraPos.y) && (point.y < box.y + cameraPos.y + box.w);
}
