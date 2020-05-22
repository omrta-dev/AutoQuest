//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include <glad/glad.h>
#include <iostream>

Game::Game() : settingsSystem_(&registry_), renderSystem_(&registry_)
{
    aik::Settings settings = settingsSystem_.getSettings();
    window_.create({settings.width, settings.height}, "", settings.style, sf::ContextSettings(16, 0, 4, 4, 3));
    gladLoadGL();
    window_.setFramerateLimit(settings.fps);
    window_.setPosition({settings.position.x, settings.position.y});
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
{)
}

void Game::gameLoop()
{
    processInput();
    update();
    render();
}

void Game::processInput()
{
    sf::Event e;
    while(window_.pollEvent(e))
    {
        if(e.type == sf::Event::KeyPressed)
        {
            if (e.key.code == sf::Keyboard::Escape)
            {
                close();
            }
        }
        if(e.type == sf::Event::Closed)
        {
            close();
        }
    }
}

void Game::update()
{

}

void Game::render()
{
    window_.display();
}

void Game::close()
{
    updatePreferences();
    window_.close();
}

void Game::updatePreferences()
{
    auto position = window_.getPosition();
    auto & settings = settingsSystem_.getSettings();
    settings.position = {position.x, position.y};
}
