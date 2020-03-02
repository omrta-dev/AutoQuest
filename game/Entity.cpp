//
// Created by omar on 2/20/20.
//

#include "Entity.hpp"
#include <glm/gtc/matrix_transform.hpp>

aik::Entity::Entity()
{
    model_ = glm::mat4(1.0f);
    position_ = glm::vec3(1.0f);
    scale_ = glm::vec3(1.0f);
    rotation_ = glm::vec3(0.0f, 0.0f, 1.0f);
    rotationAngle_ = 0.0f;
    isModified_ = true;
}

void aik::Entity::addVertex(aik::Vertex vertex)
{
    vertices_.push_back(vertex);
}

void aik::Entity::addVertices(std::vector<aik::Vertex> vertices)
{
    vertices_.reserve(vertices_.size() + vertices.size());
    vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
}

void aik::Entity::setVertices(std::vector<aik::Vertex> vertices)
{
    vertices_ = std::move(vertices);
}

void aik::Entity::clearVertices()
{
    vertices_.clear();
}

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

