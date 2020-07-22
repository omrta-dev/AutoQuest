//
// Created by omar on 5/21/20.
//
#include <SFML/Window/Event.hpp>
#include <components/Clickable.hpp>
#include <components/Renderable.hpp>
#include <components/Transform.hpp>
#include <components/Texture.hpp>
#include "RenderSystem.hpp"
#include "Shapes.hpp"
#include "Camera.hpp"
#include "Utility/HGTMesh.hpp"

aik::RenderSystem::RenderSystem(entt::registry *registry)
{
    registry_ = registry;
}

void aik::RenderSystem::initialize()
{
    //  set default clear color
    glClearColor(.4, .6, .8, 1.0);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void aik::RenderSystem::update(float dt)
{
}

void aik::RenderSystem::render()
{
    // clear the framebuffer
    glClear(static_cast<GLbitfield>(GL_COLOR_BUFFER_BIT) | static_cast<GLbitfield>(GL_DEPTH_BUFFER_BIT));
    renderGame();
}

void aik::RenderSystem::renderGame()
{
    auto resources = registry_->view<aik::Component::Renderable, aik::Component::Transform>();
    auto cameraView = registry_->view<aik::Camera>();
    auto& camera = registry_->get<aik::Camera>(cameraView.front());
    for(auto entity: resources)
    {
        // gather components required for rendering the renderable
        const auto &renderable = registry_->get<aik::Component::Renderable>(entity);
        auto &transformable = registry_->get<aik::Component::Transform>(entity);

        if (renderable.buffers != nullptr)
        {
            if (renderable.isVisible)
            {
                // only bind the buffer if it's not bound
                if (lastBufferIndex_ != renderable.buffers)
                {
                    renderable.buffers->bindBuffer(aik::resource::BufferTarget::VERTEX_ARRAY);
                    if (renderable.indices != 0)
                    {
                        renderable.buffers->bindBuffer(aik::resource::BufferTarget::ELEMENT_ARRAY);
                    }
                }
                lastBufferIndex_ = renderable.buffers;

                // ideally these would be cached inside the shader/renderable to prevent quering them but this isn't that expensive of an operation
                renderable.shader->useProgram();
                auto positionLoc = renderable.shader->getUniformLocation("position");
                renderable.shader->glUniform(positionLoc, transformable.position);
                auto scaleLoc = renderable.shader->getUniformLocation("scale");
                renderable.shader->glUniform(scaleLoc, transformable.scale);
                auto modelLoc = renderable.shader->getUniformLocation("model");
                renderable.shader->glUniformMatrix(modelLoc, GL_FALSE, transformable.getModel());
                auto viewLoc = renderable.shader->getUniformLocation("view");
                renderable.shader->glUniformMatrix(viewLoc, GL_FALSE, camera.getViewMatrix());
                auto projectionLoc = renderable.shader->getUniformLocation("projection");
                renderable.shader->glUniformMatrix(projectionLoc, GL_FALSE, camera.getProjectionMatrix());

                // temp values to pass to the shader
                auto zScalerLoc = renderable.shader->getUniformLocation("zScale");
                renderable.shader->glUniform(zScalerLoc, renderable.zScale);
                auto normalizeLoc = renderable.shader->getUniformLocation("shadeMultiplier");
                renderable.shader->glUniform(normalizeLoc, renderable.shadeMultiplier);

                if(registry_->has<aik::Component::Texture>(entity))
                {
                    const auto& texture = registry_->get<aik::Component::Texture>(entity);
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(texture.type, texture.id);
                    auto textureLoc = renderable.shader->getUniformLocation("textureMap");
                    renderable.shader->glUniform(textureLoc, static_cast<GLint>(0));
                    auto layerLoc = renderable.shader->getUniformLocation("layer");
                    renderable.shader->glUniform(layerLoc, static_cast<GLfloat>(1));
                }

                if (renderable.indices != 0)
                {
                    glDrawElements(renderable.drawMode, renderable.indices, GL_UNSIGNED_INT,
                                   reinterpret_cast<GLvoid *>(renderable.indexOffset));
                }
                else
                {
                    glDrawArrays(renderable.drawMode, renderable.indexOffset, renderable.vertices.size());
                }

                // cleanup ensures that resources here are unbound before the next draw call
                if(registry_->has<aik::Component::Texture>(entity))
                {
                    const auto &texture = registry_->get<aik::Component::Texture>(entity);
                    glBindTexture(texture.type, 0);
                }
            }
        }
    }
}

entt::entity aik::RenderSystem::createSprite(aik::resource::RenderTarget* renderTarget, aik::resource::Shader* shader)
{
    // add data to renderTarget
    auto vertices = aik::Shape::Square::getVertices();
    auto indices = aik::Shape::Square::getIndices();
    // we need to offset each index with the last index of the renderTarget to properly seperate objects in the vertex buffer
    std::for_each(indices.begin(), indices.end(), [renderTarget](unsigned int &n){n += renderTarget->lastIndex_;});
    renderTarget->lastIndex_ += vertices.size();

    renderTarget->addBufferData(aik::resource::BufferTarget::VERTEX_BUFFER, vertices);
    renderTarget->addBufferData(aik::resource::BufferTarget::ELEMENT_ARRAY, indices);

    // register component with ECS
    auto square = registry_->create();
    auto& transform = registry_->emplace<aik::Component::Transform>(square);
    transform.position = glm::vec3(1.f);
    transform.scale = glm::vec3(64.f);
    registry_->emplace<aik::Component::Renderable>(square, aik::Shape::Square::getVertices(),
            aik::Shape::Square::getIndices().size(), reinterpret_cast<GLvoid*>(aik::Shape::Square::getIndices().size()), renderTarget, shader);
    return square;
}

// increments will be in 90 arc seconds so we don't end up with a massive mesh
entt::entity aik::RenderSystem::createHGTMesh(aik::resource::RenderTarget *renderTarget, aik::resource::Shader *shader, const std::string& hgtFile)
{
    HGTMesh hgtMesh(hgtFile);
    auto vertices = hgtMesh.getVertices();
    auto indices = hgtMesh.getIndicies();

    // we need to offset each index with the last index of the renderTarget to properly seperate objects in the vertex buffer
    std::for_each(indices.begin(), indices.end(), [renderTarget](unsigned int &n){n += renderTarget->lastIndex_;});
    renderTarget->lastIndex_ += vertices.size();

    // register component with ECS
    renderTarget->addBufferData(aik::resource::BufferTarget::VERTEX_BUFFER, vertices);
    renderTarget->addBufferData(aik::resource::BufferTarget::ELEMENT_ARRAY, indices);

    // register component with ECS
    auto mesh = registry_->create();
    auto& transform = registry_->emplace<aik::Component::Transform>(mesh);
    transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.scale = glm::vec3(10);
    transform.rotation = glm::vec3(0, 1, 1);
    registry_->emplace<aik::Component::Renderable>(mesh, vertices,
                                                   indices.size(), reinterpret_cast<GLvoid*>(indices.size()), renderTarget, shader);
    return mesh;
}
