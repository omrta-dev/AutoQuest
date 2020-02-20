//
// Created by omar on 2/19/20.
//

#include "Game.hpp"
#include "SFML/Window/Event.hpp"

#include <iostream>

Game::Game() : window_(new sf::RenderWindow({800, 900}, "", sf::Style::Default)), inputManager_(window_)
{
    window_->setFramerateLimit(60);
}

Game::~Game()
{

}

void Game::startGame()
{
    initializeResources();
    while (window_->isOpen())
    {
        gameLoop();
    }
}

void Game::initializeResources()
{
    entities.emplace_back();
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
    while (window_->pollEvent(event))
    {
        inputManager_.processEvents(event);
    }
}

void Game::processPhysics()
{

}

void Game::renderGraphics()
{
    window_->clear(sf::Color(50, 100, 150, 255));
    for(const aik::Entity& entity : entities)
    {
        window_->draw(entity);
    }
    window_->display();
}
