//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"
#include "glpp/GlmGLSLWrapper.hpp"
#include <Source/OBJ_Loader.h>

Game::Game() : window_({800, 900}, "", sf::Style::Default, sf::ContextSettings(0, 0, 4, 4, 3))
{
    gladLoadGL();
    window_.setFramerateLimit(144);
}

Game::~Game()
{

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, 800, 900);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    std::vector<aik::Vertex> vertices;

    objl::Loader loader;
    loader.LoadFile("assets/models/monkey.obj");
    vertices.reserve(loader.LoadedVertices.size());
    int t = 0;
    for(const auto& vert : loader.LoadedVertices)
    {
        vertices.emplace_back(glm::vec3(vert.Position.X, vert.Position.Y, vert.Position.Z), glm::vec4(1));
        t++;
    }

    entity_.addVertices(vertices);
    entity_.setScale(glm::vec3(500.0f));
    entity_.setPosition(glm::vec3(750, 750, 0 ));

    vao_.createVertexArrayObject();
    vao_.bind();
    vbo_.createVertexBufferObject();
    vbo_.bind();
    vbo_.setData(entity_.getVertices());
    vao_.configureAttribs();
    ebo_.createVertexBufferObject(aik::BufferTarget::ELEMENT_BUFFER);
    ebo_.bind();
    ebo_.setData(loader.LoadedIndices);

    shader_.loadFromFile("assets/shaders/vert.vert", "assets/shaders/frag.frag");

    auto windowSize = window_.getSize();
    camera_.setLookAt(glm::vec3(0, 0, -2));
    camera_.createOrthographic(0.0f, windowSize.x, windowSize.y, 0.0f, -1000.0f, 1000.0f);

    // Input stuff
    inputManager_.addEvent({sf::Event::JoystickButtonPressed, 0, [this](){ glClearColor(1.0f, 0.0, 0.0, 1.0);}});
    inputManager_.addEvent({sf::Event::JoystickButtonPressed, 1, [this](){ glClearColor(0.0f, 0.0, 1.0, 1.0);}});


    inputManager_.addEvent({sf::Event::KeyPressed, sf::Keyboard::W, [this](){ camera_.move(aik::CameraMovement::UP);}});
    inputManager_.addEvent({sf::Event::KeyPressed, sf::Keyboard::S, [this](){ camera_.move(aik::CameraMovement::DOWN);}});
    inputManager_.addEvent({sf::Event::KeyPressed, sf::Keyboard::A, [this](){ camera_.move(aik::CameraMovement::LEFT);}});
    inputManager_.addEvent({sf::Event::KeyPressed, sf::Keyboard::D, [this](){ camera_.move(aik::CameraMovement::RIGHT);}});
    inputManager_.addEvent({sf::Event::KeyPressed, sf::Keyboard::Add, [this](){ entity_.rotate({0, 1, 0}, 10.0f);}});
    inputManager_.addEvent({sf::Event::KeyPressed, sf::Keyboard::Subtract, [this](){ entity_.rotate({0, 1, 0}, -10.0f);}});
    inputManager_.addEvent({sf::Event::MouseWheelScrolled, 0, [this](){ camera_.zoom(-1.0);}});
    inputManager_.addEvent({sf::Event::MouseWheelScrolled, 1, [this](){ camera_.zoom(1.0);}});


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
    while (window_.pollEvent(event))
    {
        inputManager_.processEvents(event);
        if (event.type == sf::Event::Resized)
        {
            glViewport(0, 0, event.size.width, event.size.height);
            auto windowSize = window_.getSize();
            camera_.createOrthographic(0.0f, windowSize.x, windowSize.y, 0.0f, -1000.0f, 1000.0f);
        }
        if(event.type == sf::Event::Closed)
        {
            window_.close();
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
    shader_.setUniform("view", aik::GlmGLSLWrapper::GlmToGlslMat4( camera_.GetView()));
    shader_.setUniform("projection", aik::GlmGLSLWrapper::GlmToGlslMat4( camera_.GetProjection()));

    vao_.bind();
    glDrawElements(GL_TRIANGLES, 1968, GL_UNSIGNED_INT, 0);
    window_.display();
}
