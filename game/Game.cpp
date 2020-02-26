//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include "glpp/GlmGLSLWrapper.hpp"
#include <iostream>

Game::Game() : window_(new sf::RenderWindow({800, 900}, "", sf::Style::Default, sf::ContextSettings(0, 0, 4, 4, 3))), inputManager_(window_)
{
    gladLoadGL();
    window_->setFramerateLimit(60);
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
    std::vector<aik::Vertex> vertices;
    vertices.emplace_back(glm::vec3(-1, -1, 0), glm::vec4(1.0, 0.0, 0.0, 1.0));
    vertices.emplace_back(glm::vec3(-1, 0, 0), glm::vec4(1.0, 0.0, 0.0, 1.0));
    vertices.emplace_back(glm::vec3(0, 0, 0), glm::vec4(1.0, 0.0, 0.0, 1.0));

    entity_.addVertices(vertices);
    entity_.setScale({.5f, .5f, .5f});
    vao_.createVertexArrayObject();
    vao_.bind();
    vbo_.createVertexBufferObject();
    vbo_.bind();
    vbo_.setData(entity_.getVertices());
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
    sf::Event event = sf::Event();
    while (window_->pollEvent(event))
    {
        inputManager_.processEvents(event);
    }
}

void Game::processPhysics()
{

}

void Game::renderGraphics()
{
    glClear(GL_COLOR_BUFFER_BIT);
    sf::Shader::bind(&shader_);
    shader_.setUniform("model", aik::GlmGLSLWrapper::GlmToGlslMat4(entity_.getModel()));
    vao_.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    window_->display();
}
