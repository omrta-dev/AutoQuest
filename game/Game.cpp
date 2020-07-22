//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include <glad/glad.h>
#include <components/Texture.hpp>
#include "systems/Camera.hpp"
#include <filesystem>
#include <components/Transform.hpp>

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
    window_.create({settings.width, settings.height}, "", settings.style, sf::ContextSettings(16, 8, 4, 4, 3, sf::ContextSettings::Attribute::Core));
    gladLoadGL();
    window_.setFramerateLimit(settings.fps);
    window_.setPosition({settings.position.x, settings.position.y});
    window_.setKeyRepeatEnabled(false);
    window_.setMouseCursorVisible(false);
    inputSystem_.setWindow(&window_);

    // During init, enable debug output
    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback(MessageCallback, 0);
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

    // load shader and buffer resources
    auto shader = shaderManager_.load<aik::resource::Shader>("shader"_hs, "assets/shaders/vert.vert", "assets/shaders/frag.frag");
    auto buffer = bufferManager_.load<aik::resource::RenderTarget>("game"_hs);

    // generate a random hgt file
    std::string hgtPath = "/home/omar/elevations/tt/";
    entt::entity mesh;

    for(const auto & entry : std::filesystem::directory_iterator(hgtPath))
    {
        std::cout << entry.path() << std::endl;
        mesh = renderSystem_.createHGTMesh(&buffer.get(), &shader.get(), entry.path());
        break;
    }

    auto& meshRenderable = registry_.get<aik::Component::Renderable>(mesh);
    auto& transformable = registry_.get<aik::Component::Transform>(mesh);


    // currently there is one global camera
    auto windowSize = window_.getSize();
    auto& camera = registry_.emplace<aik::Camera>(registry_.create());
    camera.setPosition({5.0f, 5.0f, 5.0f});
    camera.setLookAt({0.0f, 0.0f, 0.0f});
    camera.setViewport(0, 0, windowSize.x, windowSize.y);

    // create inputs for enabling polygon fill mode
    inputSystem_.createKeyInput([this]{ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }, sf::Keyboard::L, false);
    inputSystem_.createKeyInput([this]{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }, sf::Keyboard::F, false);
    inputSystem_.createKeyInput([this, &meshRenderable]{ meshRenderable.zScale += .01f;}, sf::Keyboard::Add, false);
    inputSystem_.createKeyInput([this, &meshRenderable]{ meshRenderable.zScale -= .01f;}, sf::Keyboard::Subtract, false);
    inputSystem_.createKeyInput([this, &meshRenderable]{ meshRenderable.shadeMultiplier += .05f;}, sf::Keyboard::Multiply, false);
    inputSystem_.createKeyInput([this, &meshRenderable]{ meshRenderable.shadeMultiplier -= .05f;}, sf::Keyboard::Divide, false);
    inputSystem_.createKeyInput([this, &meshRenderable]{
        meshRenderable.shadeMultiplier = 1.0f;
        meshRenderable.zScale = 1.0f;
        }, sf::Keyboard::R, false);
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
    auto cameraView = registry_.view<aik::Camera>();
    auto& camera = registry_.get<aik::Camera>(cameraView.front());
    camera.moveKeyboard();
    camera.update();
    camera.processMouseButtons();
    camera.processMouseMovement(window_);
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