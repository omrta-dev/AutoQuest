//
// Created by omar on 5/18/20.
//
#pragma once
#include <vector>
#include "glpp/Vertex.hpp"
#include <glm/mat4x4.hpp>
#include "Mesh.hpp"

namespace aik
{
    class Entity
    {
    public:
        void setRotation(glm::vec3 rotationAxis, float degree);
        void setScale(glm::vec3 scale);
        void setPosition(glm::vec3 position);
        void move(glm::vec3 offset);
        void rotate(glm::vec3 rotationAxis, float degree);
        const glm::mat4 &getModel();
        virtual std::vector<aik::Vertex> getVertices();
        virtual std::vector<unsigned int> getIndices();

        std::vector<Mesh> meshes_;
        bool isModified_;
        glm::vec3 rotation_{};
        float rotationAngle_;
        glm::vec3 scale_{};
        glm::vec3 position_{};
        glm::mat4 model_{};
        unsigned int maxIndex_;
        unsigned long verticesCount_;
    };
}
