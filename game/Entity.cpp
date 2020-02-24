//
// Created by omar on 2/20/20.
//

#include "Entity.hpp"

aik::Entity::Entity()
{

}

void aik::Entity::setRotation(glm::vec3 rotation)
{
    isModified_ = true;
    rotation_ = rotation;
}

void aik::Entity::setScale(glm::vec3 scale)
{
    isModified_ = true;
    scale_ = scale;
}

void aik::Entity::setPosition(glm::vec3 position)
{
    isModified_ = true;
    position_ = position;
}

void aik::Entity::move(glm::vec3 offset)
{
    isModified_ = true;
    position_ += offset;
}
