//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include "Shapes.hpp"
#include <glad/glad.h>

Game::Game() : gladStatus(gladLoadGL()), settingsSystem_(&registry_), renderSystem_(&registry_)
{
    aik::Component::Settings settings = settingsSystem_.getSettings();
    window_.create({settings.width, settings.height}, "", settings.style, sf::ContextSettings(16, 0, 4, 4, 3, sf::ContextSettings::Attribute::Core));
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
{
    renderSystem_.initialize();
    auto shader = shaderManager_.load<aik::Shader>("wood"_hs, "assets/shaders/vert.vert", "assets/shaders/frag.frag");
    auto buffer = bufferManager_.load<aik::RenderTarget>("game"_hs);
    auto square = renderSystem_.createSprite(&buffer.get(), &shader.get());
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
    renderSystem_.render();
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
