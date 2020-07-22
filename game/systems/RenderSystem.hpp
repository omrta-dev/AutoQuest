//
// Created by omar on 5/21/20.
//
#pragma once
#include <entt/entt.hpp>
#include <glad/glad.h>
#include <SFML/Graphics/Shader.hpp>
#include <components/Renderable.hpp>
#include <resource/RenderTarget.hpp>

namespace aik
{
    class RenderSystem
    {
    public:
        // Class must be initialized with this constructor to guarantee a registry is initialized
        explicit RenderSystem(entt::registry * registry);
        // initializes opengl features, must be called after an openGL context has been created
        void initialize();
        // updates each renderable with the elapsed time since last frame
        void update(float dt);
        // renders each type/category of renderables
        void render();
        // creates a sprite and 1. links the shader id to this renderable 2. adds the vertex/index data to to the renderTarget
        entt::entity createSprite(aik::resource::RenderTarget* renderTarget, aik::resource::Shader* shader);
        entt::entity createHGTMesh(aik::resource::RenderTarget* renderTarget, aik::resource::Shader* shader, const std::string& hgtFile);
    private:
        // renders all renderables that aren't part of the UI
        void renderGame();
        entt::registry * registry_;
        // is used to ensure we don't bind an already bound buffer, alternatively we can use a bindless buffer
        aik::resource::RenderTarget * lastBufferIndex_ = nullptr;
    };
}
