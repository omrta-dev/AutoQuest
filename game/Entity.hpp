//
// Created by omar on 2/20/20.
//

#pragma once

#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "glpp/Vertex.hpp"

namespace aik
{
    class Entity
    {
    public:
        Entity();
        void setRotation(glm::vec3 rotation);
        inline glm::vec3 getRotation() const {return rotation_;}
        void setScale(glm::vec3 scale);
        inline glm::vec3 getScale() const {return scale_;}
        void setPosition(glm::vec3 position);
        inline glm::vec3 getPosition() const {return position_;}
        void move(glm::vec3 offset);
    private:
        bool isModified_;
        std::vector<aik::Vertex> vertices_;
        glm::vec3 rotation_;
        glm::vec3 scale_;
        glm::vec3 position_;
    };
}
