//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include <glad/glad.h>
#include <components/Texture.hpp>
#include "Camera.hpp"
#include "components/Transform.hpp"



void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message );
}

Game::Game() : gladStatus(gladLoadGL()), settingsSystem_(&registry_), renderSystem_(&registry_), inputSystem_(&registry_)
{
    aik::Component::Settings settings = settingsSystem_.getSettings();
    window_.create({settings.width, settings.height}, "", settings.style, sf::ContextSettings(16, 0, 4, 4, 3, sf::ContextSettings::Attribute::Core));
    gladLoadGL();
    window_.setFramerateLimit(settings.fps);
    window_.setPosition({settings.position.x, settings.position.y});
    window_.setKeyRepeatEnabled(false);
    inputSystem_.setWindow(&window_);

    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );
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
    registry_.get<aik::Component::Transform>(square).position = glm::vec2(0.0f);
    registry_.get<aik::Component::Transform>(square).scale = glm::vec2(800.0f);

    auto &tex = registry_.emplace<aik::Component::Texture>(square, GL_TEXTURE_2D_ARRAY);
    tex.createTextureArray("assets/textures/tilemap.png");

    registry_.emplace<aik::Component::Clickable>(square, []{std::cout << "square was clicked!" << std::endl;}, sf::Mouse::Button::Left, 5.0f);

    // currently there is one global camera
    auto windowSize = window_.getSize();
    auto& camera = registry_.emplace<aik::Camera>(registry_.create());
    camera.setLookAt(glm::vec3(0, 0, -1));
    camera.createOrthographic(0.0f, windowSize.x, windowSize.y, 0.0f, -1000.0f, 1000.0f);

    // add inputs
    inputSystem_.createKeyInput([this]{registry_.get<aik::Camera>(registry_.view<aik::Camera>().front()).move(aik::CameraMovement::UP);}, sf::Keyboard::W, false);
    inputSystem_.createKeyInput([this]{registry_.get<aik::Camera>(registry_.view<aik::Camera>().front()).move(aik::CameraMovement::DOWN);}, sf::Keyboard::S, false);
    inputSystem_.createKeyInput([this]{registry_.get<aik::Camera>(registry_.view<aik::Camera>().front()).move(aik::CameraMovement::LEFT);}, sf::Keyboard::A, false);
    inputSystem_.createKeyInput([this]{registry_.get<aik::Camera>(registry_.view<aik::Camera>().front()).move(aik::CameraMovement::RIGHT);}, sf::Keyboard::D, false);
}

void Game::gameLoop()
{
    float dt = clock_.restart().asSeconds();
    processInput(dt);
    update(dt);
    render();
}

void Game::processInput(const float& dt)
{
    sf::Event e{};
    inputSystem_.processMouse(nullptr, dt);
    inputSystem_.processKeyboard(nullptr, dt);
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
            inputSystem_.processKeyboard(&e, dt);
            if (e.key.code == sf::Keyboard::Escape)
            {
                close();
            }
        }
        if(e.type == sf::Event::MouseButtonPressed || e.type == sf::Event::MouseButtonReleased)
        {
            inputSystem_.processMouse(&e, dt);
        }
        if(e.type == sf::Event::JoystickButtonPressed || e.type == sf::Event::JoystickMoved)
        {
            inputSystem_.processJoystick(e, dt);
        }
        if(e.type == sf::Event::Closed)
        {
            close();
        }
    }
}

void Game::update(const float& dt)
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
