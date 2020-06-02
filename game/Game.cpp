//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include <glad/glad.h>
#include "Camera.hpp"
#include "components/Transform.hpp"

Game::Game() : gladStatus(gladLoadGL()), settingsSystem_(&registry_), renderSystem_(&registry_), inputSystem_(&registry_)
{
    aik::Component::Settings settings = settingsSystem_.getSettings();
    window_.create({settings.width, settings.height}, "", settings.style, sf::ContextSettings(16, 0, 4, 4, 3, sf::ContextSettings::Attribute::Core));
    gladLoadGL();
    window_.setFramerateLimit(settings.fps);
    window_.setPosition({settings.position.x, settings.position.y});
    inputSystem_.setWindow(&window_);
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
    auto square2 = renderSystem_.createSprite(&buffer.get(), &shader.get());
    registry_.get<aik::Component::Transform>(square).position = glm::vec2(100.0f);
    registry_.get<aik::Component::Transform>(square2).position = glm::vec2(25.0f);

    // add inputs
    inputSystem_.createKeyInput([square, this]{this->registry_.get<aik::Component::Transform>(square).position = glm::vec2(500.0f);}, sf::Keyboard::W, true);

    // currently there is one global camera
    auto windowSize = window_.getSize();
    auto& camera = registry_.emplace<aik::Camera>(registry_.create());
    camera.setLookAt(glm::vec3(0, 0, -1));
    camera.createOrthographic(0.0f, windowSize.x, windowSize.y, 0.0f, -1000.0f, 1000.0f);
}

void Game::gameLoop()
{
    processInput();
    update();
    render();
}

void Game::processInput()
{
    sf::Event e{};
    while(window_.pollEvent(e))
    {
        if (e.type == sf::Event::Resized)
        {
            glViewport(0, 0, e.size.width, e.size.height);
            auto windowSize = window_.getSize();
            auto cameraView = registry_.view<aik::Camera>();
            auto& camera = registry_.get<aik::Camera>(cameraView.front());
            camera.createOrthographic(0.0f, windowSize.x, windowSize.y, 0.0f, -1000.0f, 1000.0f);
        }
        if(e.type == sf::Event::KeyPressed)
        {
            inputSystem_.processKeyboard(e, 1.0f);
            if (e.key.code == sf::Keyboard::Escape)
            {
                close();
            }
        }
        if(e.type == sf::Event::MouseButtonPressed)
        {
            inputSystem_.processMouse(e, 1.0f);
        }
        if(e.type == sf::Event::JoystickButtonPressed || e.type == sf::Event::JoystickMoved)
        {
            inputSystem_.processJoystick(e, 1.0f);
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
