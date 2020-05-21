//
// Created by omar on 5/18/20.
//

#include "Entity.hpp"
#include <glm/gtc/matrix_transform.hpp>

void aik::Entity::setRotation(glm::vec3 rotationAxis, float degree)
{
    isModified_ = true;
    rotation_ = rotationAxis;
    rotationAngle_ = degree;
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

void aik::Entity::rotate(glm::vec3 rotationAxis, float degree)
{
    rotation_ = rotationAxis;
    rotationAngle_ += degree;
    isModified_ = true;
}

const glm::mat4& aik::Entity::getModel()
{
    if(isModified_)
    {
        model_ = glm::translate(glm::mat4(1.0f), position_);
        model_ = glm::scale(model_, scale_);
        if(rotationAngle_ != 0.0f)
            model_ = glm::rotate(model_, glm::radians(rotationAngle_), rotation_);
        isModified_ = false;
    }
    return model_;
}

std::vector<aik::Vertex> aik::Entity::getVertices()
{
    return std::vector<aik::Vertex>();
}

std::vector<unsigned int> aik::Entity::getIndices()
{
    return std::vector<unsigned int>();
}
