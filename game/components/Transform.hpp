//
// Created by omar on 5/25/20.
//
#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace aik::Component
{
    class Transform
    {
    public:
        glm::vec3 position;
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        glm::vec3 lastPosition = glm::vec3(1.0f);
        glm::vec3 lastRotation = glm::vec3(0.0f);
        glm::vec3 lastScale = glm::vec3(1.0f);

        glm::mat4 model = glm::mat4(1.0f);

        const glm::mat4 getModel()
        {
            if(lastPosition != position || lastScale != scale || lastRotation != rotation)
            {
                model = glm::scale(glm::mat4(1.0f), scale);
                model = glm::rotate(model, glm::radians(180.0f), rotation);
                model = glm::translate(model, position);
                lastPosition = position;
                lastScale = scale;
                lastRotation = rotation;
            }
            return model;
        }

    };
}