//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"

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
    vertices.emplace_back(glm::vec3(0, 0, 0), glm::vec4(1.0, 0.0, 0.0, 1.0));
    vertices.emplace_back(glm::vec3(0, .9, 0), glm::vec4(1.0, 0.0, 0.0, 1.0));
    vertices.emplace_back(glm::vec3(.9, .9, 0), glm::vec4(1.0, 0.0, 0.0, 1.0));

    vao_.createVertexArrayObject();
    vao_.bind();
    vbo_.createVertexBufferObject();
    vbo_.bind();
    vbo_.setData(vertices);
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
    }
}

void Game::processPhysics()
{

}

void Game::renderGraphics()
{
    glClear(GL_COLOR_BUFFER_BIT);
    sf::Shader::bind(&shader_);
    vao_.bind();
    glDrawArrays(GL_LINE_LOOP, 0, 3);
    window_->display();
}
