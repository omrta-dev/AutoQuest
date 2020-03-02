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

        // The following functions will retrieve the view, projection matrices in different configurations
        glm::mat4 &GetView()
        { return view_; }

        glm::mat4 &GetProjection()
        { return projection_; }

        float GetZoom()
        { return zoom_; }

        bool GetCameraType()
        { return isOrtho_; }

    private:
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