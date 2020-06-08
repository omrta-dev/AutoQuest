//
// Created by omar on 6/1/20.
//
#include "CoordinateHelper.hpp"

bool aik::utility::CoordinateHelper::pointIntersectsBox(glm::vec2 point, glm::vec4 box, const aik::Camera& camera)
{
    return (point.x > box.x + camera.position_.x) && (point.x < box.x + camera.position_.x + box.z) &&
           (point.y > box.y + camera.position_.y) && (point.y < box.y + camera.position_.y + box.w);
}
