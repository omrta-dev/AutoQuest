//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include <glad/glad.h>

constexpr unsigned int model_index = 6;

Game::Game() : window_({800, 900}, "", sf::Style::Default, sf::ContextSettings(16, 0, 4, 4, 3)), settingsSystem_(&registry_)
{
    gladLoadGL();
    window_.setFramerateLimit(144);
}

void Game::startGame()
{
    initializeResources();
    while (window_.isOpen())
    {
        gameLoop();
    }
}

void Game::initializeResources()
{
}

void Game::gameLoop()
{
    processInput();
    processPhysics();
    renderGraphics();
}

void Game::processInput()
{

}

void Game::processPhysics()
{

}

void Game::renderGraphics()
{
    window_.display();
}

