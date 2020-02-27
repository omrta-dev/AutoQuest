//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include "glpp/GlmGLSLWrapper.hpp"
#include <iostream>
#include <Source/OBJ_Loader.h>

Game::Game() : window_(new sf::RenderWindow({800, 900}, "", sf::Style::Default, sf::ContextSettings(0, 0, 4, 4, 3))), inputManager_(window_)
{
    gladLoadGL();
    window_->setFramerateLimit(144);
}

Game::~Game()
{

}

void Game::startGame()
{
    initializeResources();
    while (window_->isOpen())
    {
        gameLoop();
    }
}

void Game::initializeResources()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    objl::Loader loader;
    loader.LoadFile("assets/models/monkey.obj");
    auto loadedVertices = loader.LoadedMeshes[0].Vertices;
    std::vector<aik::Vertex> vertices;
    vertices.resize(loadedVertices.size());
    for(const auto& vertex : loadedVertices)
    {
        vertices.emplace_back(glm::vec3(vertex.Position.Z, vertex.Position.Y, vertex.Position.Z), glm::vec4(0.0f));
    }

    std::cout << loader.LoadedIndices.size() << std::endl;

    entity_.addVertices(vertices);
    entity_.setPosition(glm::vec3(-0.25f));
    entity_.setScale(glm::vec3(1.0f));
    vao_.createVertexArrayObject();
    vao_.bind();
    vbo_.createVertexBufferObject();
    vbo_.bind();
    vbo_.setData(entity_.getVertices());
    ebo_.createVertexBufferObject(aik::BufferTarget::ELEMENT_BUFFER);
    ebo_.bind();
    ebo_.setData(loader.LoadedIndices);
    vao_.configureAttribs();

    shader_.loadFromFile("assets/shaders/vert.vert", "assets/shaders/frag.frag");
}

void Game::gameLoop()
{
    processInput();
    processPhysics();
    renderGraphics();
}

void Game::processInput()
{
    sf::Event event;
    while (window_->pollEvent(event))
    {
        inputManager_.processEvents(event);
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Add)
            {
                entity_.rotate({0, 1, 0}, 1.0f);
            }
            else if (event.key.code == sf::Keyboard::Subtract)
            {
                entity_.rotate({0, 1, 0}, -1.0f);
            }
        }
    }
}

void Game::processPhysics()
{

}

void Game::renderGraphics()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sf::Shader::bind(&shader_);
    shader_.setUniform("model", aik::GlmGLSLWrapper::GlmToGlslMat4(entity_.getModel()));

    vao_.bind();
    ebo_.bind();
    glDrawElements(GL_TRIANGLES, 2948, GL_UNSIGNED_INT, nullptr);
    window_->display();
}
