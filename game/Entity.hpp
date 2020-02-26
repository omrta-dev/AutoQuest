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

/*
 * Entity class which will be the base class for any drawable openGL entity
 * Contains transformation functions, setting vertices and forming the model matrix
 */

namespace aik
{
    class Entity
    {
    public:
        Entity();
        void addVertex(aik::Vertex vertex);
        void addVertices(std::vector<aik::Vertex> vertices);
        void setVertices(std::vector<aik::Vertex> vertices);
        void clearVertices();
        inline const std::vector<aik::Vertex>& getVertices() {return vertices_;}
        void setRotation(glm::vec3 rotationAxis, float degree);
        inline glm::vec3 getRotation() const {return rotation_;}
        void setScale(glm::vec3 scale);
        inline glm::vec3 getScale() const {return scale_;}
        void setPosition(glm::vec3 position);
        inline glm::vec3 getPosition() const {return position_;}
        void move(glm::vec3 offset);
        void rotate(glm::vec3 rotationAxis, float degree);
        const glm::mat4& getModel();
    private:
        bool isModified_;
        std::vector<aik::Vertex> vertices_;
        glm::vec3 rotation_;
        float rotationAngle_;
        glm::vec3 scale_;
        glm::vec3 position_;
        glm::mat4 model_;
    };
}
