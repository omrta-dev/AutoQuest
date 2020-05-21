//
// Created by omar on 2/19/20.
//

#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <entt/entt.hpp>
#include "systems/SettingsSystem.hpp"

class Game
{
public:
    Game();
    void startGame();
private:
    void initializeResources();
    void gameLoop();
    void processInput();
    void processPhysics();
    void renderGraphics();
    sf::RenderWindow window_;
    entt::registry registry_;
    aik::SettingsSystem settingsSystem_;
};

