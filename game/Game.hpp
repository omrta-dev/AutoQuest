//
// Created by omar on 2/19/20.
//

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include "InputManager.hpp"
#include "ResourceManager.hpp"
#include "Mesh.hpp"
#include "glpp/Vertex.hpp"
#include "glpp/VertexArrayObject.hpp"
#include "glpp/VertexBufferObject.hpp"
#include "Camera.hpp"
#include "Model.hpp"

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
    sf::RenderWindow window_;
    sf::Shader shader_;
    aik::InputManager inputManager_;
    aik::ResourceManager resourceManager_;
    aik::VertexArrayObject vao_;
    aik::VertexBufferObject vbo_;
    aik::VertexBufferObject ebo_;
    aik::Camera camera_;
    aik::Model model_, model_2;
};

