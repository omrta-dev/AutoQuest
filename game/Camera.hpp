//
// Created by omar on 2/29/20.
//

#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace aik
{
// This defines the direction a camera is moving in a certain frame
    enum class CameraMovement
    {
        UP, DOWN, LEFT, RIGHT
    };

    class Camera
    {
    public:
        Camera();
        // Controls the type of camera to be created
        void createOrthographic(float left, float right, float top, float bottom, float zNear, float zFar);
        void setLookAt(glm::vec3 eye);
        void move(CameraMovement movement);
        void zoom(float zoomFactor);
        void updateProjection();
        glm::vec2 getScreenToWorld(const glm::vec2& screenCoord);
        glm::mat4 view_, projection_;
        // These are the camera vectors that control position, up dir, and right dir
        glm::vec3 position_;
        bool isOrtho_;
        // Ortho Params
        float leftEdge_, rightEdge_, bottomEdge_, topEdge_;
        // Persp Params
        float zoom_ = 45.0f;
        float zNear_, zFar_;
    };
}