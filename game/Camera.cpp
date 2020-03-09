//
// Created by omar on 2/29/20.
//

#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"
#include <iostream>

aik::Camera::Camera() : position_(0, 0, 0)
{

}

void aik::Camera::createOrthographic(float left, float right, float top, float bottom, float zNear, float zFar)
{
    isOrtho_ = true;
    leftEdge_ = left;
    rightEdge_ = right;
    topEdge_ = top;
    bottomEdge_ = bottom;
    zNear_ = zNear;
    zFar_ = zFar;
    zoom_ = 0.0f;
    projection_ = glm::ortho(left, right, bottom, top, zNear, zFar);
}

void aik::Camera::setLookAt(glm::vec3 eye)
{
    position_ = eye;
    view_ = glm::translate(glm::mat4(1), position_);
}

void aik::Camera::move(aik::CameraMovement movement)
{
    switch (movement)
    {

        case CameraMovement::UP:
            setLookAt(position_ += glm::vec3(0, 1, 0));
            break;
        case CameraMovement::DOWN:
            setLookAt(position_ += glm::vec3(0, -1, 0));
            break;
        case CameraMovement::LEFT:
            setLookAt(position_ += glm::vec3(-1, 0, 0));
            break;
        case CameraMovement::RIGHT:
            setLookAt(position_ += glm::vec3(1, 0, 0));
            break;
    }
}

void aik::Camera::zoom(float zoomFactor)
{
    zoom_ += zoomFactor;
    zoom_ = glm::clamp(zoom_, -50.0f, 50.0f);
    updateProjection();
}

void aik::Camera::updateProjection()
{
    projection_ = glm::ortho(leftEdge_ + (zoom_ * 10), rightEdge_  - (zoom_ * 10), bottomEdge_+ (zoom_ * 10), topEdge_ - (zoom_ * 10), zNear_, zFar_);
}
