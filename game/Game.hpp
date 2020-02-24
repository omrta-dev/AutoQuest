//
// Created by omar on 2/19/20.
//

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "InputManager.hpp"
#include "ResourceManager.hpp"
#include "Entity.hpp"
#include "glpp/Vertex.hpp"
#include "glpp/VertexArrayObject.hpp"
#include "glpp/VertexBufferObject.hpp"

class Game
{
public:
    Game();
    ~Game();
    void startGame();
private:
    void initializeResources();
    void gameLoop();
    void processInput();
    void processPhysics();
    void renderGraphics();
    std::shared_ptr<sf::RenderWindow> window_;
    aik::InputManager inputManager_;
    aik::ResourceManager resourceManager_;
    aik::VertexArrayObject vao_;
    aik::VertexBufferObject vbo_;
};

