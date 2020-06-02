//
// Created by omar on 5/21/20.
//
#include <SFML/Window/Event.hpp>
#include <components/Clickable.hpp>
#include <components/Renderable.hpp>
#include <components/Transform.hpp>
#include "RenderSystem.hpp"
#include "Shapes.hpp"
#include "Camera.hpp"

aik::RenderSystem::RenderSystem(entt::registry *registry)
{
    registry_ = registry;
}

void aik::RenderSystem::initialize()
{
    glClearColor(.4, .6, .8, 1.0);
}

void aik::RenderSystem::update(float dt)
{

}

void aik::RenderSystem::render()
{
    glClear(static_cast<GLbitfield>(GL_COLOR_BUFFER_BIT) | static_cast<GLbitfield>(GL_DEPTH_BUFFER_BIT));
    renderUi();
    renderGame();
}

void aik::RenderSystem::renderUi()
{
    auto resources = registry_->view<aik::Component::Renderable, aik::Component::Clickable>();
    for(auto entity: resources)
    {
        const auto& renderable = registry_->get<aik::Component::Renderable>(entity);
    }
}

void aik::RenderSystem::renderGame()
{
    auto resources = registry_->view<aik::Component::Renderable, aik::Component::Transform>();
    for(auto entity: resources)
    {
        const auto &renderable = registry_->get<aik::Component::Renderable>(entity);
        const auto &transformable = registry_->get<aik::Component::Transform>(entity);
        auto cameraView = registry_->view<aik::Camera>();
        const auto& camera = registry_->get<aik::Camera>(cameraView.front());
        if (renderable.buffers != nullptr)
        {
            if (renderable.isVisible)
            {
                // only bind the buffer if it's not bound
                if (lastBufferIndex_ == renderable.buffers)
                {
                    renderable.buffers->bindBuffer(BufferTarget::VERTEX_ARRAY);
                    renderable.buffers->bindBuffer(BufferTarget::ELEMENT_ARRAY);
                }
                lastBufferIndex_ = renderable.buffers;

                renderable.shader->useProgram();
                auto positionLoc = renderable.shader->getUniformLocation("position");
                renderable.shader->glUniform(positionLoc, transformable.position);
                auto scaleLoc = renderable.shader->getUniformLocation("scale");
                renderable.shader->glUniform(scaleLoc, transformable.scale);
                auto viewLoc = renderable.shader->getUniformLocation("view");
                renderable.shader->glUniformMatrix(viewLoc, GL_FALSE, camera.view_);
                auto projectionLoc = renderable.shader->getUniformLocation("projection");
                renderable.shader->glUniformMatrix(projectionLoc, GL_FALSE, camera.projection_);
                glDrawElements(renderable.drawMode, renderable.indices, GL_UNSIGNED_INT,
                               reinterpret_cast<GLvoid *>(renderable.indexOffset));
            }
        }
    }
}

entt::entity aik::RenderSystem::createSprite(aik::RenderTarget* renderTarget, aik::Shader* shader)
{
    // add data to renderTarget
    auto vertices = aik::Shape::Square::getVertices();
    auto indices = aik::Shape::Square::getIndices();
    std::for_each(indices.begin(), indices.end(), [renderTarget, vertices](unsigned int &n){n += renderTarget->lastIndex_;});
    renderTarget->lastIndex_ += vertices.size();

    renderTarget->addBufferData(BufferTarget::VERTEX_BUFFER, vertices);
    renderTarget->addBufferData(BufferTarget::ELEMENT_ARRAY, indices, 4);

    // register component with ECS
    auto square = registry_->create();
    auto& transform = registry_->emplace<aik::Component::Transform>(square);
    transform.position = glm::vec2(1.f);
    transform.scale = glm::vec2(50.f);
    registry_->emplace<aik::Component::Renderable>(square, aik::Shape::Square::getVertices(),
            aik::Shape::Square::getIndices().size(), reinterpret_cast<GLvoid*>(aik::Shape::Square::getIndices().size()), renderTarget, shader);
    registry_->emplace<aik::Component::Clickable>(square, []{std::cout << "I was clicked!" << std::endl;}, sf::Mouse::Button::Left);
    return square;
}