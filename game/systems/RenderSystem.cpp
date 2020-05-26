//
// Created by omar on 5/21/20.
//
#include <components/Clickable.hpp>
#include <components/Renderable.hpp>
#include "RenderSystem.hpp"
#include "components/Renderable.hpp"
#include "Shapes.hpp"

aik::RenderSystem::RenderSystem(entt::registry *registry)
{
    registry_ = registry;
}

void aik::RenderSystem::initialize()
{
    // TODO: All of this gets pulled into the entity Renderable
    vert.loadFromFile("assets/shaders/vert.vert", "assets/shaders/frag.frag");
    uiRenderTarget_.createBuffer(BufferTarget::VERTEX_ARRAY);
    uiRenderTarget_.createBuffer(BufferTarget::VERTEX_BUFFER);
    uiRenderTarget_.createBuffer(BufferTarget::ELEMENT_ARRAY);
    uiRenderTarget_.allocate(BufferTarget::VERTEX_BUFFER, 1024*1024*1024, GL_DYNAMIC_DRAW);
    uiRenderTarget_.allocate(BufferTarget::ELEMENT_ARRAY, 1024*1024*1024, GL_DYNAMIC_DRAW);

    gameRenderTarget_.createBuffer(BufferTarget::VERTEX_ARRAY);
    gameRenderTarget_.createBuffer(BufferTarget::VERTEX_BUFFER);
    gameRenderTarget_.createBuffer(BufferTarget::ELEMENT_ARRAY);
    gameRenderTarget_.allocate(BufferTarget::VERTEX_BUFFER, 1024*1024*1024, GL_DYNAMIC_DRAW);
    gameRenderTarget_.allocate(BufferTarget::ELEMENT_ARRAY, 1024*1024*1024, GL_DYNAMIC_DRAW);
    // TODO: to here

    glClearColor(.4, .6, .8, 1.0);
    glPointSize(10.0f);
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
    uiRenderTarget_.bindBuffer(BufferTarget::VERTEX_ARRAY);

    auto resources = registry_->view<aik::Component::Renderable, aik::Component::Clickable>();
    for(auto entity: resources)
    {
        const auto& renderable = registry_->get<aik::Component::Renderable>(entity);
    }
}

void aik::RenderSystem::renderGame()
{
    auto resources = registry_->view<aik::Component::Renderable>(entt::exclude<aik::Component::Clickable>);
    for(auto entity: resources)
    {
        const auto &renderable = registry_->get<aik::Component::Renderable>(entity);
        if (renderable.isVisible)
        {
            // only bind the buffer if it's not bound
            if(lastBufferIndex_ != renderable.vao)
            {
                gameRenderTarget_.bindBuffer(BufferTarget::VERTEX_ARRAY);
                gameRenderTarget_.bindBuffer(BufferTarget::ELEMENT_ARRAY);
            }
            lastBufferIndex_ = renderable.vao;

            glUseProgram(renderable.program);
            glDrawElements(GL_TRIANGLES, renderable.indices, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(renderable.offset));
        }
    }

}

entt::entity aik::RenderSystem::createSquare()
{
    auto vertices = aik::Shape::Square::getVertices();
    auto indices = aik::Shape::Square::getIndices();
    std::for_each(indices.begin(), indices.end(), [this](unsigned int &n){n += lastIndex_;}); // modify the square entity

    // create and register some attributes to the square entity
    auto square = registry_->create();
    registry_->emplace<aik::Component::Renderable>(square, vertices, indices.size(), reinterpret_cast<GLvoid*>(lastIndex_ * indices.size()));

    // add data to buffers for GPU
    gameRenderTarget_.bindBuffer(BufferTarget::VERTEX_ARRAY);
    gameRenderTarget_.bindBuffer(BufferTarget::VERTEX_BUFFER);
    gameRenderTarget_.addBufferData(BufferTarget::VERTEX_BUFFER, vertices);
    gameRenderTarget_.bindBuffer(BufferTarget::ELEMENT_ARRAY);
    gameRenderTarget_.addBufferData(BufferTarget::ELEMENT_ARRAY, indices);
    gameRenderTarget_.setupVertexAttributes();
    lastIndex_ += vertices.size(); // increment index for indices to work

    return square;
}